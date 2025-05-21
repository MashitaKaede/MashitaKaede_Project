#include "Scene_Title.h"

Scene_Title::Scene_Title()
{

}

Scene_Title::~Scene_Title()
{

}

void Scene_Title::Init()
{
	SoundManager::GetInstance()->PlayBGM(BGM_Name::Title);
}

void Scene_Title::Term()
{
	SoundManager::GetInstance()->StopBGM(BGM_Name::Title);
}

void Scene_Title::Update()
{
	DrawGraph(0, 0, GameManager::GetInstance()->GetImageHandle(UI_Name::Title), true);
	if (Input::GetInstance()->GetKyeDown(Return) || Input::GetInstance()->GetButtonDown(ButtonName::Button_Start))
	{
		SceneManager::GetInstance()->LoadScene(SceneName::SideSelect);
	}
}

void Scene_Title::Debug()
{

}