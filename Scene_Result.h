#pragma once
#include "SceneManager.h"
#include "GameManager.h"
#include "SoundManager.h"

class Scene_Result : public SceneBase
{
private:
	
public:
	Scene_Result();
	~Scene_Result();
	void Init();
	void Term();
	void Update();
	void Debug();
};

