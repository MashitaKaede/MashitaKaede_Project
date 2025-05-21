#include "Offense.h"

Offense::Offense()
{
	animator = new Animator();
}

Offense::~Offense()
{
	delete animator;
}

void Offense::Init()
{
	modelHandle = MV1LoadModel("Assets/Model/Character/Survivor/Offense.mv1");
	name = "オフェンス";
	tag = "Survivor";

	radius = Character_Default_ColliderRadius;
	UpdateCollider();
	SetParent(this);

	defaultMoveSpeed = Survivor_Default_MoveSpeed;

	animator->SetModelHandle(modelHandle);
	animator->LoadAnim(CharacterName::Survivor);
	animator->PlayAnim(0);
}

void Offense::Term()
{
}

void Offense::Update()
{
	DefaultSurvivorUpdate();
}