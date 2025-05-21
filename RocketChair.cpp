#include "RocketChair.h"
#include "Survivor.h"
RocketChair::RocketChair(int Handle, int RocketChairIndex)

	:index(-1)
{
	modelHandle = Handle;
	index = RocketChairIndex;
}

void RocketChair::Init()
{
	tag = "RocketChair";
	name = "RocketChair";
	rotation = VGet(0.0f, 0.0f, 0.0f);
	scale = VGet(1.2f, 1.4f, 1.2f);
	Transform(modelHandle);

	startPos = position;
	endPos = startPos;
	endPos.y += RocketChair_ColliderHeight;
	radius = RocketChair_ColliderRadius;
	SetParent(this);
}

void RocketChair::Term()
{
}

void RocketChair::Update()
{
	Draw();
	if (restrainSurvivor != nullptr)
	{
		int materialNum = MV1GetMaterialNum(modelHandle);

		for (int i = 0; i < materialNum; i++)
		{
			MV1SetMaterialDifColor(modelHandle, i, GetColorF(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	else
	{
		int materialNum = MV1GetMaterialNum(modelHandle);

		for (int i = 0; i < materialNum; i++)
		{
			MV1SetMaterialDifColor(modelHandle, i, GetColorF(0.1f, 0.1f, 0.1f, 1.0f));
		}
	}
}

void RocketChair::Draw()
{
	MV1DrawModel(modelHandle);
}

void RocketChair::Debug()
{
}

void RocketChair::AddReleseRate(float AddRate)
{
	if (releaseRate < 0.0f)
	{
		releaseRate = 0.0f;
	}

	releaseRate += AddRate;

	if (releaseRate >= 100.0f)
	{
		releaseRate = 0.0f;
		used = false;
		restrainSurvivor->SetIsRescuing(false);
		restrainSurvivor->AddHP(2);
		restrainSurvivor->Reset();
		restrainSurvivor = nullptr;
	}
}

void RocketChair::UpdateCollider()
{
	startPos = position;
	endPos = startPos;
	endPos.y += RocketChair_ColliderHeight;
}
