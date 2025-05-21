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

// �T�o�C�o�[�̊�{�I��Update
void Survivor::DefaultSurvivorUpdate()
{
    // �`��
    Draw();

    // �R���C�_�[�X�V
    UpdateCollider();

    // AI��Update
    if (mode == Mode::CPU)
    {
        AI();
    }
    // �v���C���[��Update
    else
    {
        Player();
    }

    // �A�j���[�V�����̍X�V
    animator->Update();
}

// �o�H�T���p�|���S���Z�b�g
void Survivor::InitAStar(std::vector<PolyInfo> AStarPoly, int AStarPolyNum, std::vector<PolyInfo> WallPoly, int WallPolyNum)
{
    aStar = new AStar();
    aStar->SetPolyVec_AStar(AStarPoly, AStarPolyNum);
    aStar->SetPolyVec_Wall(WallPoly, WallPolyNum);
}

// �A�C�h�����̏���
void Survivor::Idle()
{
    // �K�v���̉�ǂ��I����ĂȂ����
    if (decodingComplete == false)
    {
        // �������Í��@�����߂�
        if (targetCipherMachine == nullptr)
        {
            DecideDecodigCipherMachine();
        }
        // ���܂��
        else
        {
            // �X�e�[�g��ύX
            state = State_Survivor::GoCipherMachine;
        }
    }
    // �I����Ă����
    else
    {
        // �������L�[�p�b�h�����߂�
        if (targetKeyPad == nullptr)
        {
            DecideInputKeyPad();
        }
        // ���܂��
        else
        {
            // �X�e�[�g��ύX
            state = State_Survivor::GoKeyPad;
        }
    }
}

