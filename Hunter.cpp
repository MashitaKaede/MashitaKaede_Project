#include "Hunter.h"

Hunter::Hunter()

    :state(State_Hunter::Idle),
     target(nullptr),
     aStar(nullptr),
     chaseDirect(false),
     targetCMIndex(-1),
     atkCoolTimeNow(false),
     carryTarget(nullptr)
{
    animator = new Animator();
}

Hunter::~Hunter()
{
    Term();
}

void Hunter::Init()
{
	modelHandle = MV1LoadModel("Assets/Model/Character/Hunter/Hunter.mv1");
	tag = "Hunter";
	name = "Hunter";

    // 座標変換
	position = VGet(3200.0f, 0.0f, 2200.0f);
    scale = VGet(2.0f, 2.0f, 2.0f);
	Transform(modelHandle);

    // カプセルコライダーの初期化
	radius = Character_Default_ColliderRadius;
    SetParent(this);

    atkCollider = new CapsuleCollider();
    atkCollider->radius = Hunter_AtkRadius;
    atkCollider->startPos = position;
    atkCollider->startPos.y += Hunter_AtkHeight;
    atkCollider->endPos = VAdd(atkCollider->startPos, VScale(forward, Hunter_AtkRange));
    atkCollider->SetParent(GetParent());
    atkCollider->colliderName = "HunterAtk";
    atkCollider->isCheck = false;

    UpdateCollider();

    // 経路探索用クラス作成
	aStar = new AStar();

    // ステータス初期化
	moveSpeed = Hunter_Default_MoveSpeed;

    animator->SetModelHandle(modelHandle);
    animator->LoadAnim(CharacterName::Hunter);
    animator->PlayAnim(0);
}

void Hunter::Term()
{
    delete animator;
    delete aStar;
}

void Hunter::InitAStar(std::vector<PolyInfo> AStarPoly, int AStarPolyNum, std::vector<PolyInfo> WallPoly, int WallPolyNum)
{
    // 経路探索用ポリゴンセット
	aStar->SetPolyVec_AStar(AStarPoly, AStarPolyNum);
	aStar->SetPolyVec_Wall(WallPoly, WallPolyNum);
}

void Hunter::Update()
{
    Draw();
    UpdateCollider();

    if (mode == Mode::CPU)
    {
        AI();
    }
    else if(mode == Mode::Player)
    {
        Player();
    }

    animator->Update();
}

void Hunter::AddSurvivor(Survivor* survivor)
{
    survivorVec.push_back(survivor);
    survivorNum++;
}

void Hunter::Idle()
{
    // ターゲットが見つかっていなければ
    if (target == nullptr)
    {
        state = State_Hunter::SearchTarget;
    }
    // 見つかれば追いかける
    else
    {
        state = State_Hunter::ChaseTarget;
    }
}

void Hunter::DecideNextCipherMachine()
{
    float minDis = -1.0f; // 一番近い距離
    int index = -1;       // ↑の暗号機の番号
    int checkCM_Num = (int)check_CM_IndexVec.size(); // チェック済み暗号機の数

    // 全部確認済みだったらリセット
    if (checkCM_Num == Map_CipherMachineNum)
    {
        // 最後に確認した暗号機は確認済みにしておく
        int lastCMIndex = check_CM_IndexVec.back();
        ResetNextCipherMachine();
        check_CM_IndexVec.push_back(lastCMIndex);
        checkCM_Num = 1;
    }

    // 暗号機の数分
    for (int i = 0; i < Map_CipherMachineNum; i++)
    {
        // 自分との距離を出す
        float dis = VSize(VSub(cipherMachineVec[i]->position, position));

        // 確認済みフラグ
        bool checked = false;

        // チェック済み暗号機があれば
        if (checkCM_Num != 0)
        {
            // チェック済み暗号機の数分
            for (int j = 0; j < checkCM_Num; j++)
            {
                // 確認済みだったら
                if (i == check_CM_IndexVec[j])
                {
                    // 確認済みフラグを立てる
                    checked = true;
                }
            }
        }

        // 確認済みでなければ
        if (checked == false)
        {
            // 最初だったら保存
            if (minDis < 0)
            {
                minDis = dis;
                index = i;
            }
            else
            {
                // さらに近いのが出てきたらそっちを保存
                if (minDis > dis)
                {
                    minDis = dis;
                    index = i;
                }
            }
        }
    }

    // 確認済み暗号機の番号保存,目標暗号機の設定,暗号機巡り開始フラグを立てる
    check_CM_IndexVec.push_back(index);
    targetCM = cipherMachineVec[index];
    goCM = true;
    targetCMIndex = index;
    // 経路探索初期化
    aStar->goal = false;
    aStar->ResetPolyVec();
    aStar->SetStartGoalPolyIndex(position, targetCM->position);
}

