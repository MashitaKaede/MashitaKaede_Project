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

    // ���W�ϊ�
	position = VGet(3200.0f, 0.0f, 2200.0f);
    scale = VGet(2.0f, 2.0f, 2.0f);
	Transform(modelHandle);

    // �J�v�Z���R���C�_�[�̏�����
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

    // �o�H�T���p�N���X�쐬
	aStar = new AStar();

    // �X�e�[�^�X������
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
    // �o�H�T���p�|���S���Z�b�g
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
    // �^�[�Q�b�g���������Ă��Ȃ����
    if (target == nullptr)
    {
        state = State_Hunter::SearchTarget;
    }
    // ������Βǂ�������
    else
    {
        state = State_Hunter::ChaseTarget;
    }
}

void Hunter::DecideNextCipherMachine()
{
    float minDis = -1.0f; // ��ԋ߂�����
    int index = -1;       // ���̈Í��@�̔ԍ�
    int checkCM_Num = (int)check_CM_IndexVec.size(); // �`�F�b�N�ς݈Í��@�̐�

    // �S���m�F�ς݂������烊�Z�b�g
    if (checkCM_Num == Map_CipherMachineNum)
    {
        // �Ō�Ɋm�F�����Í��@�͊m�F�ς݂ɂ��Ă���
        int lastCMIndex = check_CM_IndexVec.back();
        ResetNextCipherMachine();
        check_CM_IndexVec.push_back(lastCMIndex);
        checkCM_Num = 1;
    }

    // �Í��@�̐���
    for (int i = 0; i < Map_CipherMachineNum; i++)
    {
        // �����Ƃ̋������o��
        float dis = VSize(VSub(cipherMachineVec[i]->position, position));

        // �m�F�ς݃t���O
        bool checked = false;

        // �`�F�b�N�ς݈Í��@�������
        if (checkCM_Num != 0)
        {
            // �`�F�b�N�ς݈Í��@�̐���
            for (int j = 0; j < checkCM_Num; j++)
            {
                // �m�F�ς݂�������
                if (i == check_CM_IndexVec[j])
                {
                    // �m�F�ς݃t���O�𗧂Ă�
                    checked = true;
                }
            }
        }

        // �m�F�ς݂łȂ����
        if (checked == false)
        {
            // �ŏ���������ۑ�
            if (minDis < 0)
            {
                minDis = dis;
                index = i;
            }
            else
            {
                // ����ɋ߂��̂��o�Ă����炻������ۑ�
                if (minDis > dis)
                {
                    minDis = dis;
                    index = i;
                }
            }
        }
    }

    // �m�F�ς݈Í��@�̔ԍ��ۑ�,�ڕW�Í��@�̐ݒ�,�Í��@����J�n�t���O�𗧂Ă�
    check_CM_IndexVec.push_back(index);
    targetCM = cipherMachineVec[index];
    goCM = true;
    targetCMIndex = index;
    // �o�H�T��������
    aStar->goal = false;
    aStar->ResetPolyVec();
    aStar->SetStartGoalPolyIndex(position, targetCM->position);
}

