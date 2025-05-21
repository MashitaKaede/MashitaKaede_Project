#pragma once
#include "DxLib.h"
#include "Animation.h"

enum class AnimName_Survivor
{
	Idle,
	Run,
	Decoding,
	Input,
	Rescue,
	Restrainting,
	FallIdle,
	FallWalk,
	AttackHit
};

enum class AnimName_Hunter
{
	Idle,
	Walk,
	Attack,
	AttackHit,
	PickUp,
	CarryWalk,
	Restrain
};

class AnimationList
{
private:
	std::vector<Animation> animList_Hunter;
	std::vector<Animation> animList_Survivor;
public:
	AnimationList();
	~AnimationList();
	void SetAnimList();
	inline std::vector<Animation> GetAnimList_Hunter() { return animList_Hunter; }
	inline std::vector<Animation> GetAnimList_Survivor() { return animList_Survivor; }
};