void Hunter::Search_Target()
{
    // 目標暗号機が決まったら
    if (goCM)
    {
        // 経路探索
        if (aStar->goal == false)
        {
            aStarNow = true;
            animator->PlayAnim((int)AnimName_Hunter::Idle);
            aStar->Update();
        }
        // 経路が決まったら
        if (aStar->goal == true)
        {
            aStarNow = false;
            animator->PlayAnim((int)AnimName_Hunter::Walk);
            float dis = 0.0f;

            // 経路があれば目標座標を辿っていく
            if (aStar->finalPosVec.size() > 0)
            {
                VECTOR oldPos = position;
                moveVec = VScale(VNorm(VSub(aStar->finalPosVec[aStar->nowRootNum], position)), moveSpeed * FrameTime);
                position = VAdd(position, moveVec);
                dis = VSize(VSub(position, aStar->finalPosVec[aStar->nowRootNum]));
            }

            // 目標座標と近づいたら
            if (dis < 100.0f)
            {
                //nowRootNumには目標座標の残りの合計数が入ってる

                // 最後までたどり着いたら
                if (aStar->nowRootNum == 0)
                {
                    // 次の暗号機を決める
                    DecideNextCipherMachine();
                }

                // 次の座標へ
                else
                {
                    aStar->nowRootNum--;
                }
            }
        }

        Rotate();
        Transform(modelHandle);
    }
    // もし次の暗号機が決まってなければ
    else
    {
        animator->PlayAnim((int)AnimName_Hunter::Idle);
        // 次の暗号機を決める
        DecideNextCipherMachine();
    }

    // 全サバイバーとの距離を確認,距離が一定以下かつ捕まってなければターゲットにする
    for (int i = 0; i < survivorNum; i++)
    {
        float dis = VSize(VSub(survivorVec[i]->position, position));
        if (dis <= Hunter_StartChaseSurvivorDis && survivorVec[i]->GetState() != State_Survivor::Restrainting && survivorVec[i]->GetState() != State_Survivor::Flying && survivorVec[i]->GetState() != State_Survivor::GameOver && survivorVec[i]->GetState() != State_Survivor::GameClear)
        {
            // 空いてるロケットチェアが無いかつダウン状態だったらターゲットにしない
            if (DecideRocketChair(false) == false)
            {
                break;
            }
            state = State_Hunter::ChaseTarget;
            target = survivorVec[i];
            goCM = false;
            targetCMIndex = -1;
            ResetNextCipherMachine();
            break;
        }
    }
}

