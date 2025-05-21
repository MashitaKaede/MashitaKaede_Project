#include "Keypad.h"

Keypad::Keypad(int Handle, int KeypadIndex)

	:inputRate(0.0f),
	 inputComplete(false)
{
	modelHandle = Handle;
	index = KeypadIndex;
}

void Keypad::Init()
{
	tag = "Keypad";
	name = "Keypad";
	scale = VGet(1.0f, 1.0f, 1.0f);
	Transform(modelHandle);

	startPos = position;
	startPos.y -= KeyPad_ColliderHeight;
	endPos = position;
	endPos.y += KeyPad_ColliderHeight;
	radius = KeyPad_ColliderRadius;
	SetParent(this);
}

void Keypad::Term()
{
}

void Keypad::Update()
{
	Draw();
}

void Keypad::Draw()
{
	MV1DrawModel(modelHandle);
}

void Keypad::Debug()
{
	DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "");
	Debug_DispCnt++;
	DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "%s:%d", const_cast<char*>(name.c_str()), index + 1);
	Debug_DispCnt++;
	DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "“ü—Í—¦/%.1f", inputRate);
	Debug_DispCnt++;
}

void Keypad::InputPassward(float AddInputRate)
{
	inputRate += AddInputRate;
	if (inputRate >= 100.0f)
	{
		inputRate = 100.0f;
		inputComplete = true;
	}
}

void Keypad::OnCollisionHit(Collider* Collider, GameObject* Parent)
{
}
