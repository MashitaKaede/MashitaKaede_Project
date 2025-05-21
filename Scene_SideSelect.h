#pragma once
#include "SceneManager.h"
#include "GameManager.h"
#include "SoundManager.h"

class Scene_SideSelect : public SceneBase
{
private:
	int side;
	int survivorType;
	bool decideSide;
	
public:
	Scene_SideSelect();
	~Scene_SideSelect();
	void Init();
	void Term();
	void Update();
	void Debug();
	void SideSelect();
	void SurvivorTypeSelect();
};

