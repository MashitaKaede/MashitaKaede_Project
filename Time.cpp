#include "Time.h"

Time::Time()

	: prevTime(0),
	  index(0),
	  frameTimeVec(std::vector<int>(Time_CheckFrameCnt, 1000.0f / Time_DefaultFps))
{
}

Time::~Time()
{

}

void Time::Update()
{
	if (stop == false)
	{
		int nowTime = GetNowCount();
		if (prevTime != 0)
		{
			FrameTime = (float)frameTimeVec[index] / 1000.0f;
			frameTimeVec[index] = nowTime - prevTime;

			int averageTime = accumulate(frameTimeVec.begin(), frameTimeVec.end(), 0) / Time_CheckFrameCnt;
			FPS = 1000.0f / (float)averageTime;
			//FrameTime = (float)averageTime / 1000.0f;
			index++;
			if (index >= Time_CheckFrameCnt) { index = 0; }
		}
		prevTime = nowTime;
	}
}

void Time::Debug()
{
	DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "FPS : %.1f", FPS);
	Debug_DispCnt++;
	DrawFormatString(0, Debug_Dis_Y * Debug_DispCnt, GetColor(0, 0, 0), "FrameTime : %.3f", FrameTime);
	Debug_DispCnt++;
}