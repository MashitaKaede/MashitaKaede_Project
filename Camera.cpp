#include "Camera.h"

Camera::Camera()

	: chaseTarget(nullptr),
	  position(VGet(0.0f, 0.0f, 0.0f)),
	  rotH(Math::DegToRad(-90.0f)),
	  rotV(Math::DegToRad(30.0f))
{
}

Camera::~Camera()
{
}

void Camera::Init()
{
}

void Camera::Term()
{
}

void Camera::Update()
{
	Rotate();
	ChaseCharacter();
}

void Camera::Rotate()
{
	if (GetJoypadNum() > 0)
	{
		Rotate_XBox();
	}
	else
	{
		Rotate_Key();
	}
}

void Camera::Rotate_Key()
{
	// ��
	if (Input::GetInstance()->GetKye(KeyName::Right))
	{
		rotH -= Math::DegToRad(1.0f);
	}
	if (Input::GetInstance()->GetKye(KeyName::Left))
	{
		rotH += Math::DegToRad(1.0f);
	}

	// 180��������}�C�i�X��
	if (rotH > DX_PI_F)
	{
		rotH = rotH - DX_PI_F * 2;
	}
	// -180��������v���X��
	if (rotH < -DX_PI_F)
	{
		rotH = -rotH + DX_PI_F * 2;
	}

	// �c
	if (Input::GetInstance()->GetKye(KeyName::Up))
	{
		rotV -= Math::DegToRad(1.0f);
	}
	if (Input::GetInstance()->GetKye(KeyName::Down))
	{
		rotV += Math::DegToRad(1.0f);
	}

	// 90��������X�g�b�v
	if (rotV > DX_PI_F / 2)
	{
		rotV = DX_PI_F / 2;
	}
	// -90��������X�g�b�v
	if (rotV < -DX_PI_F / 2)
	{
		rotV = -DX_PI_F / 2;
	}
}

void Camera::Rotate_XBox()
{
	if (Input::GetInstance()->GetStickInput(false, true))
	{
		// ��
		if (Input::GetInstance()->GetXInputState().ThumbRX > 0)
		{
			rotH -= Math::DegToRad(Camera_RotateSpeed_X);
		}
		if (Input::GetInstance()->GetXInputState().ThumbRX < 0)
		{
			rotH += Math::DegToRad(Camera_RotateSpeed_X);
		}

		// 180��������}�C�i�X��
		if (rotH > DX_PI_F)
		{
			rotH = rotH - DX_PI_F * 2;
		}
		// -180��������v���X��
		if (rotH < -DX_PI_F)
		{
			rotH = -rotH + DX_PI_F * 2;
		}

		// �c
		if (Input::GetInstance()->GetXInputState().ThumbRY > 0)
		{
			rotV -= Math::DegToRad(Camera_RotateSpeed_Y);
		}
		if (Input::GetInstance()->GetXInputState().ThumbRY < 0)
		{
			rotV += Math::DegToRad(Camera_RotateSpeed_Y);
		}

		// �����������X�g�b�v
		if (rotV > Math::DegToRad(Camera_Max_AngleV))
		{
			rotV = Math::DegToRad(Camera_Max_AngleV);
		}
		// ������������X�g�b�v
		if (rotV < Math::DegToRad(Camera_Min_AngleV))
		{
			rotV = Math::DegToRad(Camera_Min_AngleV);
		}
	}
}

void Camera::ChaseCharacter()
{
	if (chaseTarget != nullptr)
	{
		VECTOR lookPos = chaseTarget->position;
		lookPos.y += Camera_TargetHeight;
		VECTOR cameraPos = VScale(VGet(cosf(rotH) * cosf(rotV), sinf(rotV), sinf(rotH) * cosf(rotV)), Camera_TargetDis);
		position = VAdd(cameraPos, lookPos);
		SetCameraPositionAndTarget_UpVecY(position, lookPos);
	}
}

void Camera::Debug()
{
	/*DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "");
	Debug_DispCnt++;
	DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "�J����");
	Debug_DispCnt++;
	DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "�ǔ����̃L�����F%s", const_cast<char*>(chaseTarget->name.c_str()));
	Debug_DispCnt++;
	DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "���W:X/%.1f, Y/%.1f, Z/%.1f", position.x, position.y, position.z);
	Debug_DispCnt++;
	DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "��]:H/%.1f,V/%.1f", Math::RadToDeg(rotH), Math::RadToDeg(rotV));
	Debug_DispCnt++;*/
}
