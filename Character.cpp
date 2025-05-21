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
}

void Character::Debug()
{
    DrawFormatString(600, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "");
    Debug_DispCnt++;
    DrawFormatString(600, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "%s", const_cast<char*>(name.c_str()));
    Debug_DispCnt++;
    DrawFormatString(600, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "座標:X/%.1f, Y/%.1f, Z/%.1f", position.x, position.y, position.z);
    Debug_DispCnt++;
    DrawFormatString(600, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "回転:X/%.1f, Y/%.1f, Z/%.1f", Math::RadToDeg(rotation.x), Math::RadToDeg(rotation.y), Math::RadToDeg(rotation.z));
    Debug_DispCnt++;
}

void Character::UpdateCollider()
{
    // カプセルの始点と終点更新
    startPos = position;
    endPos = startPos;
    endPos.y += Character_Default_ColliderHeight;
}