void Hunter::Chase_Target()
{
    if (target->GetState() == State_Survivor::Restrainting ||
        target->GetState() == State_Survivor::Flying ||
        target->GetState() == State_Survivor::GameOver ||
        target->GetState() == State_Survivor::GameClear)
    {
        target = nullptr;
        state = State_Hunter::Idle;
        return;
    }

    // サバイバーとの間に壁があるか確認（あったら経路探索、無ければ直接追いかける）
    VECTOR lineStartPos = position;
    lineStartPos.y += 100.0f;
    VECTOR lineEndPos = target->position;
    lineEndPos.y += 100.0f;

    // 壁あるか確認
    if (collisionMnagaer->GetWallHit(lineStartPos, lineEndPos) == true)
    {
        chaseDirect = false;
    }
    else
    {
        chaseDirect = true;
    }

    // 壁が無ければ
    if (chaseDirect == true)
    {
        float dis = VSize(VSub(target->position, position));

        // 一定距離まで近づく
        if (dis > Hunter_MinDis_Survivor)
        {
            animator->PlayAnim((int)AnimName_Hunter::Walk);
            moveVec = VScale(VNorm(VSub(target->position, position)), 10.0f);
            position = VAdd(position, moveVec);
        }
        // 一定距離以内なら
        else
        {
            // ターゲットがダウン状態なら持ち上げる
            if (target->GetState() == State_Survivor::Fall)
            {
                animator->PlayAnim((int)AnimName_Hunter::Idle);
                // 自分を持ち上げ中、ターゲットを担がれ中にする
                state = State_Hunter::PickUpTarget;
                target->SetState(State_Survivor::LiftedUp);
            }
        }

        // 探索した経路をリセットし続ける
        aStar->goal = false;
        aStar->ResetPolyVec();
        aStar->SetStartGoalPolyIndex(position, target->position);
    }

    // 壁が有無にかかわらず探索は進める
    if (aStar->goal == false)
    {
        aStarNow = true;
        aStar->Update();
        if (chaseDirect == false)
        {
            animator->PlayAnim((int)AnimName_Hunter::Idle);
        }
    }

    // 壁がある＆経路探索が終了していれば追いかける
    if (aStar->goal == true && chaseDirect == false)
    {
        aStarNow = false;
        animator->PlayAnim((int)AnimName_Hunter::Walk);
        float dis = 0.0f;

        // 経路があれば目標座標を辿っていく
        if (aStar->finalPosVec.size() > 0)
        {
            VECTOR oldPos = position;
            moveVec = VScale(VNorm(VSub(aStar->finalPosVec[aStar->nowRootNum], position)), 10.0f);
            position = VAdd(position, moveVec);
            dis = VSize(VSub(position, aStar->finalPosVec[aStar->nowRootNum]));
        }

        // 目標座標と近づいたら
        if (dis < 100.0f)
        {
            //nowRootNumには目標座標の残りの合計数が入ってる

            // 最後までたどり着いたら
            if (aStar->nowRootNum == 0)
            {
                // 探索経路をリセット
                aStar->goal = false;
                aStar->ResetPolyVec();
                aStar->SetStartGoalPolyIndex(position, target->position);
            }
            // 次の座標へ
            else
            {
                aStar->nowRootNum--;
            }
        }
    }

    // キャラの回転＆座標変換
    Rotate();
    Transform(modelHandle);

    // ハンターとサバイバーの距離を測る
    float dis = VSize(VSub(target->position, position));

    // 距離が一定以上離れたら見失う
    if (dis >= Hunter_MissSurvivorDis)
    {
        // 暗号機巡り開始
        state = State_Hunter::SearchTarget;
        target = nullptr;
        ResetNextCipherMachine();
        DecideNextCipherMachine();
    }
    else
    {
        if (target->GetState() != State_Survivor::Fall)
        {
            // 距離が一定以下で攻撃
            Attack_AI(dis);
        }        
    }

    ChangeTarget();
}

void Hunter::AttackHit()
{
    stopTime += FrameTime;
    if (animator->GetPlayEnd() == true)
    {
        stopTime = 0.0f;
        state = State_Hunter::Idle;
    }
}

void Hunter::PickUpTarget()
{
    if (animator->GetPlayEnd() == true)
    {
        // 目標ロケットチェアが決まったら
        if (DecideRocketChair(true) == true)
        {
            // ターゲット運び状態に
            state = State_Hunter::CarryTarget;

            // ターゲットの座標変換
            VECTOR newTargetPos = position;
            newTargetPos.y += 120.0f;
            newTargetPos = VAdd(newTargetPos, VScale(VScale(right, -1.0f), 80.0f));
            target->position = newTargetPos;
            target->rotation = rotation;
            target->Transform(target->GetModelHandle());
        }
        // もし使えるロケットチェアが無ければ
        else
        {
            target->SetState(State_Survivor::Fall);
            target->position = position;
            target->Transform(target->GetModelHandle());
            state = State_Hunter::Idle;
        }
    }
}

