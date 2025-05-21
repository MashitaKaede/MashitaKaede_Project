#pragma once
#include "DxLib.h"
#include "Define.h"
#include <vector>
#include <numeric>

class Time
{
private:
	std::vector<int> frameTimeVec;
	int prevTime = 0;
	int index = 0;
	bool stop;
public:
	Time();
	~Time();
	void Update();
	void Debug();
	inline void SetStop(bool flag) { stop = flag; prevTime = 0; }
};