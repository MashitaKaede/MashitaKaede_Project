#include "Door.h"

Door::Door(int Handle)
{
	modelHandle = Handle;
}

void Door::Init()
{
	scale = VGet(1.0f, 1.5f, 1.5f);
	Transform(modelHandle);
}

void Door::Term()
{
	
}

void Door::Update()
{
	Draw();
}

void Door::OpenDoor(VECTOR moveVec)
{
	position = VAdd(position, moveVec);
	Transform(modelHandle);
}

void Door::Draw()
{
	MV1DrawModel(modelHandle);
}

void Door::Deubg()
{
}
