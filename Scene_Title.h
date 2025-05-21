#pragma once
#include "SceneManager.h"
#include "SoundManager.h"
#include "GameManager.h"

class Scene_Title : public SceneBase
{
private:

public:
	Scene_Title();
	~Scene_Title();
	void Init();
	void Term();
	void Update();
	void Debug();
};

