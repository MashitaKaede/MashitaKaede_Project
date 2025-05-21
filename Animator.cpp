#include "Animator.h"

Animator::Animator()
	:playAnimIndex(-1)
{
}

void Animator::LoadAnim(CharacterName Name)
{
	switch (Name)
	{
	case CharacterName::Hunter:
		animVec = animList->GetAnimList_Hunter();
		break;
	case CharacterName::Survivor:
		animVec = animList->GetAnimList_Survivor();
		break;
	default:
		break;
	}

	animNum = (int)animVec.size();
}

void Animator::PlayAnim(int AnimIndex)
{
	// 番号が範囲外だったら無視
	if (AnimIndex < 0 || AnimIndex >= animNum)
	{
		return;
	}

	// 最初だったらそのまま再生
	if (playAnimIndex == -1)
	{
		state = AnimatorState::PlayAnim;
		playAnimIndex = AnimIndex;
		attachIndex = MV1AttachAnim(modelHandle, 0, animVec[AnimIndex].animHandle, false);
		totalTime = MV1GetAttachAnimTotalTime(modelHandle, attachIndex);
		playTime = 0.0f;
		return;
	}

	if (animVec[playAnimIndex].playEnd == true)
	{
		if (playEnd == false)
		{
			return;
		}
	}

	// モーションブレンド中だったら
	if (state == AnimatorState::BlendAnim)
	{
		// 同じ番号でなければもう一度ブレンド
		if (AnimIndex != playAnimIndex && animVec[playAnimIndex].playEnd == false)
		{
			playEnd = false;
			MV1DetachAnim(modelHandle, attachIndex_Prev);
			playAnimIndex = AnimIndex;
			attachIndex_Prev = attachIndex;
			attachIndex = MV1AttachAnim(modelHandle, 0, animVec[AnimIndex].animHandle, false);
			return;
		}
	}
	else if(state == AnimatorState::PlayAnim)
	{
		// 同じ番号なら続けて再生
		if (AnimIndex == playAnimIndex)
		{
			playTime = fmodf(playTime, totalTime);
			return;
		}
		// 違う番号だったらモーションブレンド準備
		else
		{
			playEnd = false;
			state = AnimatorState::BlendAnim;
			playAnimIndex = AnimIndex;
			attachIndex_Prev = attachIndex;
			attachIndex = MV1AttachAnim(modelHandle, 0, animVec[AnimIndex].animHandle, false);
			blendRate = 0.0f;
		}
	}
}

void Animator::Update()
{
	switch (state)
	{
	case AnimatorState::PlayAnim:
		PlayAnim();
		break;
	case AnimatorState::BlendAnim:
		BlendAnim();
		break;
	default:
		break;
	}
	
}

void Animator::PlayAnim()
{
	if (playTime >= totalTime)
	{
		if (animVec[playAnimIndex].loop == true)
		{
			PlayAnim(playAnimIndex);
			playEnd = true;
		}
		else
		{
			PlayAnim(0);
			playEnd = true;
		}
	}

	playTime += Animation_Default_PlaySpeed * FrameTime;

	MV1SetAttachAnimTime(modelHandle, attachIndex, playTime);
}

void Animator::BlendAnim()
{
	if (blendRate >= 1.0f)
	{
		state = AnimatorState::PlayAnim;
		MV1DetachAnim(modelHandle, attachIndex_Prev);
		playTime = 0.0f;
		totalTime = MV1GetAttachAnimTotalTime(modelHandle, attachIndex);
	}

	blendRate += Animation_BlendRateSpeed * FrameTime;
	MV1SetAttachAnimBlendRate(modelHandle, attachIndex, blendRate);
	MV1SetAttachAnimBlendRate(modelHandle, attachIndex_Prev, 1.0f - blendRate);
}
