#include "Kunoichi.h"

Kunoichi::Kunoichi()
{
	animator = new Animator();
}

Kunoichi::~Kunoichi()
{
	delete animator;
}

void Kunoichi::Init()
{
	modelHandle = MV1LoadModel("Assets/Model/Character/Survivor/Kunoichi.mv1");
	name = "‚­‚Ì‚¢‚¿";
	tag = "Survivor";

	radius = Character_Default_ColliderRadius;
	UpdateCollider();
	SetParent(this);

	defaultMoveSpeed = Survivor_Default_MoveSpeed;

	animator->SetModelHandle(modelHandle);
	animator->LoadAnim(CharacterName::Survivor);
	animator->PlayAnim(0);
}

void Kunoichi::Term()
{
}

void Kunoichi::Update()
{
	DefaultSurvivorUpdate();
}