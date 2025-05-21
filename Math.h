#pragma once
#include "DxLib.h"

class Math
{
public:
    // ラジアン → ディグリー（度数）変換関数
    static inline float RadToDeg(float Rad){ return Rad * (180.0f / DX_PI_F); }

    // ディグリー →ラジアン （度数）変換関数
    static inline float DegToRad(float Deg) { return Deg * (DX_PI_F / 180.0f); }

    // ポリゴンの法線を求める
    static VECTOR GetPolygonNormal(VECTOR Vpos1, VECTOR Vpos2, VECTOR Vpos3);
};