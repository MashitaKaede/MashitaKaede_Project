#include "CipherMachine.h"

CipherMachine::CipherMachine(int Handle, int CipherMachineIndex)

	: decodingRate(0.0f),
	  decodingComplete(false)
{
	modelHandle = Handle;
	index = CipherMachineIndex;
}

void CipherMachine::Init()
{
	tag = "CipherMachine";
	name = "CipherMachine";
	rotation = VGet(0.0f, DX_PI_F, 0.0f);
	scale = VGet(1.2f, 1.2f, 1.2f);
	Transform(modelHandle);

	startPos = position;
	endPos = startPos;
	endPos.y += CipherMachine_ColliderHeight;
	radius = CipherMachine_ColliderRadius;
	SetParent(this);

	decodingPos = {
		{VAdd(position,VScale(forward, -110.0f)), false, 0},
		{VAdd(position,VScale(right, 110.0f)), false , 1},
		{VAdd(position,VScale(right, -110.0f)), false, 2},
		{VAdd(position,VScale(forward, 110.0f)), false, 3},
	};
}

void CipherMachine::Term()
{

}

void CipherMachine::Update()
{
	Draw();
	if (decodingComplete == true)
	{
		int materialNum = MV1GetMaterialNum(modelHandle);

		for (int i = 0; i < materialNum; i++)
		{
			MV1SetMaterialDifColor(modelHandle, i, GetColorF(1.0f, 1.0f, 0.0f, 1.0f));
		}
	}
}

void CipherMachine::Draw()
{
	MV1DrawModel(modelHandle);
	/*DrawLine3D(position, VGet(position.x, position.y + 300.0f, position.z), GetColor(0, 255, 0));
	DrawLine3D(VGet(position.x, position.y + 300.0f, position.z), VAdd(VGet(position.x, position.y + 300.0f, position.z), VScale(right, 100.0f)), GetColor(0, 255, 0));
	DrawLine3D(VGet(position.x, position.y + 300.0f, position.z), VAdd(VGet(position.x, position.y + 300.0f, position.z), VScale(right, -100.0f)), GetColor(0, 255, 0));
	DrawLine3D(VGet(position.x, position.y + 300.0f, position.z), VAdd(VGet(position.x, position.y + 300.0f, position.z), VScale(forward, 100.0f)), GetColor(0, 255, 0));
	DrawLine3D(VGet(position.x, position.y + 300.0f, position.z), VAdd(VGet(position.x, position.y + 300.0f, position.z), VScale(forward, -100.0f)), GetColor(0, 255, 0));*/
}

void CipherMachine::Decoding(float AddDecodingRate)
{
	decodingRate += AddDecodingRate;
	if (decodingRate >= 100.0f)
	{
		decodingRate = 100.0f;
		decodingComplete = true;
	}
}

void CipherMachine::Debug()
{
	DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "");
	Debug_DispCnt++;
	DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "%s:%d", const_cast<char*>(name.c_str()), index + 1);
	Debug_DispCnt++;
	DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "‰ð“Ç—¦/%.1f", decodingRate);
	Debug_DispCnt++;
}

void CipherMachine::OnCollisionHit(Collider* Collider, GameObject* Parent)
{
}

void CipherMachine::LeaveDecodeingPos(int Index)
{

	decodingPos[Index].used = false;
}

DecodingPos CipherMachine::GetDecodingPos()
{
	DecodingPos decPos = decodingPos[0];
	for (int i = 0; i < 4; i++)
	{
		if (decodingPos[i].used == false)
		{
			decodingPos[i].used = true;
			decPos = decodingPos[i];
			break;
		}
	}
	return decPos;
}
