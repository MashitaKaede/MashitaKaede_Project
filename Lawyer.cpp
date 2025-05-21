#include "Lawyer.h"

Lawyer::Lawyer()
{
	animator = new Animator();
}

Lawyer::~Lawyer()
{
	delete animator;
}

void Lawyer::Init()
{
	modelHandle = MV1LoadModel("Assets/Model/Character/Survivor/Lawyer.mv1");
	name = "•ÙŒìŽm";
	tag = "Survivor";

	radius = Character_Default_ColliderRadius;
	UpdateCollider();
	SetParent(this);

	defaultMoveSpeed = Survivor_Default_MoveSpeed;

	animator->SetModelHandle(modelHandle);
	animator->LoadAnim(CharacterName::Survivor);
	animator->PlayAnim(0);
}

void Lawyer::Term()
{
}

void Lawyer::Update()
{
	DefaultSurvivorUpdate();
}