void Hunter::AI()
{
    switch (state)
    {
    case State_Hunter::None:
        break;
    case State_Hunter::Idle:
        animator->PlayAnim((int)AnimName_Hunter::Idle);
        Idle();
        break;
    case State_Hunter::SearchTarget:
        Search_Target();
        break;
    case State_Hunter::ChaseTarget:
        Chase_Target();
        break;
    case State_Hunter::Attack:
        break;
    case State_Hunter::AtkHit:
        animator->PlayAnim((int)AnimName_Hunter::AttackHit);
        AttackHit();  
        break;
    case State_Hunter::PickUpTarget:
        animator->PlayAnim((int)AnimName_Hunter::PickUp);
        PickUpTarget();
        break;
    case State_Hunter::CarryTarget:
        CarryTarget_AI();
        break;
    case State_Hunter::Restrain:
        animator->PlayAnim((int)AnimName_Hunter::Restrain);
        if (animator->GetPlayEnd() == true)
        {
            state = State_Hunter::Idle;
        }
    default:
        break;
    }
}

void Hunter::ChangeTarget()
{
    float nowDis = 0.0f;
    if (target != nullptr)
    {
        nowDis = VSize(VSub(target->position, position));
    }
    
    // 全サバイバーとの距離を確認,距離が一定以下かつ捕まってなければターゲットにする
    for (int i = 0; i < survivorNum; i++)
    {
        float dis = VSize(VSub(survivorVec[i]->position, position));
        if (dis < nowDis && survivorVec[i]->GetState() != State_Survivor::Restrainting && survivorVec[i]->GetState() != State_Survivor::Fall && survivorVec[i]->GetState() != State_Survivor::Flying && survivorVec[i]->GetState() != State_Survivor::GameOver && survivorVec[i]->GetState() != State_Survivor::GameClear)
        {
            // 空いてるロケットチェアが無いかつダウン状態だったらターゲットにしない
            if (DecideRocketChair(false) == false)
            {
                break;
            }
            target = survivorVec[i];
            goCM = false;
            targetCMIndex = -1;
            ResetNextCipherMachine();
            break;
        }
    }
}

void Hunter::ResetNextCipherMachine()
{
    check_CM_IndexVec.clear();
}

void Hunter::Player()
{
    switch (state)
    {
    case State_Hunter::None:
        break;
    case State_Hunter::Idle:
        animator->PlayAnim((int)AnimName_Hunter::Idle);
        CheckMove();
        Attack_Player();
        break;
    case State_Hunter::Move:
        animator->PlayAnim((int)AnimName_Hunter::Walk);
        Move();
        Attack_Player();
        break;
    case State_Hunter::AtkHit:
        animator->PlayAnim((int)AnimName_Hunter::AttackHit);
        AttackHit();
        break;
    case State_Hunter::PickUpTarget:
        animator->PlayAnim((int)AnimName_Hunter::PickUp);
        if (animator->GetPlayEnd() == true)
        {
            state = State_Hunter::CarryTarget;
        }
        break;
    case State_Hunter::CarryTarget:
        CarryTarget_Player();
        Move();
        break;
    case State_Hunter::Restrain:
        animator->PlayAnim((int)AnimName_Hunter::Restrain);
        if (animator->GetPlayEnd() == true)
        {
            state = State_Hunter::Idle;
        }
    default:
        break;
    }
}

void Hunter::Move()
{
    // XBoxコントローラーが繋がってたらXBox操作
    if (GetJoypadNum() > 0)
    {
        Move_XBox();
    }
    // 繋がってない場合キーボード操作
    else
    {
        Move_Key();
    }
}

void Hunter::Debug()
{
    Character::Debug();
    DrawFormatString(600, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "State : %d", (int)state);
    Debug_DispCnt++;
}

void Hunter::CheckMove()
{
    // スティック入力があれば
    if (Input::GetInstance()->GetStickInput(true, false) == true)
    {
        // ステートの変更
        state = State_Hunter::Move;
    }
}

