#pragma once
#include "DxLib.h"
#include <vector>
#include <string>
#include "Define.h"
#include "AnimationList.h"

enum class CharacterName
{
	Hunter,
	Survivor,
};

enum class AnimatorState
{
	PlayAnim,
	BlendAnim,
};

class Animator
{
private:
	int modelHandle;
	int animNum;
	AnimatorState state;
	AnimationList* animList;
	std::vector<Animation> animVec;

	int playAnimIndex;
	int attachIndex;
	float totalTime;
	float playTime;

	int attachIndex_Prev;
	float totalTime_Prev;

	float blendRate;

	bool playEnd;

	void PlayAnim();
	void BlendAnim();
public:
	Animator();
	void LoadAnim(CharacterName Name);
	void PlayAnim(int AnimIndex);
	void Update();
	inline void SetModelHandle(int ModelHandle) { modelHandle = ModelHandle; }
	inline void SetAnimList(AnimationList* AnimList) { animList = AnimList; }
	inline bool GetPlayEnd() { return playEnd; }
	inline float GetTotalTime() { return totalTime; }
	inline float GetPlayTime() { return playTime; }
	inline float GetPlayAnimRate() { return (playTime / totalTime); }
	inline int GetPlayIndex() { return playAnimIndex; }
};

