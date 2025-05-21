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
    // XYZ�̉�]�s������
    MATRIX matRotX = MGetRotX(rotation.x);  // X����]�s������
    MATRIX matRotY = MGetRotY(rotation.y);  // Y����]�s������
    MATRIX matRotZ = MGetRotZ(rotation.z);  // Z����]�s������

    // �@Z����]�s��Y����]�s��
    matrix = MMult(matRotZ, matRotY);

    // �AZY����]�s��X����]�s��
    matrix = MMult(matrix, matRotX);

    // �B�g�k�̍s������
    MATRIX matScl = MGetScale(VGet(scale.x, scale.y, scale.z));

    // �C�g�k�s��XYZ��]�s��
    matrix = MMult(matScl, matrix);

    // �DXYZ��]�g�k�s�񂘕��s�ړ��s��
    MATRIX matTrans = MGetTranslate(position);
    matrix = MMult(matrix, matTrans);

    // �ŏI�I�ȍ��W�ϊ��s������f���ɓK�p
    MV1SetMatrix(modelHandle, matrix);

    // ���݂̉�]�}�g���N�X���g���đO�����x�N�g�������
    MATRIX rotMatrix = MGetIdent();
    rotMatrix.m[0][0] = matrix.m[0][0]; rotMatrix.m[0][1] = matrix.m[0][1]; rotMatrix.m[0][2] = matrix.m[0][2];
    rotMatrix.m[1][0] = matrix.m[1][0]; rotMatrix.m[1][1] = matrix.m[1][1]; rotMatrix.m[1][2] = matrix.m[1][2];
    rotMatrix.m[2][0] = matrix.m[2][0]; rotMatrix.m[2][1] = matrix.m[2][1]; rotMatrix.m[2][2] = matrix.m[2][2];
    forward = VNorm(VTransform(VGet(0.0f, 0.0f, -1.0f), rotMatrix));
    right = VNorm(VTransform(VGet(1.0f, 0.0f, 0.0f), rotMatrix));
}