void Hunter::Move_Key()
{
    moveVec = VGet(0.0f, 0.0f, 0.0f);

    // 移動方向 (カメラの向きを考慮)
    if (Input::GetInstance()->GetKye(KeyName::W))
    {
        moveVec = VAdd(moveVec, VGet(cosf(cameraRotH), 0.0f, sinf(cameraRotH)));
    }
    if (Input::GetInstance()->GetKye(KeyName::S))
    {
        moveVec = VAdd(moveVec, VGet(-cosf(cameraRotH), 0.0f, -sinf(cameraRotH)));
    }
    if (Input::GetInstance()->GetKye(KeyName::A))
    {
        moveVec = VAdd(moveVec, VGet(-sinf(cameraRotH), 0.0f, cosf(cameraRotH)));
    }
    if (Input::GetInstance()->GetKye(KeyName::D))
    {
        moveVec = VAdd(moveVec, VGet(sinf(cameraRotH), 0.0f, -cosf(cameraRotH)));
    }

    // 壁との当たり判定用(線の始点)
    VECTOR startPos = position;
    startPos.y += Collision_Line_AddHeight;

    // 移動後の座標決定
    position = VAdd(position, VScale(moveVec, moveSpeed * FrameTime));

    // 壁との当たり判定用(線の終点)
    VECTOR endPos = VAdd(position, moveVec);
    endPos.y += Collision_Line_AddHeight;

    DrawLine3D(startPos, endPos, GetColor(255, 0, 0));

    slideVec = collisionMnagaer->GetSlideVec(startPos, endPos, moveVec);

    DrawLine3D(endPos, VAdd(endPos, VScale(slideVec, 10.0f)), GetColor(255, 0, 0));

    if (VSize(slideVec) != VSize(moveVec)) //(VSize(slideVec) > 0.0f)
    {
        startPos.y = 0.0f;
        position = VAdd(startPos, slideVec);
    }

    // キャラクターの回転処理
    if (VSize(moveVec) > 0.0f)
    {
        rotation.y = atan2f(moveVec.x, moveVec.z) + DX_PI_F;
    }

    // 角度を -180° ～ 180° に正規化
    if (rotation.y > DX_PI_F)
    {
        rotation.y -= DX_PI_F * 2;
    }
    if (rotation.y < -DX_PI_F)
    {
        rotation.y += DX_PI_F * 2;
    }

    // 座標変換
    Transform(modelHandle);
}

void Hunter::Move_XBox()
{
    // 移動ベクトルの初期化
    moveVec = VGet(0.0f, 0.0f, 0.0f);

    // スティック入力があれば
    if (Input::GetInstance()->GetStickInput(true, false))
    {
        // 移動ベクトルのセット（スティック角度＋カメラ角度＋調整用角度）
        float angle = Input::GetInstance()->GetStickAngle_L() + cameraRotH - DX_PI_F / 2;
        moveVec = VGet(cosf(angle), 0.0f, sinf(angle));
    }
    else
    {
        if (state != State_Hunter::CarryTarget)
        {
            state = State_Hunter::Idle;
        }        
    }

    // 移動ベクトル*速度*フレームタイム
    moveVec = VScale(moveVec, moveSpeed * FrameTime);


    // 壁との当たり判定用(線の始点)
    VECTOR oldPos = position;             // 移動前の場所
    oldPos.y += Collision_Line_AddHeight; // 少し上げる

    // 移動後の座標決定
    position = VAdd(position, moveVec);

    // 壁との当たり判定用(線の終点)
    VECTOR nextPos = position;             // 移動後の場所
    nextPos.y += Collision_Line_AddHeight; // 少し上げる

    // 移動前の移動ベクトル表示
    //DrawLine3D(oldPos, nextPos, GetColor(255, 0, 0));

    // 壁ずりベクトル取得（無ければそのままmoveVecが帰ってくる）
    slideVec = collisionMnagaer->GetSlideVec(oldPos, nextPos, moveVec);

    // 壁ずりベクトル適用後の移動ベクトル表示
    //DrawLine3D(nextPos, VAdd(nextPos, slideVec), GetColor(255, 0, 0));

    // 移動ベクトルと壁ずりベクトルが同じでなければ移動前の場所に壁ずりベクトルを足す
    if (VSize(slideVec) != VSize(moveVec))
    {
        oldPos.y = 0.0f;
        position = VAdd(oldPos, slideVec);
    }

    // キャラクターの回転処理
    Rotate();

    // 座標変換
    Transform(modelHandle);
}

