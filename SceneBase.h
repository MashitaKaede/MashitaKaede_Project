#pragma once
#include "DxLib.h"

class SceneBase
{
private:
public:
	virtual void Init() = 0;
	virtual void Term() = 0;
	virtual void Update() = 0;
	virtual void Debug() = 0;
};

