#include "GameManager.h"

GameManager* GameManager::instance = nullptr;

GameManager::GameManager()
{
}

GameManager::~GameManager()
{
}

void GameManager::Init()
{
	if (instance != nullptr)
	{
		return;
	}

	instance = new GameManager();
}

void GameManager::Term()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}

	InitGraph();
}

void GameManager::LoadImages()
{
	imageHandleVec = {
		LoadGraph("Assets/Image/Title.png"),
		LoadGraph("Assets/Image/Button_B.png"),
		LoadGraph("Assets/Image/BaseIcon.png"),
		LoadGraph("Assets/Image/InjuryIcon.png"),
		LoadGraph("Assets/Image/DownIcon.png"),
		LoadGraph("Assets/Image/RestraintIcon.png"),
		LoadGraph("Assets/Image/Injury.png"),
		LoadGraph("Assets/Image/Down.png"),
		LoadGraph("Assets/Image/Restraint.png"),
		LoadGraph("Assets/Image/Carry.png"),
		LoadGraph("Assets/Image/Death.png"),
		LoadGraph("Assets/Image/Escape.png"),
		LoadGraph("Assets/Image/Hunter.png"),
		LoadGraph("Assets/Image/Gardener.png"),
		LoadGraph("Assets/Image/Kunoichi.png"),
		LoadGraph("Assets/Image/Lawyer.png"),
		LoadGraph("Assets/Image/Offense.png"),
	};
}
