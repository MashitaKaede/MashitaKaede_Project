#include "GameObject.h"

GameObject::GameObject()

    :modelHandle(-1),
     matrix(MGetIdent()),
     position(VGet(0.0f, 0.0f, 0.0f)),
     rotation(VGet(0.0f, 0.0f, 0.0f)),
     scale(VGet(1.0f, 1.0f, 1.0f)), 
     forward(VGet(0.0f, 0.0f, -1.0f)),
     right(VGet(1.0f, 0.0f, 0.0f))
{
}

GameObject::~GameObject()
{
    if (modelHandle != -1)
    {
        MV1DeleteModel(modelHandle);
    }
}

void GameObject::Transform(int modelHandle)
{
    // XYZÌñ]sñðìé
    MATRIX matRotX = MGetRotX(rotation.x);  // X²ñ]sñðìé
    MATRIX matRotY = MGetRotY(rotation.y);  // Y²ñ]sñðìé
    MATRIX matRotZ = MGetRotZ(rotation.z);  // Z²ñ]sñðìé

    // @Z²ñ]sñY²ñ]sñ
    matrix = MMult(matRotZ, matRotY);

    // AZY²ñ]sñX²ñ]sñ
    matrix = MMult(matrix, matRotX);

    // BgkÌsñðìé
    MATRIX matScl = MGetScale(VGet(scale.x, scale.y, scale.z));

    // CgksñXYZñ]sñ
    matrix = MMult(matScl, matrix);

    // DXYZñ]gksñ½sÚ®sñ
    MATRIX matTrans = MGetTranslate(position);
    matrix = MMult(matrix, matTrans);

    // ÅIIÈÀWÏ·sñðfÉKp
    MV1SetMatrix(modelHandle, matrix);

    // »ÝÌñ]}gNXðgÁÄOü«xNgðìé
    MATRIX rotMatrix = MGetIdent();
    rotMatrix.m[0][0] = matrix.m[0][0]; rotMatrix.m[0][1] = matrix.m[0][1]; rotMatrix.m[0][2] = matrix.m[0][2];
    rotMatrix.m[1][0] = matrix.m[1][0]; rotMatrix.m[1][1] = matrix.m[1][1]; rotMatrix.m[1][2] = matrix.m[1][2];
    rotMatrix.m[2][0] = matrix.m[2][0]; rotMatrix.m[2][1] = matrix.m[2][1]; rotMatrix.m[2][2] = matrix.m[2][2];
    forward = VNorm(VTransform(VGet(0.0f, 0.0f, -1.0f), rotMatrix));
    right = VNorm(VTransform(VGet(1.0f, 0.0f, 0.0f), rotMatrix));
}
