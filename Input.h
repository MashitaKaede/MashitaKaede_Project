#pragma once
#include "DxLib.h"
#include "Define.h"
#include <vector>
#include "Math.h"

// キーボード
enum KeyName
{
	W,
	A,
	S,
	D,
	Up,
	Left,
	Down,
	Right,
	L_Shift,
	Space,
	Return,
	KeyNum,
};

// Xbox
enum ButtonName
{
	Button_B,
	Button_A,
	Button_X,
	Button_Y,
	Button_L,
	Button_R,
	Button_Left,
	Button_Right,
	Button_Up,
	Button_Down,
	Button_Start,
	Button_Back,
	ButtonNum,
};

class Input
{
private:
	static Input* instance;

	// キーボード
	int keyNum;
	std::vector<int> keyCodeList;
	std::vector<int> keyInputInfo_Now;
	std::vector<int> keyInputInfo_Prev;

	// Xbox
	int buttonNum;
	XINPUT_STATE xInput;
	std::vector<int> buttonCodeList;
	std::vector<int> buttonInputInfo_Now;
	std::vector<int> buttonInputInfo_Prev;
	float stickAngle_L;
	float stickAngle_R;

	Input();
	~Input();
public:
	static inline Input* GetInstance() { return instance; }
	static void Init();
	static void Term();
	void Update();
	void Debug();

	// キーボード
	void KeyInfoInit();
	int GetKyeDown(KeyName Key);
	int GetKye(KeyName Key);
	int GetKyeUp(KeyName Key);

	// Xbox
	void ButtonInfoInit();
	int GetButtonDown(ButtonName Button);
	int GetButton(ButtonName Button);
	int GetButtonUp(ButtonName Button);
	bool GetStickInput(bool L, bool R);
	inline XINPUT_STATE GetXInputState() { return xInput; }
	inline float GetStickAngle_L() { return stickAngle_L; }
	inline float GetStickAngle_R() { return stickAngle_R; }
};

