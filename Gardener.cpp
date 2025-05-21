#include "Gardener.h"

Gardener::Gardener()
{
	animator = new Animator();
}

Gardener::~Gardener()
{
	delete animator;
}

void Gardener::Init()
{
	modelHandle = MV1LoadModel("Assets/Model/Character/Survivor/Gardener.mv1");
	name = "’ëŽt";
	tag = "Survivor";

	radius = Character_Default_ColliderRadius;
	UpdateCollider();
	SetParent(this);

	defaultMoveSpeed = Survivor_Default_MoveSpeed;

	animator->SetModelHandle(modelHandle);
	animator->LoadAnim(CharacterName::Survivor);
	animator->PlayAnim(0);
}

void Gardener::Term()
{
}

void Gardener::Update()
{
	DefaultSurvivorUpdate();
}