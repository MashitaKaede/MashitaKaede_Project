#include "Survivor.h"
#include "Hunter.h"
#include "RocketChair.h"

Survivor::Survivor()

	:state(State_Survivor::Idle),
	 decodingCipherMachine(nullptr),
     inputKeyPad(nullptr),
     rocketChair(nullptr),
     hunter(nullptr),
     rescueSurvivor(nullptr),
     hp(4),
     restraintCnt(0),
     decodingCMIndex(-1),
     gameOverRate(0.0f),
	 addDecodingRate(Survivor_Default_AddDecodingRate),
     addInputRate(Survivor_Default_AddInputRate),
     isVulnerable(true),
     isRescuing(false),
     actionDisabled(false),
     aStar(nullptr),
     targetCipherMachine(nullptr),
     targetKeyPad(nullptr),
     cipherMachineVec(),
     keyPadVec(),
     otherSurvivorVec(),
     posVec_Exit_AStar(),
     runAwayPointVec(),
     inputKeyPadIndex(-1),
     decodingComplete(false),
     runAwayPointIndex(-1)
{
}

Survivor::~Survivor()
{
    if (aStar != nullptr)
    {
        delete aStar;
    }
}

// サバイバーの基本的なUpdate
void Survivor::DefaultSurvivorUpdate()
{
    // 描画
    Draw();

    // コライダー更新
    UpdateCollider();

    // AIのUpdate
    if (mode == Mode::CPU)
    {
        AI();
    }
    // プレイヤーのUpdate
    else
    {
        Player();
    }

    // アニメーションの更新
    animator->Update();
}

// 経路探索用ポリゴンセット
void Survivor::InitAStar(std::vector<PolyInfo> AStarPoly, int AStarPolyNum, std::vector<PolyInfo> WallPoly, int WallPolyNum)
{
    aStar = new AStar();
    aStar->SetPolyVec_AStar(AStarPoly, AStarPolyNum);
    aStar->SetPolyVec_Wall(WallPoly, WallPolyNum);
}

// アイドル時の処理
void Survivor::Idle()
{
    // 必要分の解読が終わってなければ
    if (decodingComplete == false)
    {
        // 向かう暗号機を決める
        if (targetCipherMachine == nullptr)
        {
            DecideDecodigCipherMachine();
        }
        // 決まれば
        else
        {
            // ステートを変更
            state = State_Survivor::GoCipherMachine;
        }
    }
    // 終わっていれば
    else
    {
        // 向かうキーパッドを決める
        if (targetKeyPad == nullptr)
        {
            DecideInputKeyPad();
        }
        // 決まれば
        else
        {
            // ステートを変更
            state = State_Survivor::GoKeyPad;
        }
    }
}

