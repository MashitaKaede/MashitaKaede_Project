#pragma once
#include "DxLib.h"

class Math
{
public:
    // ���W�A�� �� �f�B�O���[�i�x���j�ϊ��֐�
    static inline float RadToDeg(float Rad){ return Rad * (180.0f / DX_PI_F); }

    // �f�B�O���[ �����W�A�� �i�x���j�ϊ��֐�
    static inline float DegToRad(float Deg) { return Deg * (DX_PI_F / 180.0f); }

    // �|���S���̖@�������߂�
    static VECTOR GetPolygonNormal(VECTOR Vpos1, VECTOR Vpos2, VECTOR Vpos3);
};