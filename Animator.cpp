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
	// �ԍ����͈͊O�������疳��
	if (AnimIndex < 0 || AnimIndex >= animNum)
	{
		return;
	}

	// �ŏ��������炻�̂܂܍Đ�
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

	// ���[�V�����u�����h����������
	if (state == AnimatorState::BlendAnim)
	{
		// �����ԍ��łȂ���΂�����x�u�����h
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
		// �����ԍ��Ȃ瑱���čĐ�
		if (AnimIndex == playAnimIndex)
		{
			playTime = fmodf(playTime, totalTime);
			return;
		}
		// �Ⴄ�ԍ��������烂�[�V�����u�����h����
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