void Hunter::Attack_Player()
{
    if (Input::GetInstance()->GetButtonDown(Button_R))
    {
        if (atkCoolTimeNow == false)
        {
            animator->PlayAnim((int)AnimName_Hunter::Attack);
            atkCoolTimeNow = true;
        }
    }

    if (atkCoolTimeNow == true)
    {
        if (animator->GetPlayIndex() != (int)AnimName_Hunter::Attack)
        {
            atkCoolTimeNow = false;
        }
        else
        {
            if (animator->GetPlayAnimRate() > 0.4f && animator->GetPlayAnimRate() < 0.5f)
            {
                atkCollider->isCheck = true;
            }
            else
            {
                atkCollider->isCheck = false;
            }
        }
    }
}

void Hunter::CarryTarget_Player()
{
    // スティック入力があれば
    if (Input::GetInstance()->GetStickInput(true, false) == true)
    {
        animator->PlayAnim((int)AnimName_Hunter::CarryWalk);
    }
    else
    {
        animator->PlayAnim((int)AnimName_Hunter::Idle);
    }
    VECTOR newTargetPos = position;
    newTargetPos.y += 120.0f;
    newTargetPos = VAdd(newTargetPos, VScale(VScale(right, -1.0f), 80.0f));
    carryTarget->position = newTargetPos;
    carryTarget->rotation = rotation;
    carryTarget->Transform(carryTarget->GetModelHandle());
}

void Hunter::UpdateCollider()
{
    // カプセルの始点と終点更新
    startPos = position;
    endPos = startPos;
    endPos.y += Character_Default_ColliderHeight;

    atkCollider->startPos = position;
    atkCollider->startPos.y += Hunter_AtkHeight;
    atkCollider->endPos = VAdd(atkCollider->startPos, VScale(forward, Hunter_AtkRange));
}

void Hunter::Attack_AI(float DisFromTarget)
{
    if (DisFromTarget < Hunter_AtkRange)
    {
        if (atkCoolTimeNow == false)
        {
            animator->PlayAnim((int)AnimName_Hunter::Attack);
            atkCoolTimeNow = true;
        }
    }

    if (atkCoolTimeNow == true)
    {
        if (animator->GetPlayIndex() != (int)AnimName_Hunter::Attack)
        {
            atkCoolTimeNow = false;
        }
        else
        {
            if (animator->GetPlayAnimRate() > 0.4f && animator->GetPlayAnimRate() < 0.5f)
            {
                atkCollider->isCheck = true;
            }
            else
            {
                atkCollider->isCheck = false;
            }
        }
    }
}

void Hunter::CarryTarget_AI()
{
    VECTOR newTargetPos = position;
    newTargetPos.y += 120.0f;
    newTargetPos = VAdd(newTargetPos, VScale(VScale(right, -1.0f), 80.0f));
    target->position = newTargetPos;
    target->rotation = rotation;
    target->Transform(target->GetModelHandle());

    // 経路探索
    if (aStar->goal == false)
    {
        aStarNow = true;
        animator->PlayAnim((int)AnimName_Hunter::Idle);
        aStar->Update();
    }
    // 経路が決まったら
    if (aStar->goal == true)
    {
        aStarNow = false;
        animator->PlayAnim((int)AnimName_Hunter::CarryWalk);
        float dis = 0.0f;

        // 経路があれば目標座標を辿っていく
        if (aStar->finalPosVec.size() > 0)
        {
            VECTOR oldPos = position;
            moveVec = VScale(VNorm(VSub(aStar->finalPosVec[aStar->nowRootNum], position)), moveSpeed * FrameTime);
            position = VAdd(position, moveVec);
            dis = VSize(VSub(position, aStar->finalPosVec[aStar->nowRootNum]));
        }

        // 目標座標と近づいたら
        if (dis < 100.0f)
        {
            //nowRootNumには目標座標の残りの合計数が入ってる

            // 最後までたどり着いたら
            if (aStar->nowRootNum == 0)
            {
                target->SetRocketChair(rocketChairVec[targetRCIndex]);
                target->AddRestraintCnt();
                target->SetState(State_Survivor::Restrainting);
                target->position = rocketChairVec[targetRCIndex]->position;
                target->Transform(target->GetModelHandle());
                rocketChairVec[targetRCIndex]->SetSurvivor(target);
                target = nullptr;
                state = State_Hunter::Restrain;
            }

            // 次の座標へ
            else
            {
                aStar->nowRootNum--;
            }
        }
    }

    Rotate();
    Transform(modelHandle);
}

