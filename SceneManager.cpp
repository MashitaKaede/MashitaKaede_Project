#include "SceneManager.h"

SceneManager* SceneManager::instance = nullptr;

SceneManager::SceneManager()
{

}

SceneManager::~SceneManager()
{

}

void SceneManager::Init()
{
	if (instance != nullptr) 
	{
		return;
	}

	instance = new SceneManager();
}

void SceneManager::LoadScene(SceneName LoadSceneName)
{
	time->SetStop(true);

	if (nowScene != nullptr)
	{
		nowScene->Term();
		delete nowScene;
	}

	SceneBase* scene = nullptr;
	
	if (LoadSceneName == Title)
	{
		scene = new Scene_Title();
	}
	else if (LoadSceneName == SideSelect)
	{
		scene = new Scene_SideSelect();
	}
	else if (LoadSceneName == Main)
	{
		scene = new Scene_Main();
	}
	else if (LoadSceneName == Result)
	{
		scene = new Scene_Result();
	}

	if (LoadSceneName == Main)
	{
		SetBackgroundColor(200, 200, 200);
	}
	else
	{
		SetBackgroundColor(200, 200, 200);
	}

	if (scene != nullptr)
	{
		scene->Init();
	}

	nowScene = scene;

	time->SetStop(false);
}

void SceneManager::Term()
{
	if (instance) 
	{
		delete instance;
		instance = nullptr;
	}
}


void SceneManager::Update()
{
	if (nowScene != nullptr)
	{
		nowScene->Update();
	}
	else
	{
		DrawFormatString(300, 300, GetColor(255, 0, 0), "---ƒV[ƒ“‚ª‚ ‚è‚Ü‚¹‚ñ---");
	}
}

void SceneManager::Debug()
{
	if (nowScene != nullptr)
	{
		nowScene->Debug();
	}
}