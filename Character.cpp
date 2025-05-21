#include "Character.h"

Character::Character()

    :cameraRotH(0.0f),
     defaultMoveSpeed(0.0f),
     moveSpeed(0.0f),
     moveVec(VGet(0.0f, 0.0f, 0.0f)),
     slideVec(VGet(0.0f, 0.0f, 0.0f)),
     collisionMnagaer(nullptr),
     animator(nullptr),
     mode(Mode::CPU)
{
}

void Character::Draw()
{
    MV1DrawModel(modelHandle);
}

void Character::Rotate()
{
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
}

void Character::Debug()
{
    DrawFormatString(600, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "");
    Debug_DispCnt++;
    DrawFormatString(600, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "%s", const_cast<char*>(name.c_str()));
    Debug_DispCnt++;
    DrawFormatString(600, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "���W:X/%.1f, Y/%.1f, Z/%.1f", position.x, position.y, position.z);
    Debug_DispCnt++;
    DrawFormatString(600, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "��]:X/%.1f, Y/%.1f, Z/%.1f", Math::RadToDeg(rotation.x), Math::RadToDeg(rotation.y), Math::RadToDeg(rotation.z));
    Debug_DispCnt++;
}

void Character::UpdateCollider()
{
    // �J�v�Z���̎n�_�ƏI�_�X�V
    startPos = position;
    endPos = startPos;
    endPos.y += Character_Default_ColliderHeight;
}