// �ڕW�Í��@�����߂�
void Survivor::DecideDecodigCipherMachine()
{
    float minDis = -1.0f; // ��ԋ߂�����
    int index = -1;       // ���̈Í��@�̔ԍ�

    // �Í��@�̐���
    for (int i = 0; i < Map_CipherMachineNum; i++)
    {
        // �����Ƃ̋������o��
        float dis = VSize(VSub(cipherMachineVec[i]->position, position));

        // ��ǖ������Ȃ�
        if (cipherMachineVec[i]->GetDecodingComplete() == false)
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

    if (index != -1)
    {
        targetCipherMachine = cipherMachineVec[index];
        // �o�H�T��������
        aStar->ResetPolyVec();
        aStar->SetStartGoalPolyIndex(position, targetCipherMachine->position);
    }
}

// �ڕW�Í��@�Ɍ�����
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

    // �ڕW���W�Ƃ̊Ԃɕǂ�������Β��ڌ�����
    if (collisionMnagaer->GetWallHit(position, targetCipherMachine->position) == false)
    {
        animator->PlayAnim((int)AnimName_Survivor::Run);
        moveVec = VScale(VNorm(VSub(targetCipherMachine->position, position)), moveSpeed * FrameTime);
        position = VAdd(position, moveVec);
    }
    else
    {
        // �o�H�T��
        if (aStar->goal == false)
        {
            aStarNow = true;
            animator->PlayAnim((int)AnimName_Survivor::Idle);
            aStar->Update();
        }
        // �o�H�����܂�����
        if (aStar->goal == true)
        {
            aStarNow = false;
            animator->PlayAnim((int)AnimName_Survivor::Run);
            // �ڕW���W�Ƃ̋���
            float dis = 0.0f;

            // �o�H������ΖڕW���W��H���Ă���
            if (aStar->finalPosVec.size() > 0)
            {
                // �S�ĒH��I����ē������Ȃ������璼�ڌ�����
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

            // �ڕW���W�Ƌ߂Â�����
            if (dis < Survivor_AStar_GoNextPoint_Dis)
            {
                //nowRootNum�ɂ͖ڕW���W�̎c��̍��v���������Ă�
                // ���̍��W�� 
                aStar->nowRootNum--;
            }
        }
    }
    
    // �ڕW�̈Í��@�ƐڐG�������ǂɈڂ�
    if (collisionMnagaer->CheckHit_Capsule_Capsule(this, targetCipherMachine) == true)
    {
        // �X�e�[�g�̕ύX
        state = State_Survivor::Decoding;
        // ��ǒ��Í��@�̃Z�b�g
        decodingCipherMachine = targetCipherMachine;
        // �ڕW�Í��@�̃��Z�b�g
        targetCipherMachine = nullptr;
        
        DecodingPos decoPos = decodingCipherMachine->GetDecodingPos();
        position = decoPos.pos;
        decodingCMIndex = decoPos.index;
        moveVec = VScale(VNorm(VSub(decodingCipherMachine->position, position)), moveSpeed * FrameTime);
    }

    // ���W�ϊ�
    Rotate();
    Transform(modelHandle);
}

// ���
void Survivor::Decoding()
{
    // ���
    decodingCipherMachine->Decoding(addDecodingRate);
    if (mode == Mode::Player)
    {
        DrawBox(1920 / 2 - 200, 600, 1920 / 2 + 200, 620, GetColor(0, 0, 0), true);
        DrawBox(1920 / 2 - 200, 600, 1920 / 2 - 200 + (int)((float)400 * (decodingCipherMachine->GetDecodingRate() / 100.0f)), 620, GetColor(200, 200, 0), true);
        SoundManager::GetInstance()->PlaySE(SE_Name::Decoding);
    }
    
    // ��Ǌ���
    if (decodingCipherMachine->GetDecodingComplete() == true)
    {
        // ��ǒ��̈Í��@�����Z�b�g
        decodingCipherMachine = nullptr;

        // �X�e�[�g�ύX
        state = State_Survivor::Idle;
    }
}

// �ڕW�L�[�p�b�h�����߂�
void Survivor::DecideInputKeyPad()
{
    float minDis = -1.0f; // ��ԋ߂�����
    int index = -1;       // ���̈Í��@�̔ԍ�

    // �Í��@�̐���
    for (int i = 0; i < Map_ExitNum; i++)
    {
        // �����Ƃ̋������o��
        float dis = VSize(VSub(keyPadVec[i]->position, position));

        // ���͊����Ȃ�
        if (keyPadVec[i]->GetInputComplete() == true)
        {
            minDis = dis;
            index = i;
            break;
        }
        else
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

    if (index != -1)
    {
        targetKeyPad = keyPadVec[index];
        inputKeyPadIndex = index;
        // �o�H�T��������
        aStar->ResetPolyVec();
        VECTOR keyPadPos = targetKeyPad->position;
        keyPadPos.y = 0.0f;
        aStar->SetStartGoalPolyIndex(position, keyPadPos);
    }
}

// �ڕW�L�[�p�b�h�Ɍ�����
void Survivor::GoTargetKeyPad()
{
    // �ڕW���W�Ƃ̊Ԃɕǂ�������Β��ڌ�����

    VECTOR keyPadPos = targetKeyPad->position;
    keyPadPos.y = 0.0f;
    if (collisionMnagaer->GetWallHit(position, keyPadPos) == false)
    {
        animator->PlayAnim((int)AnimName_Survivor::Run);
        // �L�[�p�b�h��Y��0�łȂ��̂łO�ɂ��Ă���
        VECTOR targetPos = targetKeyPad->position;
        targetPos.y = 0.0f;
        moveVec = VScale(VNorm(VSub(targetPos, position)), moveSpeed * FrameTime);
        position = VAdd(position, moveVec);
    }
    else
    {
        // �o�H�T��
        if (aStar->goal == false)
        {
            aStarNow = true;
            animator->PlayAnim((int)AnimName_Survivor::Idle);
            aStar->Update();
        }
        // �o�H�����܂�����
        if (aStar->goal == true)
        {
            aStarNow = false;
            animator->PlayAnim((int)AnimName_Survivor::Run);
            // �ڕW���W�Ƃ̋���
            float dis = 0.0f;

            // �o�H������ΖڕW���W��H���Ă���
            if (aStar->finalPosVec.size() > 0)
            {
                // �S�ĒH��I����ē������Ȃ������璼�ڌ�����
                if (aStar->nowRootNum < 0)
                {
                    // �L�[�p�b�h��Y��0�łȂ��̂łO�ɂ��Ă���
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

            // �ڕW���W�Ƌ߂Â�����
            if (dis < Survivor_AStar_GoNextPoint_Dis)
            {
                //nowRootNum�ɂ͖ڕW���W�̎c��̍��v���������Ă�
                // ���̍��W�� 
                aStar->nowRootNum--;
            }
        }
    }
    
    // �ڕW�̃L�[�p�b�h�ƐڐG������p�X���[�h���͂Ɉڂ�
    if (collisionMnagaer->CheckHit_Capsule_Capsule(this, targetKeyPad) == true)
    {
        // �X�e�[�g�ύX
        state = State_Survivor::InputPassward;
        // ���͒��L�[�p�b�h�̃Z�b�g
        inputKeyPad = targetKeyPad;
        // �ڕW�L�[�p�b�h�̃��Z�b�g
        targetKeyPad = nullptr;
    }

    // ���W�ϊ�
    Rotate();
    Transform(modelHandle);
}

// �p�X���[�h����
void Survivor::InputPassward()
{
    if (mode == Mode::Player)
    {
        DrawBox(1920 / 2 - 200, 600, 1920 / 2 + 200, 620, GetColor(0, 0, 0), true);
        DrawBox(1920 / 2 - 200, 600, 1920 / 2 - 200 + (int)((float)400 * (inputKeyPad->GetInputRate() / 100.0f)), 620, GetColor(200, 200, 0), true);
        SoundManager::GetInstance()->PlaySE(SE_Name::Decoding);
    }

    // ����
    inputKeyPad->InputPassward(addInputRate);

    // ���͊���
    if (inputKeyPad->GetInputComplete() == true )
    {
        if(mode == Mode::CPU)
        {
            // �X�e�[�g�ύX
            state = State_Survivor::GoExit;

            // �o�H�T��������
            aStar->ResetPolyVec();
            aStar->SetStartGoalPolyIndex(position, posVec_Exit_AStar[inputKeyPadIndex]);
        }
        else
        {
            // �X�e�[�g�ύX
            state = State_Survivor::Idle;
        }
             
        // ���͒��̃L�[�p�b�h���Z�b�g
        inputKeyPad = nullptr;   
    }
}

// �E�o
void Survivor::Escape()
{
    // �ڕW���W�Ƃ̊Ԃɕǂ�������Β��ڌ�����
    if (collisionMnagaer->GetWallHit(position, posVec_Exit_AStar[inputKeyPadIndex]) == false)
    {
        animator->PlayAnim((int)AnimName_Survivor::Run);
        moveVec = VScale(VNorm(VSub(posVec_Exit_AStar[inputKeyPadIndex], position)), moveSpeed * FrameTime);
        position = VAdd(position, moveVec);
    }
    else
    {
        // �o�H�T��
        if (aStar->goal == false)
        {
            aStarNow = true;
            animator->PlayAnim((int)AnimName_Survivor::Idle);
            aStar->Update();
        }
        // �o�H�����܂�����
        if (aStar->goal == true)
        {
            aStarNow = false;
            animator->PlayAnim((int)AnimName_Survivor::Run);
            // �ڕW���W�Ƃ̋���
            float dis = 0.0f;

            // �o�H������ΖڕW���W��H���Ă���
            if (aStar->finalPosVec.size() > 0)
            {
                moveVec = VScale(VNorm(VSub(aStar->finalPosVec[aStar->nowRootNum], position)), moveSpeed * FrameTime);
                position = VAdd(position, moveVec);
                dis = VSize(VSub(position, aStar->finalPosVec[aStar->nowRootNum]));
            }

            // �ڕW���W�Ƌ߂Â�����
            if (dis < Survivor_AStar_GoNextPoint_Dis)
            {
                //nowRootNum�ɂ͖ڕW���W�̎c��̍��v���������Ă�

                // �Ō�܂ł��ǂ蒅������N���A
                if (aStar->nowRootNum <= 0)
                {
                    state = State_Survivor::GameClear;
                }
                // ���̍��W��
                else
                {
                    aStar->nowRootNum--;
                }
            }
        }
    }

    // ���W�ϊ�
    Rotate();
    Transform(modelHandle);
}

// �~���ɍs�����`�F�b�N
void Survivor::CheckGoRescue()
{
    // ���̃T�o�C�o�[�̐�
    int otherSurvivorNum = (int)otherSurvivorVec.size();

    // �~���Ώۂ̃T�o�C�o�[
    Survivor* survivor = nullptr;

    // �߂܂��Ă�T�o�C�o�[�̒��ň�ԋ߂�����
    float minDis = 0.0f;

    for (int i = 0; i < otherSurvivorNum; i++)
    {
        // �߂܂��Ă���
        if (otherSurvivorVec[i]->GetState() == State_Survivor::Restrainting)
        {
            // �����Ƃ̋������o��
            float dis = VSize(VSub(otherSurvivorVec[i]->position, position));

            // �~���Ώۂ��܂����Ȃ����
            if (survivor == nullptr)
            {
                // �����ƃT�o�C�o�[��ۑ�
                survivor = otherSurvivorVec[i];
                minDis = dis;
            }
            // �����ꍇ�������ׂ�
            else
            {
                // �����Ƌ߂����
                if (minDis > dis)
                {
                    // �����ƃT�o�C�o�[���㏑��
                    survivor = otherSurvivorVec[i];
                    minDis = dis;
                }
            }
        }
    }

    // �~���Ώۂ������ꍇ
    if (survivor != nullptr)
    {
        // �~���Ώۂɓo�^
        rescueSurvivor = survivor;

        // ��ǒ��̏ꍇ�A��Ǘ��������ȏ�ɂȂ�����~���ɍs��
        if (state == State_Survivor::Decoding)
        {
            if (decodingCipherMachine->GetDecodingRate() < 50.0f)
            {
                decodingCipherMachine->LeaveDecodeingPos(decodingCMIndex);
                // �X�e�[�g�ύX
                state = State_Survivor::GoRescue;
                // �o�H�T��������               
                aStar->ResetPolyVec();
                aStar->SetStartGoalPolyIndex(position, rescueSurvivor->position);
            }
        }
        else
        {
            // �X�e�[�g�ύX
            state = State_Survivor::GoRescue;
            // �o�H�T��������               
            aStar->ResetPolyVec();
            aStar->SetStartGoalPolyIndex(position, rescueSurvivor->position);
        }
    }
}

// �~���Ɍ�����
void Survivor::GoRescue()
{
    // �ڕW���W�Ƃ̊Ԃɕǂ�������Β��ڌ�����
    if (collisionMnagaer->GetWallHit(position, rescueSurvivor->position) == false)
    {
        animator->PlayAnim((int)AnimName_Survivor::Run);
        moveVec = VScale(VNorm(VSub(rescueSurvivor->position, position)), moveSpeed * FrameTime);
        position = VAdd(position, moveVec);
    }
    else
    {
        // �o�H�T��
        if (aStar->goal == false)
        {
            aStarNow = true;
            animator->PlayAnim((int)AnimName_Survivor::Idle);
            aStar->Update();
        }
        // �o�H�����܂�����
        if (aStar->goal == true)
        {
            aStarNow = false;
            animator->PlayAnim((int)AnimName_Survivor::Run);
            // �ڕW���W�Ƃ̋���
            float dis = 0.0f;

            // �o�H������ΖڕW���W��H���Ă���
            if (aStar->finalPosVec.size() > 0)
            {
                // �S�ĒH��I����ē������Ȃ������璼�ڌ�����
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

            // �ڕW���W�Ƌ߂Â�����
            if (dis < Survivor_AStar_GoNextPoint_Dis)
            {
                //nowRootNum�ɂ͖ڕW���W�̎c��̍��v���������Ă�
                // ���̍��W�� 
                aStar->nowRootNum--;
            }
        }
    }

    // �~���Ώۂ̕߂܂��Ă郍�P�b�g�`�F�A�ƐڐG������~���Ɉڂ�
    if (collisionMnagaer->CheckHit_Capsule_Capsule(this, rescueSurvivor->GetRocketChair()) == true)
    {
        // �X�e�[�g�̕ύX
        state = State_Survivor::Rescue;
    }

    // �����~���Ώۂ��߂܂��Ă��ԂłȂ���΃A�C�h����Ԃ�
    if (rescueSurvivor->GetState() != State_Survivor::Restrainting)
    {
        rescueSurvivor = nullptr;
        state = State_Survivor::Idle;
    }

    // ���W�ϊ�
    Rotate();
    Transform(modelHandle);
}

// �~��
void Survivor::RescueSurvivor()
{
    // �~���Ώۂ����P�b�g�`�F�A�̏��������Ă��
    if (rescueSurvivor->GetRocketChair()->GetSurvivor() != nullptr)
    {
        // �~����i�߂�
        rescueSurvivor->SetIsRescuing(true);
        rescueSurvivor->GetRocketChair()->AddReleseRate(Survivor_Default_AddRescueRate);
        if (mode == Mode::Player)
        {
            DrawBox(1920 / 2 - 200, 600, 1920 / 2 + 200, 620, GetColor(0, 0, 0), true);
            DrawBox(1920 / 2 - 200, 600, 1920 / 2 - 200 + (int)((float)400 * (rescueSurvivor->GetRocketChair()->GetReleaseRate() / 100.0f)), 620, GetColor(200, 200, 0), true);
        }
    }
    // ������΃A�C�h����Ԃɖ߂�
    else
    {
        // �~���Ώۂ����Z�b�g
        rescueSurvivor->SetIsRescuing(false);
        rescueSurvivor = nullptr;

        // �X�e�[�g�̕ύX
        state = State_Survivor::Idle;
    }
}

// �����邩�`�F�b�N
void Survivor::CheckRunAway()
{
    // �n���^�[�Ƃ̋������o��
    float disFromHunter = VSize(VSub(hunter->position, position));

    // ���������ȉ��������瓦����ꏊ�����߂�
    if (disFromHunter <= Survivor_StartRunAway_Dis)
    {
        if (state == State_Survivor::Decoding && decodingCipherMachine != nullptr)
        {
            decodingCipherMachine->LeaveDecodeingPos(decodingCMIndex);
        }
        DecideRunAwayRoot();
    }
}

// �����ꏊ����
void Survivor::DecideRunAwayRoot()
{
    // ������ꏊ�̐�
    int runAwayPointNum = (int)runAwayPointVec.size();

    // �n���^�[�̕���
    VECTOR hunterDir = VSub(hunter->position, position);

    // ��ԏ���������
    float minDot = 1.0f;

    // ������ꏊ�̔ԍ�
    runAwayPointIndex = -1;

    for (int i = 0; i < runAwayPointNum; i++)
    {
        // ���̏ꏊ�Ƃ̋������o�� 
        float dis = VSize(VSub(runAwayPointVec[i], position));

        // ���ȉ��A�ȏゾ������m�F���Ȃ��i�����ꏊ�ɍs�����Ȃ����߁A��������ƒT���Ɏ��Ԃ������邽�߁j
        if (dis > 3500.0f || dis < 300.0f)
        {
            continue;
        }

        // ���̏ꏊ�̕������o��
        VECTOR pointDir = VSub(runAwayPointVec[i], position);

        // �n���^�[�̕����Ƃ̓���
        float dot = VDot(hunterDir, pointDir);

        // ����ɏ������̂��o����
        if (minDot > dot)
        {
            // �X�V
            minDot = dot;
            runAwayPointIndex = i;
        }


    }

    // �S�ă_���������烉���_���Ɍ�����
    if (runAwayPointIndex == -1)
    {
        runAwayPointIndex = GetRand(runAwayPointNum - 1);
    }

    // �X�e�[�g�̕ύX
    state = State_Survivor::RunAway;

    // �o�H�T���̏�����
    aStar->ResetPolyVec();
    aStar->SetStartGoalPolyIndex(position, runAwayPointVec[runAwayPointIndex]);
}

// ������
void Survivor::RunAway()
{
    // �ڕW���W�Ƃ̊Ԃɕǂ�������Β��ڌ�����
    if (collisionMnagaer->GetWallHit(position, runAwayPointVec[runAwayPointIndex]) == false)
    {
        animator->PlayAnim((int)AnimName_Survivor::Run);
        moveVec = VScale(VNorm(VSub(runAwayPointVec[runAwayPointIndex], position)), moveSpeed * FrameTime);
        position = VAdd(position, moveVec);
    }
    else
    {
        // �o�H�T��
        if (aStar->goal == false)
        {
            aStarNow = true;
            animator->PlayAnim((int)AnimName_Survivor::Idle);
            aStar->Update();
        }
        // �o�H�����܂�����
        if (aStar->goal == true)
        {
            aStarNow = false;
            animator->PlayAnim((int)AnimName_Survivor::Run);
            float dis = 0.0f;

            // �o�H������ΖڕW���W��H���Ă���
            if (aStar->finalPosVec.size() > 0)
            {
                moveVec = VScale(VNorm(VSub(aStar->finalPosVec[aStar->nowRootNum], position)), moveSpeed * FrameTime);
                position = VAdd(position, moveVec);
                dis = VSize(VSub(position, aStar->finalPosVec[aStar->nowRootNum]));
            }

            // �ڕW���W�Ƌ߂Â�����
            if (dis < 50.0f)
            {
                //nowRootNum�ɂ͖ڕW���W�̎c��̍��v���������Ă�

                // �Ō�܂ł��ǂ蒅������
                if (aStar->nowRootNum == 0)
                {
                    // ������x�o�H�T��
                    DecideRunAwayRoot();
                }
                else
                {
                    // ���̍��W��
                    aStar->nowRootNum--;
                }
            }
        }
    }

    // �ڕW���W�Ƃ̋���
    float targetDis = VSize(VSub(runAwayPointVec[runAwayPointIndex], position));

    // �ڕW���W�Ƌ߂Â�����
    if (targetDis < 50.0f)
    {
        // ������x�o�H�T��
        DecideRunAwayRoot();
    }

    // �n���^�[�Ƃ̋������o�� 
    float dis = VSize(VSub(hunter->position, position));

    // �n���^�[�ƈ��ȏ㗣�ꂽ��A�C�h����Ԃɖ߂��Ď��̍s�������߂�����
    if (dis > Survivor_EndRunAway_Dis)
    {
        state = State_Survivor::Idle;
    }

    // �����Í��@���K�v����Ǐo���Ă���΃A�C�h����Ԃ��玟�̍s�������߂�����
    if (decodingComplete == true)
    {
        state = State_Survivor::Idle;
    }

    // ���W�ϊ�
    Rotate();
    Transform(modelHandle);
}

// �N���A�`�F�b�N
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

// �s���Ώۂ̃|�C���^�̃��Z�b�g
void Survivor::ResetActionTargets()
{
    // ��ǒ��̈Í��@
    if (state != State_Survivor::Decoding)
    {
        decodingCipherMachine = nullptr;
    }

    // ���͒��̃L�[�p�b�h
    if (state != State_Survivor::InputPassward)
    {
        inputKeyPad = nullptr;
    }

    // �~���Ώۂ̃T�o�C�o�[
    if (state != State_Survivor::Rescue && state != State_Survivor::GoRescue)
    {
        if (rescueSurvivor != nullptr)
        {
            rescueSurvivor->SetIsRescuing(false);
            rescueSurvivor = nullptr;
        }
    } 
}

// ��΂���鎞�̏���
void Survivor::Flying()
{
    // ���P�b�g�`�F�A�ƃT�o�C�o�[���΂�
    position.y += Survivor_FlyingSpeed;
    Transform(modelHandle);
    rocketChair->position.y += Survivor_FlyingSpeed;
    rocketChair->Transform(rocketChair->GetModelHandle());
    rocketChair->UpdateCollider();

    // ���ȏ�̍��x�ɂȂ�����Q�[���I�[�o�[
    if (position.y > Survivor_Max_FlyingHeight)
    {
        state = State_Survivor::GameOver;
    }
}

// �߂܂������̏���
void Survivor::AddRestraintCnt()
{
    restraintCnt++;
    // ���ڂ͉����Ȃ�

    // ���ڂ�
    if (restraintCnt == 2)
    {
        // �Q�[���I�[�o�[�Q�[�W�������ȉ��Ȃ甼����
        if (gameOverRate <= 50.0f)
        {
            gameOverRate = 50.0f;
        }
        // �����ȏ�Ȃ��΂�
        else if (gameOverRate > 50.0f)
        {
            gameOverRate = 100.0f;
            state = State_Survivor::Flying;
        }
    }

    // �O��ڂȂ��΂�
    if (restraintCnt == 3)
    {
        gameOverRate = 100.0f;
        state = State_Survivor::Flying;
    }
}

// �߂܂��Ă鎞�̏���
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

// �X�e�[�g�ɉ����ăX�s�[�h�ύX
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

// AI��Update
void Survivor::AI()
{
    switch (state)
    {
    case State_Survivor::None:
        break;
    case State_Survivor::Idle: // �A�C�h����
        animator->PlayAnim((int)AnimName_Survivor::Idle);
        Idle();
        if (decodingComplete == false)
        {
            // �����邩�`�F�b�N
            CheckRunAway();
            // �~���ɍs�����`�F�b�N
            CheckGoRescue();
        }
        // �U�����󂯂�
        isVulnerable = true;
        break;
    case State_Survivor::GoCipherMachine: // �Í��@�Ɍ������Ă�
        GoTargetCipherMachine();
        // �����邩�`�F�b�N
        CheckRunAway();
        // �~���ɍs�����`�F�b�N
        CheckGoRescue();
        // �U�����󂯂�
        isVulnerable = true;
        break;
    case State_Survivor::Decoding: // ��ǒ�
        animator->PlayAnim((int)AnimName_Survivor::Decoding);
        Decoding();
        // �����邩�`�F�b�N
        CheckRunAway();
        // �~���ɍs�����`�F�b�N
        CheckGoRescue();
        // �U�����󂯂�
        isVulnerable = true;
        break;
    case State_Survivor::GoKeyPad: // �L�[�p�b�h�Ɍ������Ă�
        GoTargetKeyPad();
        // �U�����󂯂�
        isVulnerable = true;
        break;
    case State_Survivor::InputPassward: // �p�X���[�h���͒�
        animator->PlayAnim((int)AnimName_Survivor::Input);
        InputPassward();
        // �U�����󂯂�
        isVulnerable = true;
        break;
    case State_Survivor::GoExit: // �o���Ɍ������Ă�
        Escape();
        // �U�����󂯂�
        isVulnerable = true;
        break;
    case State_Survivor::RunAway: // ������
        RunAway();
        // �U�����󂯂�
        isVulnerable = true;
        break;
    case State_Survivor::GoRescue: // �~���Ɍ������Ă�
        GoRescue();
        // �����邩�`�F�b�N
        CheckRunAway();
        // �U�����󂯂�
        isVulnerable = true;
        break;
    case State_Survivor::Rescue: // �~����
        animator->PlayAnim((int)AnimName_Survivor::Rescue);
        RescueSurvivor();
        // �U�����󂯂�
        isVulnerable = true;
        break;
    case State_Survivor::Fall: // �_�E����
        animator->PlayAnim((int)AnimName_Survivor::FallIdle);
        // �U�����󂯂�
        isVulnerable = false;
        break;
    case State_Survivor::LiftedUp: // �S���ꒆ
        animator->PlayAnim((int)AnimName_Survivor::FallIdle);
        break;
    case State_Survivor::Restrainting: // �S����
        animator->PlayAnim((int)AnimName_Survivor::Restrainting);
        Restrainting();
        break;
    case State_Survivor::Flying: // ��΂��ꒆ
        animator->PlayAnim((int)AnimName_Survivor::Restrainting);
        Flying();
        // �U�����󂯂Ȃ�
        isVulnerable = false;
        break;
    case State_Survivor::GameOver: // �Q�[���I�[�o�[
        animator->PlayAnim((int)AnimName_Survivor::Restrainting);
        // �U�����󂯂Ȃ�
        isVulnerable = false;
        break;
    case State_Survivor::GameClear: // �Q�[���N���A
        animator->PlayAnim((int)AnimName_Survivor::Run);
        // �U�����󂯂Ȃ�
        isVulnerable = false;
        break;
    default:
        break;
    }

    // �s���Ώۂ̃|�C���^�̃��Z�b�g
    ResetActionTargets();

    // �X�s�[�h�ύX
    ChangeSpeed();

    // �N���A�`�F�b�N
    CheckGameClear();
}

// Player��Update
void Survivor::Player()
{
    switch (state)
    {
    case State_Survivor::None:
        break;
    case State_Survivor::Idle: // �A�C�h����
        animator->PlayAnim((int)AnimName_Survivor::Idle);
        // �ړ����邩�`�F�b�N
        CheckMove();
        // �s���\
        actionDisabled = false;
        // �U�����󂯂�
        isVulnerable = true;
        break;
    case State_Survivor::Move: // �ړ���
        animator->PlayAnim((int)AnimName_Survivor::Run);
        // �ړ�
        Move();
        // ��~���邩�`�F�b�N
        CheckIdle();
        // �s���\
        actionDisabled = false;
        // �U�����󂯂�
        isVulnerable = true;
        break;
    case State_Survivor::Decoding: // ��ǒ�
        animator->PlayAnim((int)AnimName_Survivor::Decoding);
        // ���
        Decoding();
        // �ړ����邩�`�F�b�N
        CheckMove();
        // �s���\
        actionDisabled = false;
        // �U�����󂯂�
        isVulnerable = true;
        break;
    case State_Survivor::InputPassward: // �p�X���[�h���͒�
        animator->PlayAnim((int)AnimName_Survivor::Input);
        // ����
        InputPassward();
        // �ړ����邩�`�F�b�N
        CheckMove();
        // �s���\
        actionDisabled = false;
        // �U�����󂯂�
        isVulnerable = true;
        break;
    case State_Survivor::Rescue: // �~����
        animator->PlayAnim((int)AnimName_Survivor::Rescue);
        // �~��
        RescueSurvivor();
        // �ړ����邩�`�F�b�N
        CheckMove();
        // �s���\
        actionDisabled = false;
        // �U�����󂯂�
        isVulnerable = true;
        break;
    case State_Survivor::Fall: // �_�E����        
        // �ړ�
        Move();
        // �s���s�\
        actionDisabled = true;
        if (VSize(moveVec) != 0.0f)
        {
            animator->PlayAnim((int)AnimName_Survivor::FallWalk);
        }
        else
        {
            animator->PlayAnim((int)AnimName_Survivor::FallIdle);
        }
        // �U�����󂯂Ȃ�
        isVulnerable = false;
        break;
    case State_Survivor::LiftedUp: // �S���ꒆ
        animator->PlayAnim((int)AnimName_Survivor::FallIdle);
        // �s���s�\
        actionDisabled = true;
        // �U�����󂯂Ȃ�
        isVulnerable = false;
        break;
    case State_Survivor::Restrainting: // �S����
        animator->PlayAnim((int)AnimName_Survivor::Restrainting);
        Restrainting();
        // �s���s�\
        actionDisabled = true;
        break;
    case State_Survivor::Flying: // ��΂��ꒆ
        animator->PlayAnim((int)AnimName_Survivor::Restrainting);
        Flying();
        // �s���s�\
        actionDisabled = true;
        // �U�����󂯂Ȃ�
        isVulnerable = false;
        break;
    case State_Survivor::GameOver: // �Q�[���I�[�o�[
        animator->PlayAnim((int)AnimName_Survivor::Restrainting);
        // �s���s�\
        actionDisabled = true;
        // �U�����󂯂Ȃ�
        isVulnerable = false;
        break;
    case State_Survivor::GameClear: // �Q�[���N���A
        animator->PlayAnim((int)AnimName_Survivor::Run);
        // �s���s�\
        actionDisabled = true;
        // �U�����󂯂Ȃ�
        isVulnerable = false;
        break;
    default:
        break;
    }

    // �s���Ώۂ̃|�C���^�̃��Z�b�g
    ResetActionTargets();

    // �X�s�[�h�ύX
    ChangeSpeed();

    // �N���A�`�F�b�N
    CheckGameClear();

    // ���n
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

// �L�[�{�[�h�����Xbox���썇�킹���֐�
void Survivor::Move()
{
    // XBox�R���g���[���[���q�����Ă���XBox����
    if (GetJoypadNum() > 0)
    {
        Move_XBox();
    }
    // �q�����ĂȂ��ꍇ�L�[�{�[�h����i���͎g���Ȃ��j
    else
    {
        Move_Key();
    }
}

// �L�[�{�[�h����i���͎g���Ȃ��j
void Survivor::Move_Key()
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
    Rotate();

    // ���W�ϊ�
    Transform(modelHandle);
}

// Xbox����
void Survivor::Move_XBox()
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

// �ړ����邩�`�F�b�N
void Survivor::CheckMove()
{
    // �X�e�B�b�N���͂������
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
        // �X�e�[�g�̕ύX
        state = State_Survivor::Move;
    }
}

// ��~���邩�`�F�b�N
void Survivor::CheckIdle()
{
    // �X�e�B�b�N���͂��������
    if (Input::GetInstance()->GetStickInput(true, false) == false)
    {
        // �X�e�[�g�̕ύX
        state = State_Survivor::Idle;
    }
}

// �f�o�b�O�\��
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
    // �o�H�T���̏�����
    if (mode == Mode::CPU)
    {
        aStar->ResetPolyVec();
    }
}

// �����蔻��
void Survivor::OnCollisionHit(Collider* Collider, GameObject* Parent)
{
    // �Í��@�ƐG��Ă���ꍇ
	if (Parent->tag == "CipherMachine")
	{
        // �s���ł����
		if (actionDisabled == false)
		{
            // B�{�^����������邩���[�h���v���C���[��������
			if (Input::GetInstance()->GetButtonDown(ButtonName::Button_B) && mode == Mode::Player)
			{
                // ���łɉ�ǒ��ł����
                if (state == State_Survivor::Decoding)
                {
                    decodingCipherMachine->LeaveDecodeingPos(decodingCMIndex);
                    // ��ǒ��̈Í��@���Z�b�g
                    decodingCipherMachine = nullptr;

                    // �X�e�[�g�̕ύX
                    state = State_Survivor::Idle;
                }
                // ��ǒ��łȂ����
                else
                {
                    // ��ǒ��̈Í��@�ɓo�^
                    decodingCipherMachine = (CipherMachine*)Parent;

                    // ��ǂ��I����ĂȂ����
                    if (decodingCipherMachine->GetDecodingComplete() == false)
                    {
                        // �X�e�[�g�̕ύX
                        state = State_Survivor::Decoding;
                        
                        DecodingPos decoPos = decodingCipherMachine->GetDecodingPos();
                        position = decoPos.pos;
                        decodingCMIndex = decoPos.index;
                        moveVec = VScale(VNorm(VSub(decodingCipherMachine->position, position)), moveSpeed * FrameTime);
                        // ���W�ϊ�
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

    // �L�[�p�b�h�ƐG��Ă���ꍇ
    if (Parent->tag == "Keypad")
    {
        // �s���o�����
        if (actionDisabled == false)
        {
            // B�{�^����������邩���[�h���v���C���[��������
            if (Input::GetInstance()->GetButtonDown(ButtonName::Button_B) && mode == Mode::Player)
            {
                // ���łɓ��͒��Ȃ�
                if (state == State_Survivor::InputPassward)
                {
                    // ���͒��̃L�[�p�b�h�����Z�b�g
                    inputKeyPad = nullptr;

                    // �X�e�[�g�̕ύX
                    state = State_Survivor::Idle;
                }
                // ���͒��łȂ����
                else
                {
                    // ���͒��̃L�[�p�b�h�ɓo�^
                    inputKeyPad = (Keypad*)Parent;

                    // �X�e�[�g�̕ύX
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

    // �n���^�[�̍U�������������ꍇ
    if (Collider->colliderName == "HunterAtk")
    {       
        // �߂܂��Ă�Ƃ��͋�U�肳����
        if (state == State_Survivor::Restrainting)
        {
            Collider->isCheck = false;
            Hunter* hunter = (Hunter*)Parent;
            hunter->SetState(State_Hunter::AtkHit);
            return;
        }

        // �U�����󂯂��Ԃ�������
        if (isVulnerable == true)
        {
            // ��ǁA���́A�~�����͈ꌂ�ł����
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
            // ����ȊO�͔����̃_���[�W���󂯂�
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

            // hp���O�ɂȂ�����
            if (hp <= 0)
            {
                // �_�E����Ԃɂ���
                state = State_Survivor::Fall;

                hp = 0;
            }

            // �n���^�[���U�����q�b�g��Ԃɂ���
            Collider->isCheck = false;
            Hunter* hunter = (Hunter*)Parent;
            hunter->SetState(State_Hunter::AtkHit);
            if (hunter->mode == Mode::Player)
            {
                SoundManager::GetInstance()->PlaySE(SE_Name::AttackHit);
            }
        }
    }

    // ���P�b�g�`�F�A�ɐG��Ă���ꍇ
    if (Parent->tag == "RocketChair")
    {
        // �G�ꂽ���P�b�g�`�F�A���擾
        RocketChair* rocketChair = (RocketChair*)Parent;

        // ���P�b�g�`�F�A�ɃT�o�C�o�[���߂܂��Ă�ꍇ
        if (rocketChair->GetSurvivor() != nullptr)
        {
            // �s���ł����
            if (actionDisabled == false)
            {
                // B�{�^����������邩���[�h���v���C���[��������
                if (Input::GetInstance()->GetButtonDown(ButtonName::Button_B) && mode == Mode::Player)
                {
                    // ���łɋ~�����Ȃ�
                    if (state == State_Survivor::Rescue)
                    {
                        // �~���Ώۂ����Z�b�g
                        rescueSurvivor->GetRocketChair()->AddReleseRate(-100.0f);
                        rescueSurvivor->SetIsRescuing(false);
                        rescueSurvivor = nullptr;

                        // �X�e�[�g�̕ύX
                        state = State_Survivor::Idle;
                    }
                    // �~�����łȂ����
                    else
                    {
                        // �~���Ώۂɓo�^
                        rescueSurvivor = rocketChair->GetSurvivor();
                        rescueSurvivor->SetIsRescuing(true);
                        // �X�e�[�g�̕ύX
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