bool Hunter::DecideRocketChair(bool startAStar)
{
    float minDis = -1.0f; // 一番近い距離
    int index = -1;       // ↑の暗号機の番号
    int unusedRCNum = 0;  // 使われてないロケットチェアの数

    // ロケットチェアの数分
    for (int i = 0; i < Map_RocketChairNum; i++)
    {
        if (rocketChairVec[i]->used == false)
        {
            unusedRCNum++;
            // 自分との距離を出す
            float dis = VSize(VSub(rocketChairVec[i]->position, position));

            // 最初だったら保存
            if (minDis < 0)
            {
                minDis = dis;
                index = i;
            }
            else
            {
                // さらに近いのが出てきたらそっちを保存
                if (minDis > dis)
                {
                    minDis = dis;
                    index = i;
                }
            }
        }
    }

    if (unusedRCNum == 0)
    {
        return false;
    }
    else
    {
        targetRCIndex = index;

        if (startAStar == true)
        {
            // 経路探索初期化
            aStar->goal = false;
            aStar->ResetPolyVec();
            rocketChairVec[index]->used = true;
            aStar->SetStartGoalPolyIndex(position, rocketChairVec[index]->position);
        }
        
        return true;
    }
}

void Hunter::OnCollisionHit(Collider* Collider, GameObject* Parent)
{
    if (Parent->tag == "Survivor")
    {
        Survivor* survivor = (Survivor*)Parent;
        if (survivor->GetState() == State_Survivor::Fall)
        {
            if (Input::GetInstance()->GetButtonDown(Button_B) && mode == Mode::Player && carryTarget == nullptr)
            {
                state = State_Hunter::PickUpTarget;
                carryTarget = survivor;
                carryTarget->SetState(State_Survivor::LiftedUp);
            }
            if (mode == Mode::Player && state != State_Hunter::PickUpTarget && state != State_Hunter::CarryTarget)
            {
                VECTOR drawPos = survivor->position;
                drawPos.y += 250.0f;
                DrawBillboard3D(drawPos, 0.5f, 0.5f, 60.0f, 0.0f, GameManager::GetInstance()->GetImageHandle(UI_Name::Button_B), true);
            }
        }
    }
    else if (Parent->tag == "RocketChair")
    {
        if (state == State_Hunter::CarryTarget)
        {
            RocketChair* rocketChair = (RocketChair*)Parent;
            if (mode == Mode::Player)
            {
                VECTOR drawPos = rocketChair->position;
                drawPos.y += 250.0f;
                DrawBillboard3D(drawPos, 0.5f, 0.5f, 60.0f, 0.0f, GameManager::GetInstance()->GetImageHandle(UI_Name::Button_B), true);
            }
            if (Input::GetInstance()->GetButtonDown(Button_B) && mode == Mode::Player)
            {
                state = State_Hunter::Restrain;
                
                carryTarget->SetRocketChair(rocketChair);
                carryTarget->AddRestraintCnt();
                carryTarget->SetState(State_Survivor::Restrainting);
                carryTarget->position = Parent->position;
                carryTarget->Transform(carryTarget->GetModelHandle());
                rocketChair->SetSurvivor(carryTarget);
                carryTarget = nullptr;
            }
        }        
    }
}