void Hunter::Search_Target()
{
    // �ڕW�Í��@�����܂�����
    if (goCM)
    {
        // �o�H�T��
        if (aStar->goal == false)
        {
            aStarNow = true;
            animator->PlayAnim((int)AnimName_Hunter::Idle);
            aStar->Update();
        }
        // �o�H�����܂�����
        if (aStar->goal == true)
        {
            aStarNow = false;
            animator->PlayAnim((int)AnimName_Hunter::Walk);
            float dis = 0.0f;

            // �o�H������ΖڕW���W��H���Ă���
            if (aStar->finalPosVec.size() > 0)
            {
                VECTOR oldPos = position;
                moveVec = VScale(VNorm(VSub(aStar->finalPosVec[aStar->nowRootNum], position)), moveSpeed * FrameTime);
                position = VAdd(position, moveVec);
                dis = VSize(VSub(position, aStar->finalPosVec[aStar->nowRootNum]));
            }

            // �ڕW���W�Ƌ߂Â�����
            if (dis < 100.0f)
            {
                //nowRootNum�ɂ͖ڕW���W�̎c��̍��v���������Ă�

                // �Ō�܂ł��ǂ蒅������
                if (aStar->nowRootNum == 0)
                {
                    // ���̈Í��@�����߂�
                    DecideNextCipherMachine();
                }

                // ���̍��W��
                else
                {
                    aStar->nowRootNum--;
                }
            }
        }

        Rotate();
        Transform(modelHandle);
    }
    // �������̈Í��@�����܂��ĂȂ����
    else
    {
        animator->PlayAnim((int)AnimName_Hunter::Idle);
        // ���̈Í��@�����߂�
        DecideNextCipherMachine();
    }

    // �S�T�o�C�o�[�Ƃ̋������m�F,���������ȉ����߂܂��ĂȂ���΃^�[�Q�b�g�ɂ���
    for (int i = 0; i < survivorNum; i++)
    {
        float dis = VSize(VSub(survivorVec[i]->position, position));
        if (dis <= Hunter_StartChaseSurvivorDis && survivorVec[i]->GetState() != State_Survivor::Restrainting && survivorVec[i]->GetState() != State_Survivor::Flying && survivorVec[i]->GetState() != State_Survivor::GameOver && survivorVec[i]->GetState() != State_Survivor::GameClear)
        {
            // �󂢂Ă郍�P�b�g�`�F�A���������_�E����Ԃ�������^�[�Q�b�g�ɂ��Ȃ�
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

    // �T�o�C�o�[�Ƃ̊Ԃɕǂ����邩�m�F�i��������o�H�T���A������Β��ڒǂ�������j
    VECTOR lineStartPos = position;
    lineStartPos.y += 100.0f;
    VECTOR lineEndPos = target->position;
    lineEndPos.y += 100.0f;

    // �ǂ��邩�m�F
    if (collisionMnagaer->GetWallHit(lineStartPos, lineEndPos) == true)
    {
        chaseDirect = false;
    }
    else
    {
        chaseDirect = true;
    }

    // �ǂ��������
    if (chaseDirect == true)
    {
        float dis = VSize(VSub(target->position, position));

        // ��苗���܂ŋ߂Â�
        if (dis > Hunter_MinDis_Survivor)
        {
            animator->PlayAnim((int)AnimName_Hunter::Walk);
            moveVec = VScale(VNorm(VSub(target->position, position)), 10.0f);
            position = VAdd(position, moveVec);
        }
        // ��苗���ȓ��Ȃ�
        else
        {
            // �^�[�Q�b�g���_�E����ԂȂ玝���グ��
            if (target->GetState() == State_Survivor::Fall)
            {
                animator->PlayAnim((int)AnimName_Hunter::Idle);
                // �����������グ���A�^�[�Q�b�g��S���ꒆ�ɂ���
                state = State_Hunter::PickUpTarget;
                target->SetState(State_Survivor::LiftedUp);
            }
        }

        // �T�������o�H�����Z�b�g��������
        aStar->goal = false;
        aStar->ResetPolyVec();
        aStar->SetStartGoalPolyIndex(position, target->position);
    }

    // �ǂ��L���ɂ�����炸�T���͐i�߂�
    if (aStar->goal == false)
    {
        aStarNow = true;
        aStar->Update();
        if (chaseDirect == false)
        {
            animator->PlayAnim((int)AnimName_Hunter::Idle);
        }
    }

    // �ǂ����違�o�H�T�����I�����Ă���Βǂ�������
    if (aStar->goal == true && chaseDirect == false)
    {
        aStarNow = false;
        animator->PlayAnim((int)AnimName_Hunter::Walk);
        float dis = 0.0f;

        // �o�H������ΖڕW���W��H���Ă���
        if (aStar->finalPosVec.size() > 0)
        {
            VECTOR oldPos = position;
            moveVec = VScale(VNorm(VSub(aStar->finalPosVec[aStar->nowRootNum], position)), 10.0f);
            position = VAdd(position, moveVec);
            dis = VSize(VSub(position, aStar->finalPosVec[aStar->nowRootNum]));
        }

        // �ڕW���W�Ƌ߂Â�����
        if (dis < 100.0f)
        {
            //nowRootNum�ɂ͖ڕW���W�̎c��̍��v���������Ă�

            // �Ō�܂ł��ǂ蒅������
            if (aStar->nowRootNum == 0)
            {
                // �T���o�H�����Z�b�g
                aStar->goal = false;
                aStar->ResetPolyVec();
                aStar->SetStartGoalPolyIndex(position, target->position);
            }
            // ���̍��W��
            else
            {
                aStar->nowRootNum--;
            }
        }
    }

    // �L�����̉�]�����W�ϊ�
    Rotate();
    Transform(modelHandle);

    // �n���^�[�ƃT�o�C�o�[�̋����𑪂�
    float dis = VSize(VSub(target->position, position));

    // ���������ȏ㗣�ꂽ�猩����
    if (dis >= Hunter_MissSurvivorDis)
    {
        // �Í��@����J�n
        state = State_Hunter::SearchTarget;
        target = nullptr;
        ResetNextCipherMachine();
        DecideNextCipherMachine();
    }
    else
    {
        if (target->GetState() != State_Survivor::Fall)
        {
            // ���������ȉ��ōU��
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
        // �ڕW���P�b�g�`�F�A�����܂�����
        if (DecideRocketChair(true) == true)
        {
            // �^�[�Q�b�g�^�я�Ԃ�
            state = State_Hunter::CarryTarget;

            // �^�[�Q�b�g�̍��W�ϊ�
            VECTOR newTargetPos = position;
            newTargetPos.y += 120.0f;
            newTargetPos = VAdd(newTargetPos, VScale(VScale(right, -1.0f), 80.0f));
            target->position = newTargetPos;
            target->rotation = rotation;
            target->Transform(target->GetModelHandle());
        }
        // �����g���郍�P�b�g�`�F�A���������
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
    
    // �S�T�o�C�o�[�Ƃ̋������m�F,���������ȉ����߂܂��ĂȂ���΃^�[�Q�b�g�ɂ���
    for (int i = 0; i < survivorNum; i++)
    {
        float dis = VSize(VSub(survivorVec[i]->position, position));
        if (dis < nowDis && survivorVec[i]->GetState() != State_Survivor::Restrainting && survivorVec[i]->GetState() != State_Survivor::Fall && survivorVec[i]->GetState() != State_Survivor::Flying && survivorVec[i]->GetState() != State_Survivor::GameOver && survivorVec[i]->GetState() != State_Survivor::GameClear)
        {
            // �󂢂Ă郍�P�b�g�`�F�A���������_�E����Ԃ�������^�[�Q�b�g�ɂ��Ȃ�
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
    // XBox�R���g���[���[���q�����Ă���XBox����
    if (GetJoypadNum() > 0)
    {
        Move_XBox();
    }
    // �q�����ĂȂ��ꍇ�L�[�{�[�h����
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
    // �X�e�B�b�N���͂������
    if (Input::GetInstance()->GetStickInput(true, false) == true)
    {
        // �X�e�[�g�̕ύX
        state = State_Hunter::Move;
    }
}

void Hunter::Move_Key()
{
    moveVec = VGet(0.0f, 0.0f, 0.0f);

    // �ړ����� (�J�����̌������l��)
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

    // �ǂƂ̓����蔻��p(���̎n�_)
    VECTOR startPos = position;
    startPos.y += Collision_Line_AddHeight;

    // �ړ���̍��W����
    position = VAdd(position, VScale(moveVec, moveSpeed * FrameTime));

    // �ǂƂ̓����蔻��p(���̏I�_)
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

    // �L�����N�^�[�̉�]����
    if (VSize(moveVec) > 0.0f)
    {
        rotation.y = atan2f(moveVec.x, moveVec.z) + DX_PI_F;
    }

    // �p�x�� -180�� �` 180�� �ɐ��K��
    if (rotation.y > DX_PI_F)
    {
        rotation.y -= DX_PI_F * 2;
    }
    if (rotation.y < -DX_PI_F)
    {
        rotation.y += DX_PI_F * 2;
    }

    // ���W�ϊ�
    Transform(modelHandle);
}

void Hunter::Move_XBox()
{
    // �ړ��x�N�g���̏�����
    moveVec = VGet(0.0f, 0.0f, 0.0f);

    // �X�e�B�b�N���͂������
    if (Input::GetInstance()->GetStickInput(true, false))
    {
        // �ړ��x�N�g���̃Z�b�g�i�X�e�B�b�N�p�x�{�J�����p�x�{�����p�p�x�j
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

    // �ړ��x�N�g��*���x*�t���[���^�C��
    moveVec = VScale(moveVec, moveSpeed * FrameTime);


    // �ǂƂ̓����蔻��p(���̎n�_)
    VECTOR oldPos = position;             // �ړ��O�̏ꏊ
    oldPos.y += Collision_Line_AddHeight; // �����グ��

    // �ړ���̍��W����
    position = VAdd(position, moveVec);

    // �ǂƂ̓����蔻��p(���̏I�_)
    VECTOR nextPos = position;             // �ړ���̏ꏊ
    nextPos.y += Collision_Line_AddHeight; // �����グ��

    // �ړ��O�̈ړ��x�N�g���\��
    //DrawLine3D(oldPos, nextPos, GetColor(255, 0, 0));

    // �ǂ���x�N�g���擾�i������΂��̂܂�moveVec���A���Ă���j
    slideVec = collisionMnagaer->GetSlideVec(oldPos, nextPos, moveVec);

    // �ǂ���x�N�g���K�p��̈ړ��x�N�g���\��
    //DrawLine3D(nextPos, VAdd(nextPos, slideVec), GetColor(255, 0, 0));

    // �ړ��x�N�g���ƕǂ���x�N�g���������łȂ���Έړ��O�̏ꏊ�ɕǂ���x�N�g���𑫂�
    if (VSize(slideVec) != VSize(moveVec))
    {
        oldPos.y = 0.0f;
        position = VAdd(oldPos, slideVec);
    }

    // �L�����N�^�[�̉�]����
    Rotate();

    // ���W�ϊ�
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
    // �X�e�B�b�N���͂������
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
    // �J�v�Z���̎n�_�ƏI�_�X�V
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

    // �o�H�T��
    if (aStar->goal == false)
    {
        aStarNow = true;
        animator->PlayAnim((int)AnimName_Hunter::Idle);
        aStar->Update();
    }
    // �o�H�����܂�����
    if (aStar->goal == true)
    {
        aStarNow = false;
        animator->PlayAnim((int)AnimName_Hunter::CarryWalk);
        float dis = 0.0f;

        // �o�H������ΖڕW���W��H���Ă���
        if (aStar->finalPosVec.size() > 0)
        {
            VECTOR oldPos = position;
            moveVec = VScale(VNorm(VSub(aStar->finalPosVec[aStar->nowRootNum], position)), moveSpeed * FrameTime);
            position = VAdd(position, moveVec);
            dis = VSize(VSub(position, aStar->finalPosVec[aStar->nowRootNum]));
        }

        // �ڕW���W�Ƌ߂Â�����
        if (dis < 100.0f)
        {
            //nowRootNum�ɂ͖ڕW���W�̎c��̍��v���������Ă�

            // �Ō�܂ł��ǂ蒅������
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

            // ���̍��W��
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
    float minDis = -1.0f; // ��ԋ߂�����
    int index = -1;       // ���̈Í��@�̔ԍ�
    int unusedRCNum = 0;  // �g���ĂȂ����P�b�g�`�F�A�̐�

    // ���P�b�g�`�F�A�̐���
    for (int i = 0; i < Map_RocketChairNum; i++)
    {
        if (rocketChairVec[i]->used == false)
        {
            unusedRCNum++;
            // �����Ƃ̋������o��
            float dis = VSize(VSub(rocketChairVec[i]->position, position));

            // �ŏ���������ۑ�
            if (minDis < 0)
            {
                minDis = dis;
                index = i;
            }
            else
            {
                // ����ɋ߂��̂��o�Ă����炻������ۑ�
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
            // �o�H�T��������
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