// 目標暗号機を決める
void Survivor::DecideDecodigCipherMachine()
{
    float minDis = -1.0f; // 一番近い距離
    int index = -1;       // ↑の暗号機の番号

    // 暗号機の数分
    for (int i = 0; i < Map_CipherMachineNum; i++)
    {
        // 自分との距離を出す
        float dis = VSize(VSub(cipherMachineVec[i]->position, position));

        // 解読未完了なら
        if (cipherMachineVec[i]->GetDecodingComplete() == false)
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

    if (index != -1)
    {
        targetCipherMachine = cipherMachineVec[index];
        // 経路探索初期化
        aStar->ResetPolyVec();
        aStar->SetStartGoalPolyIndex(position, targetCipherMachine->position);
    }
}

// 目標暗号機に向かう
void Survivor::GoTargetCipherMachine()
{
    if (targetCipherMachine == nullptr)
    {
        DecideDecodigCipherMachine();
    }

    if(targetCipherMachine->GetDecodingComplete() == true)
    {
        targetCipherMachine = nullptr;
        state = State_Survivor::Idle;
        return;
    }

    // 目標座標との間に壁が無ければ直接向かう
    if (collisionMnagaer->GetWallHit(position, targetCipherMachine->position) == false)
    {
        animator->PlayAnim((int)AnimName_Survivor::Run);
        moveVec = VScale(VNorm(VSub(targetCipherMachine->position, position)), moveSpeed * FrameTime);
        position = VAdd(position, moveVec);
    }
    else
    {
        // 経路探索
        if (aStar->goal == false)
        {
            aStarNow = true;
            animator->PlayAnim((int)AnimName_Survivor::Idle);
            aStar->Update();
        }
        // 経路が決まったら
        if (aStar->goal == true)
        {
            aStarNow = false;
            animator->PlayAnim((int)AnimName_Survivor::Run);
            // 目標座標との距離
            float dis = 0.0f;

            // 経路があれば目標座標を辿っていく
            if (aStar->finalPosVec.size() > 0)
            {
                // 全て辿り終わって到着しなかったら直接向かう
                if (aStar->nowRootNum < 0)
                {
                    moveVec = VScale(VNorm(VSub(targetCipherMachine->position, position)), moveSpeed * FrameTime);
                }
                else
                {
                    moveVec = VScale(VNorm(VSub(aStar->finalPosVec[aStar->nowRootNum], position)), moveSpeed * FrameTime);
                }

                position = VAdd(position, moveVec);

                if (aStar->nowRootNum < 0)
                {
                    dis = 0.0f;
                }
                else
                {
                    dis = VSize(VSub(position, aStar->finalPosVec[aStar->nowRootNum]));
                }
            }

            // 目標座標と近づいたら
            if (dis < Survivor_AStar_GoNextPoint_Dis)
            {
                //nowRootNumには目標座標の残りの合計数が入ってる
                // 次の座標へ 
                aStar->nowRootNum--;
            }
        }
    }
    
    // 目標の暗号機と接触したら解読に移る
    if (collisionMnagaer->CheckHit_Capsule_Capsule(this, targetCipherMachine) == true)
    {
        // ステートの変更
        state = State_Survivor::Decoding;
        // 解読中暗号機のセット
        decodingCipherMachine = targetCipherMachine;
        // 目標暗号機のリセット
        targetCipherMachine = nullptr;
        
        DecodingPos decoPos = decodingCipherMachine->GetDecodingPos();
        position = decoPos.pos;
        decodingCMIndex = decoPos.index;
        moveVec = VScale(VNorm(VSub(decodingCipherMachine->position, position)), moveSpeed * FrameTime);
    }

    // 座標変換
    Rotate();
    Transform(modelHandle);
}

// 解読
void Survivor::Decoding()
{
    // 解読
    decodingCipherMachine->Decoding(addDecodingRate);
    if (mode == Mode::Player)
    {
        DrawBox(1920 / 2 - 200, 600, 1920 / 2 + 200, 620, GetColor(0, 0, 0), true);
        DrawBox(1920 / 2 - 200, 600, 1920 / 2 - 200 + (int)((float)400 * (decodingCipherMachine->GetDecodingRate() / 100.0f)), 620, GetColor(200, 200, 0), true);
        SoundManager::GetInstance()->PlaySE(SE_Name::Decoding);
    }
    
    // 解読完了
    if (decodingCipherMachine->GetDecodingComplete() == true)
    {
        // 解読中の暗号機をリセット
        decodingCipherMachine = nullptr;

        // ステート変更
        state = State_Survivor::Idle;
    }
}

// 目標キーパッドを決める
void Survivor::DecideInputKeyPad()
{
    float minDis = -1.0f; // 一番近い距離
    int index = -1;       // ↑の暗号機の番号

    // 暗号機の数分
    for (int i = 0; i < Map_ExitNum; i++)
    {
        // 自分との距離を出す
        float dis = VSize(VSub(keyPadVec[i]->position, position));

        // 入力完了なら
        if (keyPadVec[i]->GetInputComplete() == true)
        {
            minDis = dis;
            index = i;
            break;
        }
        else
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

    if (index != -1)
    {
        targetKeyPad = keyPadVec[index];
        inputKeyPadIndex = index;
        // 経路探索初期化
        aStar->ResetPolyVec();
        VECTOR keyPadPos = targetKeyPad->position;
        keyPadPos.y = 0.0f;
        aStar->SetStartGoalPolyIndex(position, keyPadPos);
    }
}

// 目標キーパッドに向かう
void Survivor::GoTargetKeyPad()
{
    // 目標座標との間に壁が無ければ直接向かう

    VECTOR keyPadPos = targetKeyPad->position;
    keyPadPos.y = 0.0f;
    if (collisionMnagaer->GetWallHit(position, keyPadPos) == false)
    {
        animator->PlayAnim((int)AnimName_Survivor::Run);
        // キーパッドのYは0でないので０にしておく
        VECTOR targetPos = targetKeyPad->position;
        targetPos.y = 0.0f;
        moveVec = VScale(VNorm(VSub(targetPos, position)), moveSpeed * FrameTime);
        position = VAdd(position, moveVec);
    }
    else
    {
        // 経路探索
        if (aStar->goal == false)
        {
            aStarNow = true;
            animator->PlayAnim((int)AnimName_Survivor::Idle);
            aStar->Update();
        }
        // 経路が決まったら
        if (aStar->goal == true)
        {
            aStarNow = false;
            animator->PlayAnim((int)AnimName_Survivor::Run);
            // 目標座標との距離
            float dis = 0.0f;

            // 経路があれば目標座標を辿っていく
            if (aStar->finalPosVec.size() > 0)
            {
                // 全て辿り終わって到着しなかったら直接向かう
                if (aStar->nowRootNum < 0)
                {
                    // キーパッドのYは0でないので０にしておく
                    VECTOR targetPos = targetKeyPad->position;
                    targetPos.y = 0.0f;
                    moveVec = VScale(VNorm(VSub(targetPos, position)), moveSpeed * FrameTime);
                }
                else
                {
                    moveVec = VScale(VNorm(VSub(aStar->finalPosVec[aStar->nowRootNum], position)), moveSpeed * FrameTime);
                }

                position = VAdd(position, moveVec);

                if (aStar->nowRootNum < 0)
                {
                    dis = 0.0f;
                }
                else
                {
                    dis = VSize(VSub(position, aStar->finalPosVec[aStar->nowRootNum]));
                }
            }

            // 目標座標と近づいたら
            if (dis < Survivor_AStar_GoNextPoint_Dis)
            {
                //nowRootNumには目標座標の残りの合計数が入ってる
                // 次の座標へ 
                aStar->nowRootNum--;
            }
        }
    }
    
    // 目標のキーパッドと接触したらパスワード入力に移る
    if (collisionMnagaer->CheckHit_Capsule_Capsule(this, targetKeyPad) == true)
    {
        // ステート変更
        state = State_Survivor::InputPassward;
        // 入力中キーパッドのセット
        inputKeyPad = targetKeyPad;
        // 目標キーパッドのリセット
        targetKeyPad = nullptr;
    }

    // 座標変換
    Rotate();
    Transform(modelHandle);
}

// パスワード入力
void Survivor::InputPassward()
{
    if (mode == Mode::Player)
    {
        DrawBox(1920 / 2 - 200, 600, 1920 / 2 + 200, 620, GetColor(0, 0, 0), true);
        DrawBox(1920 / 2 - 200, 600, 1920 / 2 - 200 + (int)((float)400 * (inputKeyPad->GetInputRate() / 100.0f)), 620, GetColor(200, 200, 0), true);
        SoundManager::GetInstance()->PlaySE(SE_Name::Decoding);
    }

    // 入力
    inputKeyPad->InputPassward(addInputRate);

    // 入力完了
    if (inputKeyPad->GetInputComplete() == true )
    {
        if(mode == Mode::CPU)
        {
            // ステート変更
            state = State_Survivor::GoExit;

            // 経路探索初期化
            aStar->ResetPolyVec();
            aStar->SetStartGoalPolyIndex(position, posVec_Exit_AStar[inputKeyPadIndex]);
        }
        else
        {
            // ステート変更
            state = State_Survivor::Idle;
        }
             
        // 入力中のキーパッドリセット
        inputKeyPad = nullptr;   
    }
}

// 脱出
void Survivor::Escape()
{
    // 目標座標との間に壁が無ければ直接向かう
    if (collisionMnagaer->GetWallHit(position, posVec_Exit_AStar[inputKeyPadIndex]) == false)
    {
        animator->PlayAnim((int)AnimName_Survivor::Run);
        moveVec = VScale(VNorm(VSub(posVec_Exit_AStar[inputKeyPadIndex], position)), moveSpeed * FrameTime);
        position = VAdd(position, moveVec);
    }
    else
    {
        // 経路探索
        if (aStar->goal == false)
        {
            aStarNow = true;
            animator->PlayAnim((int)AnimName_Survivor::Idle);
            aStar->Update();
        }
        // 経路が決まったら
        if (aStar->goal == true)
        {
            aStarNow = false;
            animator->PlayAnim((int)AnimName_Survivor::Run);
            // 目標座標との距離
            float dis = 0.0f;

            // 経路があれば目標座標を辿っていく
            if (aStar->finalPosVec.size() > 0)
            {
                moveVec = VScale(VNorm(VSub(aStar->finalPosVec[aStar->nowRootNum], position)), moveSpeed * FrameTime);
                position = VAdd(position, moveVec);
                dis = VSize(VSub(position, aStar->finalPosVec[aStar->nowRootNum]));
            }

            // 目標座標と近づいたら
            if (dis < Survivor_AStar_GoNextPoint_Dis)
            {
                //nowRootNumには目標座標の残りの合計数が入ってる

                // 最後までたどり着いたらクリア
                if (aStar->nowRootNum <= 0)
                {
                    state = State_Survivor::GameClear;
                }
                // 次の座標へ
                else
                {
                    aStar->nowRootNum--;
                }
            }
        }
    }

    // 座標変換
    Rotate();
    Transform(modelHandle);
}

// 救助に行くかチェック
void Survivor::CheckGoRescue()
{
    // 他のサバイバーの数
    int otherSurvivorNum = (int)otherSurvivorVec.size();

    // 救助対象のサバイバー
    Survivor* survivor = nullptr;

    // 捕まってるサバイバーの中で一番近い距離
    float minDis = 0.0f;

    for (int i = 0; i < otherSurvivorNum; i++)
    {
        // 捕まってたら
        if (otherSurvivorVec[i]->GetState() == State_Survivor::Restrainting)
        {
            // 自分との距離を出す
            float dis = VSize(VSub(otherSurvivorVec[i]->position, position));

            // 救助対象がまだいなければ
            if (survivor == nullptr)
            {
                // 距離とサバイバーを保存
                survivor = otherSurvivorVec[i];
                minDis = dis;
            }
            // いた場合距離を比べる
            else
            {
                // もっと近ければ
                if (minDis > dis)
                {
                    // 距離とサバイバーを上書き
                    survivor = otherSurvivorVec[i];
                    minDis = dis;
                }
            }
        }
    }

    // 救助対象が居た場合
    if (survivor != nullptr)
    {
        // 救助対象に登録
        rescueSurvivor = survivor;

        // 解読中の場合、解読率が半分以上になったら救助に行く
        if (state == State_Survivor::Decoding)
        {
            if (decodingCipherMachine->GetDecodingRate() < 50.0f)
            {
                decodingCipherMachine->LeaveDecodeingPos(decodingCMIndex);
                // ステート変更
                state = State_Survivor::GoRescue;
                // 経路探索初期化               
                aStar->ResetPolyVec();
                aStar->SetStartGoalPolyIndex(position, rescueSurvivor->position);
            }
        }
        else
        {
            // ステート変更
            state = State_Survivor::GoRescue;
            // 経路探索初期化               
            aStar->ResetPolyVec();
            aStar->SetStartGoalPolyIndex(position, rescueSurvivor->position);
        }
    }
}

// 救助に向かう
void Survivor::GoRescue()
{
    // 目標座標との間に壁が無ければ直接向かう
    if (collisionMnagaer->GetWallHit(position, rescueSurvivor->position) == false)
    {
        animator->PlayAnim((int)AnimName_Survivor::Run);
        moveVec = VScale(VNorm(VSub(rescueSurvivor->position, position)), moveSpeed * FrameTime);
        position = VAdd(position, moveVec);
    }
    else
    {
        // 経路探索
        if (aStar->goal == false)
        {
            aStarNow = true;
            animator->PlayAnim((int)AnimName_Survivor::Idle);
            aStar->Update();
        }
        // 経路が決まったら
        if (aStar->goal == true)
        {
            aStarNow = false;
            animator->PlayAnim((int)AnimName_Survivor::Run);
            // 目標座標との距離
            float dis = 0.0f;

            // 経路があれば目標座標を辿っていく
            if (aStar->finalPosVec.size() > 0)
            {
                // 全て辿り終わって到着しなかったら直接向かう
                if (aStar->nowRootNum < 0)
                {
                    moveVec = VScale(VNorm(VSub(rescueSurvivor->position, position)), moveSpeed * FrameTime);
                }
                else
                {
                    moveVec = VScale(VNorm(VSub(aStar->finalPosVec[aStar->nowRootNum], position)), moveSpeed * FrameTime);
                }

                position = VAdd(position, moveVec);

                if (aStar->nowRootNum < 0)
                {
                    dis = 0.0f;
                }
                else
                {
                    dis = VSize(VSub(position, aStar->finalPosVec[aStar->nowRootNum]));
                }
            }

            // 目標座標と近づいたら
            if (dis < Survivor_AStar_GoNextPoint_Dis)
            {
                //nowRootNumには目標座標の残りの合計数が入ってる
                // 次の座標へ 
                aStar->nowRootNum--;
            }
        }
    }

    // 救助対象の捕まってるロケットチェアと接触したら救助に移る
    if (collisionMnagaer->CheckHit_Capsule_Capsule(this, rescueSurvivor->GetRocketChair()) == true)
    {
        // ステートの変更
        state = State_Survivor::Rescue;
    }

    // もし救助対象が捕まってる状態でなければアイドル状態へ
    if (rescueSurvivor->GetState() != State_Survivor::Restrainting)
    {
        rescueSurvivor = nullptr;
        state = State_Survivor::Idle;
    }

    // 座標変換
    Rotate();
    Transform(modelHandle);
}

// 救助
void Survivor::RescueSurvivor()
{
    // 救助対象がロケットチェアの情報を持ってれば
    if (rescueSurvivor->GetRocketChair()->GetSurvivor() != nullptr)
    {
        // 救助を進める
        rescueSurvivor->SetIsRescuing(true);
        rescueSurvivor->GetRocketChair()->AddReleseRate(Survivor_Default_AddRescueRate);
        if (mode == Mode::Player)
        {
            DrawBox(1920 / 2 - 200, 600, 1920 / 2 + 200, 620, GetColor(0, 0, 0), true);
            DrawBox(1920 / 2 - 200, 600, 1920 / 2 - 200 + (int)((float)400 * (rescueSurvivor->GetRocketChair()->GetReleaseRate() / 100.0f)), 620, GetColor(200, 200, 0), true);
        }
    }
    // 無ければアイドル状態に戻す
    else
    {
        // 救助対象をリセット
        rescueSurvivor->SetIsRescuing(false);
        rescueSurvivor = nullptr;

        // ステートの変更
        state = State_Survivor::Idle;
    }
}

// 逃げるかチェック
void Survivor::CheckRunAway()
{
    // ハンターとの距離を出す
    float disFromHunter = VSize(VSub(hunter->position, position));

    // 距離が一定以下だったら逃げる場所を決める
    if (disFromHunter <= Survivor_StartRunAway_Dis)
    {
        if (state == State_Survivor::Decoding && decodingCipherMachine != nullptr)
        {
            decodingCipherMachine->LeaveDecodeingPos(decodingCMIndex);
        }
        DecideRunAwayRoot();
    }
}

// 逃走場所決め
void Survivor::DecideRunAwayRoot()
{
    // 逃げる場所の数
    int runAwayPointNum = (int)runAwayPointVec.size();

    // ハンターの方向
    VECTOR hunterDir = VSub(hunter->position, position);

    // 一番小さい内積
    float minDot = 1.0f;

    // 逃げる場所の番号
    runAwayPointIndex = -1;

    for (int i = 0; i < runAwayPointNum; i++)
    {
        // 候補の場所との距離を出す 
        float dis = VSize(VSub(runAwayPointVec[i], position));

        // 一定以下、以上だったら確認しない（同じ場所に行かせないため、遠すぎると探索に時間がかかるため）
        if (dis > 3500.0f || dis < 300.0f)
        {
            continue;
        }

        // 候補の場所の方向を出す
        VECTOR pointDir = VSub(runAwayPointVec[i], position);

        // ハンターの方向との内積
        float dot = VDot(hunterDir, pointDir);

        // さらに小さいのが出たら
        if (minDot > dot)
        {
            // 更新
            minDot = dot;
            runAwayPointIndex = i;
        }


    }

    // 全てダメだったらランダムに向かう
    if (runAwayPointIndex == -1)
    {
        runAwayPointIndex = GetRand(runAwayPointNum - 1);
    }

    // ステートの変更
    state = State_Survivor::RunAway;

    // 経路探索の初期化
    aStar->ResetPolyVec();
    aStar->SetStartGoalPolyIndex(position, runAwayPointVec[runAwayPointIndex]);
}

// 逃げる
void Survivor::RunAway()
{
    // 目標座標との間に壁が無ければ直接向かう
    if (collisionMnagaer->GetWallHit(position, runAwayPointVec[runAwayPointIndex]) == false)
    {
        animator->PlayAnim((int)AnimName_Survivor::Run);
        moveVec = VScale(VNorm(VSub(runAwayPointVec[runAwayPointIndex], position)), moveSpeed * FrameTime);
        position = VAdd(position, moveVec);
    }
    else
    {
        // 経路探索
        if (aStar->goal == false)
        {
            aStarNow = true;
            animator->PlayAnim((int)AnimName_Survivor::Idle);
            aStar->Update();
        }
        // 経路が決まったら
        if (aStar->goal == true)
        {
            aStarNow = false;
            animator->PlayAnim((int)AnimName_Survivor::Run);
            float dis = 0.0f;

            // 経路があれば目標座標を辿っていく
            if (aStar->finalPosVec.size() > 0)
            {
                moveVec = VScale(VNorm(VSub(aStar->finalPosVec[aStar->nowRootNum], position)), moveSpeed * FrameTime);
                position = VAdd(position, moveVec);
                dis = VSize(VSub(position, aStar->finalPosVec[aStar->nowRootNum]));
            }

            // 目標座標と近づいたら
            if (dis < 50.0f)
            {
                //nowRootNumには目標座標の残りの合計数が入ってる

                // 最後までたどり着いたら
                if (aStar->nowRootNum == 0)
                {
                    // もう一度経路探索
                    DecideRunAwayRoot();
                }
                else
                {
                    // 次の座標へ
                    aStar->nowRootNum--;
                }
            }
        }
    }

    // 目標座標との距離
    float targetDis = VSize(VSub(runAwayPointVec[runAwayPointIndex], position));

    // 目標座標と近づいたら
    if (targetDis < 50.0f)
    {
        // もう一度経路探索
        DecideRunAwayRoot();
    }

    // ハンターとの距離を出す 
    float dis = VSize(VSub(hunter->position, position));

    // ハンターと一定以上離れたらアイドル状態に戻して次の行動を決めさせる
    if (dis > Survivor_EndRunAway_Dis)
    {
        state = State_Survivor::Idle;
    }

    // もし暗号機が必要数解読出来ていればアイドル状態から次の行動を決めさせる
    if (decodingComplete == true)
    {
        state = State_Survivor::Idle;
    }

    // 座標変換
    Rotate();
    Transform(modelHandle);
}

// クリアチェック
void Survivor::CheckGameClear()
{
    if (position.z < -1100.0f || position.z > 4550.0f)
    {
        if (state != State_Survivor::LiftedUp)
        {
            state = State_Survivor::GameClear;
        }        
    }
}

// 行動対象のポインタのリセット
void Survivor::ResetActionTargets()
{
    // 解読中の暗号機
    if (state != State_Survivor::Decoding)
    {
        decodingCipherMachine = nullptr;
    }

    // 入力中のキーパッド
    if (state != State_Survivor::InputPassward)
    {
        inputKeyPad = nullptr;
    }

    // 救助対象のサバイバー
    if (state != State_Survivor::Rescue && state != State_Survivor::GoRescue)
    {
        if (rescueSurvivor != nullptr)
        {
            rescueSurvivor->SetIsRescuing(false);
            rescueSurvivor = nullptr;
        }
    } 
}

// 飛ばされる時の処理
void Survivor::Flying()
{
    // ロケットチェアとサバイバーを飛ばす
    position.y += Survivor_FlyingSpeed;
    Transform(modelHandle);
    rocketChair->position.y += Survivor_FlyingSpeed;
    rocketChair->Transform(rocketChair->GetModelHandle());
    rocketChair->UpdateCollider();

    // 一定以上の高度になったらゲームオーバー
    if (position.y > Survivor_Max_FlyingHeight)
    {
        state = State_Survivor::GameOver;
    }
}

// 捕まった時の処理
void Survivor::AddRestraintCnt()
{
    restraintCnt++;
    // 一回目は何もなし

    // 二回目は
    if (restraintCnt == 2)
    {
        // ゲームオーバーゲージが半分以下なら半分に
        if (gameOverRate <= 50.0f)
        {
            gameOverRate = 50.0f;
        }
        // 半分以上なら飛ばす
        else if (gameOverRate > 50.0f)
        {
            gameOverRate = 100.0f;
            state = State_Survivor::Flying;
        }
    }

    // 三回目なら飛ばす
    if (restraintCnt == 3)
    {
        gameOverRate = 100.0f;
        state = State_Survivor::Flying;
    }
}

// 捕まってる時の処理
void Survivor::Restrainting()
{
    rotation = rocketChair->rotation;
    position.y = 50.0f;
    Transform(modelHandle);

    int gameEndSurvivorNum = 0;
    for (int i = 0; i < 3; i++)
    {
        if (otherSurvivorVec[i]->GetState() == State_Survivor::GameOver || otherSurvivorVec[i]->GetState() == State_Survivor::GameClear || otherSurvivorVec[i]->GetState() == State_Survivor::Restrainting)
        {
            gameEndSurvivorNum++;
        }
    }
    if (gameEndSurvivorNum == 3)
    {
        gameOverRate = 100.0f;
    }
    if (state == State_Survivor::Restrainting && !isRescuing)
    {
        gameOverRate += Survivor_Default_AddGameOverRate;
        if (gameOverRate >= 100.0f)
        {
            gameOverRate = 100.0f;
            state = State_Survivor::Flying;
        }
    }
}

// ステートに応じてスピード変更
void Survivor::ChangeSpeed()
{
    if (state == State_Survivor::Fall)
    {
        moveSpeed = Survivor_FallMoveSpped;
    }
    else
    {
        moveSpeed = Survivor_Default_MoveSpeed;
    }
}

// AIのUpdate
void Survivor::AI()
{
    switch (state)
    {
    case State_Survivor::None:
        break;
    case State_Survivor::Idle: // アイドル中
        animator->PlayAnim((int)AnimName_Survivor::Idle);
        Idle();
        if (decodingComplete == false)
        {
            // 逃げるかチェック
            CheckRunAway();
            // 救助に行くかチェック
            CheckGoRescue();
        }
        // 攻撃を受ける
        isVulnerable = true;
        break;
    case State_Survivor::GoCipherMachine: // 暗号機に向かってる
        GoTargetCipherMachine();
        // 逃げるかチェック
        CheckRunAway();
        // 救助に行くかチェック
        CheckGoRescue();
        // 攻撃を受ける
        isVulnerable = true;
        break;
    case State_Survivor::Decoding: // 解読中
        animator->PlayAnim((int)AnimName_Survivor::Decoding);
        Decoding();
        // 逃げるかチェック
        CheckRunAway();
        // 救助に行くかチェック
        CheckGoRescue();
        // 攻撃を受ける
        isVulnerable = true;
        break;
    case State_Survivor::GoKeyPad: // キーパッドに向かってる
        GoTargetKeyPad();
        // 攻撃を受ける
        isVulnerable = true;
        break;
    case State_Survivor::InputPassward: // パスワード入力中
        animator->PlayAnim((int)AnimName_Survivor::Input);
        InputPassward();
        // 攻撃を受ける
        isVulnerable = true;
        break;
    case State_Survivor::GoExit: // 出口に向かってる
        Escape();
        // 攻撃を受ける
        isVulnerable = true;
        break;
    case State_Survivor::RunAway: // 逃走中
        RunAway();
        // 攻撃を受ける
        isVulnerable = true;
        break;
    case State_Survivor::GoRescue: // 救助に向かってる
        GoRescue();
        // 逃げるかチェック
        CheckRunAway();
        // 攻撃を受ける
        isVulnerable = true;
        break;
    case State_Survivor::Rescue: // 救助中
        animator->PlayAnim((int)AnimName_Survivor::Rescue);
        RescueSurvivor();
        // 攻撃を受ける
        isVulnerable = true;
        break;
    case State_Survivor::Fall: // ダウン中
        animator->PlayAnim((int)AnimName_Survivor::FallIdle);
        // 攻撃を受ける
        isVulnerable = false;
        break;
    case State_Survivor::LiftedUp: // 担がれ中
        animator->PlayAnim((int)AnimName_Survivor::FallIdle);
        break;
    case State_Survivor::Restrainting: // 拘束中
        animator->PlayAnim((int)AnimName_Survivor::Restrainting);
        Restrainting();
        break;
    case State_Survivor::Flying: // 飛ばされ中
        animator->PlayAnim((int)AnimName_Survivor::Restrainting);
        Flying();
        // 攻撃を受けない
        isVulnerable = false;
        break;
    case State_Survivor::GameOver: // ゲームオーバー
        animator->PlayAnim((int)AnimName_Survivor::Restrainting);
        // 攻撃を受けない
        isVulnerable = false;
        break;
    case State_Survivor::GameClear: // ゲームクリア
        animator->PlayAnim((int)AnimName_Survivor::Run);
        // 攻撃を受けない
        isVulnerable = false;
        break;
    default:
        break;
    }

    // 行動対象のポインタのリセット
    ResetActionTargets();

    // スピード変更
    ChangeSpeed();

    // クリアチェック
    CheckGameClear();
}

// PlayerのUpdate
void Survivor::Player()
{
    switch (state)
    {
    case State_Survivor::None:
        break;
    case State_Survivor::Idle: // アイドル中
        animator->PlayAnim((int)AnimName_Survivor::Idle);
        // 移動するかチェック
        CheckMove();
        // 行動可能
        actionDisabled = false;
        // 攻撃を受ける
        isVulnerable = true;
        break;
    case State_Survivor::Move: // 移動中
        animator->PlayAnim((int)AnimName_Survivor::Run);
        // 移動
        Move();
        // 停止するかチェック
        CheckIdle();
        // 行動可能
        actionDisabled = false;
        // 攻撃を受ける
        isVulnerable = true;
        break;
    case State_Survivor::Decoding: // 解読中
        animator->PlayAnim((int)AnimName_Survivor::Decoding);
        // 解読
        Decoding();
        // 移動するかチェック
        CheckMove();
        // 行動可能
        actionDisabled = false;
        // 攻撃を受ける
        isVulnerable = true;
        break;
    case State_Survivor::InputPassward: // パスワード入力中
        animator->PlayAnim((int)AnimName_Survivor::Input);
        // 入力
        InputPassward();
        // 移動するかチェック
        CheckMove();
        // 行動可能
        actionDisabled = false;
        // 攻撃を受ける
        isVulnerable = true;
        break;
    case State_Survivor::Rescue: // 救助中
        animator->PlayAnim((int)AnimName_Survivor::Rescue);
        // 救助
        RescueSurvivor();
        // 移動するかチェック
        CheckMove();
        // 行動可能
        actionDisabled = false;
        // 攻撃を受ける
        isVulnerable = true;
        break;
    case State_Survivor::Fall: // ダウン中        
        // 移動
        Move();
        // 行動不能
        actionDisabled = true;
        if (VSize(moveVec) != 0.0f)
        {
            animator->PlayAnim((int)AnimName_Survivor::FallWalk);
        }
        else
        {
            animator->PlayAnim((int)AnimName_Survivor::FallIdle);
        }
        // 攻撃を受けない
        isVulnerable = false;
        break;
    case State_Survivor::LiftedUp: // 担がれ中
        animator->PlayAnim((int)AnimName_Survivor::FallIdle);
        // 行動不能
        actionDisabled = true;
        // 攻撃を受けない
        isVulnerable = false;
        break;
    case State_Survivor::Restrainting: // 拘束中
        animator->PlayAnim((int)AnimName_Survivor::Restrainting);
        Restrainting();
        // 行動不能
        actionDisabled = true;
        break;
    case State_Survivor::Flying: // 飛ばされ中
        animator->PlayAnim((int)AnimName_Survivor::Restrainting);
        Flying();
        // 行動不能
        actionDisabled = true;
        // 攻撃を受けない
        isVulnerable = false;
        break;
    case State_Survivor::GameOver: // ゲームオーバー
        animator->PlayAnim((int)AnimName_Survivor::Restrainting);
        // 行動不能
        actionDisabled = true;
        // 攻撃を受けない
        isVulnerable = false;
        break;
    case State_Survivor::GameClear: // ゲームクリア
        animator->PlayAnim((int)AnimName_Survivor::Run);
        // 行動不能
        actionDisabled = true;
        // 攻撃を受けない
        isVulnerable = false;
        break;
    default:
        break;
    }

    // 行動対象のポインタのリセット
    ResetActionTargets();

    // スピード変更
    ChangeSpeed();

    // クリアチェック
    CheckGameClear();

    // 音系
    if (state != State_Survivor::Decoding && state != State_Survivor::InputPassward)
    {
        SoundManager::GetInstance()->StopSE(SE_Name::Decoding);
    }

    float dis = VSize(VSub(position, hunter->position));
    if(dis < Hunter_StartChaseSurvivorDis && hunter->GetTarget() == (Survivor*)this && actionDisabled == false)
    {
        SoundManager::GetInstance()->PlayBGM(BGM_Name::Approach);
        int vol = (int)((float)255 * (1.0f - dis / Hunter_StartChaseSurvivorDis));
        SoundManager::GetInstance()->ChangeVolume_BGM(BGM_Name::Approach, vol);

        SoundManager::GetInstance()->PlaySE(SE_Name::HeartBeat);
        SoundManager::GetInstance()->ChangeVolume_SE(SE_Name::HeartBeat, vol);
    }
    else
    {
        SoundManager::GetInstance()->StopBGM(BGM_Name::Approach);
        SoundManager::GetInstance()->StopSE(SE_Name::HeartBeat);
    }
}

// キーボード操作とXbox操作合わせた関数
void Survivor::Move()
{
    // XBoxコントローラーが繋がってたらXBox操作
    if (GetJoypadNum() > 0)
    {
        Move_XBox();
    }
    // 繋がってない場合キーボード操作（今は使えない）
    else
    {
        Move_Key();
    }
}

// キーボード操作（今は使えない）
void Survivor::Move_Key()
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
    Rotate();

    // 座標変換
    Transform(modelHandle);
}

// Xbox操作
void Survivor::Move_XBox()
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

// 移動するかチェック
void Survivor::CheckMove()
{
    // スティック入力があれば
    if (Input::GetInstance()->GetStickInput(true, false) == true)
    {
        if (state == State_Survivor::Rescue)
        {
            rescueSurvivor->GetRocketChair()->AddReleseRate(-100.0f);
        }
        if (state == State_Survivor::Decoding && decodingCipherMachine != nullptr)
        {
            decodingCipherMachine->LeaveDecodeingPos(decodingCMIndex);
        }
        // ステートの変更
        state = State_Survivor::Move;
    }
}

// 停止するかチェック
void Survivor::CheckIdle()
{
    // スティック入力が無ければ
    if (Input::GetInstance()->GetStickInput(true, false) == false)
    {
        // ステートの変更
        state = State_Survivor::Idle;
    }
}

// デバッグ表示
void Survivor::Debug()
{
    Character::Debug();
    DrawFormatString(600, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "HP : %d", hp);
    Debug_DispCnt++;
    DrawFormatString(600, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "GameOverRate : %.1f", gameOverRate);
    Debug_DispCnt++;
    DrawFormatString(600, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "State : %d", (int)state);
    Debug_DispCnt++;
    DrawFormatString(600, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "CMPosIndex : %d", decodingCMIndex);
    Debug_DispCnt++;
}

void Survivor::Reset()
{
    state = State_Survivor::Idle;
    ResetActionTargets();
    // 経路探索の初期化
    if (mode == Mode::CPU)
    {
        aStar->ResetPolyVec();
    }
}

// 当たり判定
void Survivor::OnCollisionHit(Collider* Collider, GameObject* Parent)
{
    // 暗号機と触れている場合
	if (Parent->tag == "CipherMachine")
	{
        // 行動できれば
		if (actionDisabled == false)
		{
            // Bボタンが押されるかつモードがプレイヤーだったら
			if (Input::GetInstance()->GetButtonDown(ButtonName::Button_B) && mode == Mode::Player)
			{
                // すでに解読中であれば
                if (state == State_Survivor::Decoding)
                {
                    decodingCipherMachine->LeaveDecodeingPos(decodingCMIndex);
                    // 解読中の暗号機リセット
                    decodingCipherMachine = nullptr;

                    // ステートの変更
                    state = State_Survivor::Idle;
                }
                // 解読中でなければ
                else
                {
                    // 解読中の暗号機に登録
                    decodingCipherMachine = (CipherMachine*)Parent;

                    // 解読が終わってなければ
                    if (decodingCipherMachine->GetDecodingComplete() == false)
                    {
                        // ステートの変更
                        state = State_Survivor::Decoding;
                        
                        DecodingPos decoPos = decodingCipherMachine->GetDecodingPos();
                        position = decoPos.pos;
                        decodingCMIndex = decoPos.index;
                        moveVec = VScale(VNorm(VSub(decodingCipherMachine->position, position)), moveSpeed * FrameTime);
                        // 座標変換
                        Rotate();
                        Transform(modelHandle);
                    }                    
                }	
			}

            CipherMachine* cipherMachine = (CipherMachine*)Parent;
            if (mode == Mode::Player && cipherMachine->GetDecodingComplete() == false && state != State_Survivor::Decoding)
            {
                VECTOR drawPos = cipherMachine->position;
                drawPos.y += 250.0f;
                DrawBillboard3D(drawPos, 0.5f, 0.5f, 60.0f, 0.0f, GameManager::GetInstance()->GetImageHandle(UI_Name::Button_B), true);
            }
		}
	}

    // キーパッドと触れている場合
    if (Parent->tag == "Keypad")
    {
        // 行動出来れば
        if (actionDisabled == false)
        {
            // Bボタンが押されるかつモードがプレイヤーだったら
            if (Input::GetInstance()->GetButtonDown(ButtonName::Button_B) && mode == Mode::Player)
            {
                // すでに入力中なら
                if (state == State_Survivor::InputPassward)
                {
                    // 入力中のキーパッドをリセット
                    inputKeyPad = nullptr;

                    // ステートの変更
                    state = State_Survivor::Idle;
                }
                // 入力中でなければ
                else
                {
                    // 入力中のキーパッドに登録
                    inputKeyPad = (Keypad*)Parent;

                    // ステートの変更
                    state = State_Survivor::InputPassward;
                }
                
            }

            Keypad* keyPad = (Keypad*)Parent;
            if (mode == Mode::Player && keyPad->GetInputComplete() == false && state != State_Survivor::InputPassward)
            {
                VECTOR drawPos = keyPad->position;
                drawPos.y += 250.0f;
                DrawBillboard3D(drawPos, 0.5f, 0.5f, 60.0f, 0.0f, GameManager::GetInstance()->GetImageHandle(UI_Name::Button_B), true);
            }
        }      
    }

    // ハンターの攻撃が当たった場合
    if (Collider->colliderName == "HunterAtk")
    {       
        // 捕まってるときは空振りさせる
        if (state == State_Survivor::Restrainting)
        {
            Collider->isCheck = false;
            Hunter* hunter = (Hunter*)Parent;
            hunter->SetState(State_Hunter::AtkHit);
            return;
        }

        // 攻撃を受ける状態だったら
        if (isVulnerable == true)
        {
            // 解読、入力、救助中は一撃でやられる
            if (state == State_Survivor::Decoding || 
                state == State_Survivor::Rescue || 
                state == State_Survivor::InputPassward)
            {
                if (mode == Mode::Player)
                {
                    SoundManager::GetInstance()->PlaySE(SE_Name::AttackHit);
                }
                hp -= 4;
                if (state == State_Survivor::Rescue)
                {
                    rescueSurvivor->GetRocketChair()->AddReleseRate(-100.0f);
                }
                if (state == State_Survivor::Decoding && decodingCipherMachine != nullptr)
                {
                    decodingCipherMachine->LeaveDecodeingPos(decodingCMIndex);
                }
            }
            // それ以外は半分のダメージを受ける
            else
            {
                if (mode == Mode::Player)
                {
                    SoundManager::GetInstance()->PlaySE(SE_Name::AttackHit);
                }
                hp -= 2;
                if(hp != 0)
                {
                    animator->PlayAnim((int)AnimName_Survivor::AttackHit);
                }                
            }

            // hpが０になったら
            if (hp <= 0)
            {
                // ダウン状態にする
                state = State_Survivor::Fall;

                hp = 0;
            }

            // ハンターを攻撃をヒット状態にする
            Collider->isCheck = false;
            Hunter* hunter = (Hunter*)Parent;
            hunter->SetState(State_Hunter::AtkHit);
            if (hunter->mode == Mode::Player)
            {
                SoundManager::GetInstance()->PlaySE(SE_Name::AttackHit);
            }
        }
    }

    // ロケットチェアに触れている場合
    if (Parent->tag == "RocketChair")
    {
        // 触れたロケットチェアを取得
        RocketChair* rocketChair = (RocketChair*)Parent;

        // ロケットチェアにサバイバーが捕まってる場合
        if (rocketChair->GetSurvivor() != nullptr)
        {
            // 行動できれば
            if (actionDisabled == false)
            {
                // Bボタンが押されるかつモードがプレイヤーだったら
                if (Input::GetInstance()->GetButtonDown(ButtonName::Button_B) && mode == Mode::Player)
                {
                    // すでに救助中なら
                    if (state == State_Survivor::Rescue)
                    {
                        // 救助対象をリセット
                        rescueSurvivor->GetRocketChair()->AddReleseRate(-100.0f);
                        rescueSurvivor->SetIsRescuing(false);
                        rescueSurvivor = nullptr;

                        // ステートの変更
                        state = State_Survivor::Idle;
                    }
                    // 救助中でなければ
                    else
                    {
                        // 救助対象に登録
                        rescueSurvivor = rocketChair->GetSurvivor();
                        rescueSurvivor->SetIsRescuing(true);
                        // ステートの変更
                        state = State_Survivor::Rescue;
                    }
                }

                if (mode == Mode::Player && rocketChair->GetReleaseRate() == false && state != State_Survivor::Rescue)
                {
                    VECTOR drawPos = rocketChair->position;
                    drawPos.y += 250.0f;
                    DrawBillboard3D(drawPos, 0.5f, 0.5f, 60.0f, 0.0f, GameManager::GetInstance()->GetImageHandle(UI_Name::Button_B), true);
                }
            }
        }
    }
}