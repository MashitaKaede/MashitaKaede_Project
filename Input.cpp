#include "Input.h"

Input* Input::instance = nullptr;

Input::Input()

	: keyNum(0),
	  buttonNum(0),
	  xInput({}),
	  stickAngle_L(0.0f),
	  stickAngle_R(0.0f)

{
	
}

Input::~Input()
{

}

void Input::Init()
{
	if (instance != nullptr)
	{
		return;
	}

	instance = new Input();
}

void Input::Term()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}

void Input::Update()
{	
	// キーボード入力更新
	keyInputInfo_Prev = keyInputInfo_Now;
	for (int i = 0; i < keyNum; i++)
	{
		if (CheckHitKey(keyCodeList[i]))
		{
			keyInputInfo_Now[i] = true;
		}
		else
		{
			keyInputInfo_Now[i] = false;
		}
	}

	// Xbox入力更新
	GetJoypadXInputState(DX_INPUT_PAD1, &xInput);
	buttonInputInfo_Prev = buttonInputInfo_Now;
	for (int i = 0; i < buttonNum; i++)
	{
		if (xInput.Buttons[buttonCodeList[i]])
		{
			buttonInputInfo_Now[i] = true;
		}
		else
		{
			buttonInputInfo_Now[i] = false;
		}
	}

	stickAngle_L = atan2f((float)xInput.ThumbLY, (float)xInput.ThumbLX);
	stickAngle_R = atan2f((float)xInput.ThumbRY, (float)xInput.ThumbRX);
}

void Input::KeyInfoInit()
{
	keyCodeList = {
		KEY_INPUT_W,
		KEY_INPUT_A,
		KEY_INPUT_S,
		KEY_INPUT_D,
		KEY_INPUT_UP,
		KEY_INPUT_LEFT,
		KEY_INPUT_DOWN,
		KEY_INPUT_RIGHT,
		KEY_INPUT_LSHIFT,
		KEY_INPUT_SPACE,
		KEY_INPUT_RETURN,
	};
	keyNum = (int)KeyName::KeyNum;
	keyInputInfo_Now = std::vector<int>(keyNum, 0);
	keyInputInfo_Prev = std::vector<int>(keyNum, 0);
}

int Input::GetKyeDown(KeyName Key)
{
	int keyNum = (int)Key;
	
	if (keyInputInfo_Now[keyNum] && !keyInputInfo_Prev[keyNum])
	{
		return 1;
	}
	return 0;
}

int Input::GetKye(KeyName Key)
{
	int keyNum = (int)Key;

	if (keyInputInfo_Now[keyNum] && keyInputInfo_Prev[keyNum])
	{
		return 1;
	}
	return 0;
}

int Input::GetKyeUp(KeyName Key)
{
	int keyNum = (int)Key;

	if (!keyInputInfo_Now[keyNum] && keyInputInfo_Prev[keyNum])
	{
		return 1;
	}
	return 0;
}

void Input::ButtonInfoInit()
{
	buttonCodeList = {
		XINPUT_BUTTON_B,
		XINPUT_BUTTON_A,
		XINPUT_BUTTON_X,
		XINPUT_BUTTON_Y,
		XINPUT_BUTTON_LEFT_SHOULDER,
		XINPUT_BUTTON_RIGHT_SHOULDER,
		XINPUT_BUTTON_DPAD_LEFT,
		XINPUT_BUTTON_DPAD_RIGHT,
		XINPUT_BUTTON_DPAD_UP,
		XINPUT_BUTTON_DPAD_DOWN,
		XINPUT_BUTTON_START,
		XINPUT_BUTTON_BACK

	};
	buttonNum = (int)ButtonName::ButtonNum;
	buttonInputInfo_Now = std::vector<int>(buttonNum, 0);
	buttonInputInfo_Prev = std::vector<int>(buttonNum, 0);
}

int Input::GetButtonDown(ButtonName Button)
{
	int buttonNum = (int)Button;

	if (buttonInputInfo_Now[buttonNum] && !buttonInputInfo_Prev[buttonNum])
	{
		return 1;
	}
	return 0;
}

int Input::GetButton(ButtonName Button)
{
	int buttonNum = (int)Button;

	if (buttonInputInfo_Now[buttonNum] && buttonInputInfo_Prev[buttonNum])
	{
		return 1;
	}
	return 0;
}

int Input::GetButtonUp(ButtonName Button)
{
	int buttonNum = (int)Button;

	if (!buttonInputInfo_Now[buttonNum] && buttonInputInfo_Prev[buttonNum])
	{
		return 1;
	}
	return 0;
}

bool Input::GetStickInput(bool L, bool R)
{

	if (L && !R)
	{
		if (xInput.ThumbLX > XBox_Invalid_Range || xInput.ThumbLX < -XBox_Invalid_Range || xInput.ThumbLY > XBox_Invalid_Range || xInput.ThumbLY < -XBox_Invalid_Range)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (!L && R)
	{
		if ((xInput.ThumbRX != 0) || (xInput.ThumbRY != 0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

void Input::Debug()
{
	DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "");
	Debug_DispCnt++;
	DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "入力");
	Debug_DispCnt++;
	DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "SthickAngle_L / %.1f", Math::RadToDeg(stickAngle_L));
	Debug_DispCnt++;
	DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "SthickAngle_R / %.1f", Math::RadToDeg(stickAngle_R));
	Debug_DispCnt++;
}