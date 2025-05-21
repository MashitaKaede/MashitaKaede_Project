#include "AnimationList.h"

AnimationList::AnimationList()
{
	SetAnimList();
}

AnimationList::~AnimationList()
{
	int animNum_Survivor = (int)animList_Survivor.size();
	for (int i = 0; i < animNum_Survivor; i++)
	{
		MV1DeleteModel(animList_Survivor[i].animHandle);
	}

	int animNum_Hunter = (int)animList_Hunter.size();
	for (int i = 0; i < animNum_Hunter; i++)
	{
		MV1DeleteModel(animList_Hunter[i].animHandle);
	}
}

void AnimationList::SetAnimList()
{
	// サバイバー
	animList_Survivor = {

		{"Assets/Animation/Survivor/Idle.mv1" , -1, true, false},
		{"Assets/Animation/Survivor/Run.mv1" , -1, false, false},
		{"Assets/Animation/Survivor/Decoding.mv1" , -1, false, false},
		{"Assets/Animation/Survivor/Input.mv1" , -1, false, false},
		{"Assets/Animation/Survivor/Rescue.mv1" , -1, false, false},
		{"Assets/Animation/Survivor/Restrainting.mv1" , -1, false, false},
		{"Assets/Animation/Survivor/FallIdle.mv1" , -1, true, false},
		{"Assets/Animation/Survivor/FallWalk.mv1" , -1, false, false},
		{"Assets/Animation/Survivor/AttackHit.mv1" , -1, false, true},
	};

	int animNum_Survivor = (int)animList_Survivor.size();

	for (int i = 0; i < animNum_Survivor; i++)
	{
		animList_Survivor[i].animHandle = MV1LoadModel(animList_Survivor[i].fileName.c_str());
	}

	// ハンター
	animList_Hunter = {
		{"Assets/Animation/Hunter/Idle.mv1", -1, true, false},
		{"Assets/Animation/Hunter/Walk.mv1", -1, true, false},
		{"Assets/Animation/Hunter/Attack.mv1", -1, true, true},
		{"Assets/Animation/Hunter/AttackHit.mv1", -1, true, true},
		{"Assets/Animation/Hunter/PickUp.mv1", -1, true, true},
		{"Assets/Animation/Hunter/CarryWalk.mv1", -1, true, false},
		{"Assets/Animation/Hunter/Restrain.mv1", -1, true, true},
	};

	int animNum_Hunter = (int)animList_Hunter.size();

	for (int i = 0; i < animNum_Hunter; i++)
	{
		animList_Hunter[i].animHandle = MV1LoadModel(animList_Hunter[i].fileName.c_str());
	}
}