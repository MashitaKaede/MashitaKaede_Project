#pragma once
#include "SceneManager.h"
#include "GameManager.h"
#include "SoundManager.h"
#include "CollisionManager.h"
#include "Map.h"
#include "Camera.h"
#include "Gardener.h"
#include "Kunoichi.h"
#include "Lawyer.h"
#include "Offense.h"
#include "Hunter.h"



class Scene_Main : public SceneBase
{
private:
	CollisionManager* collisionManager;
	AnimationList* animList;
	std::vector<Survivor*> survivorVec;
	Hunter* hunter;
	Camera* camera;
	Map* map;
	int survivorNum;
	
	bool drawCharState;
public:
	Scene_Main();
	~Scene_Main();
	void Init();
	void Term();
	void Update();
	void Debug();
	void CheckGameEnd();
	void UI();
	void DrawCharStateForMovie();
};

