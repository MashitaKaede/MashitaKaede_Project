#include "Math.h"

VECTOR Math::GetPolygonNormal(VECTOR Vpos1, VECTOR Vpos2, VECTOR Vpos3) 
{
    VECTOR V12 = VSub(Vpos2, Vpos1);
    VECTOR V13 = VSub(Vpos3, Vpos1);
    VECTOR normal = VCross(V12, V13);
    normal = VNorm(normal);
    return normal;
}