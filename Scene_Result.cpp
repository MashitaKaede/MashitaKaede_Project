#include "Scene_Result.h"

Scene_Result::Scene_Result()
{

}

Scene_Result::~Scene_Result()
{

}

void Scene_Result::Init()
{
	SoundManager::GetInstance()->PlayBGM(BGM_Name::Result);
}

void Scene_Result::Term()
{
	SoundManager::GetInstance()->StopBGM(BGM_Name::Result);
}

void Scene_Result::Update()
{
	SetFontSize(50);

	int escapeNum = 0;
	for (int i = 0; i < 4; i++)
	{
		if (GameManager::GetInstance()->GetSurvivorEscapeFlagVec().size() > 0)
		{
			if (GameManager::GetInstance()->GetSurvivorEscapeFlagVec()[i] == true)
			{
				escapeNum++;
				if (i == 0)
				{
					DrawFormatString(1920 / 2 - 450 + i * 300 - GetDrawFormatStringWidth("脱出") / 2, 550, GetColor(0, 0, 0), "脱出");
					
				}
				else if(i == 1)
				{
					DrawFormatString(1920 / 2 - 450 + i * 300 - GetDrawFormatStringWidth("脱出") / 8 * 3, 550, GetColor(0, 0, 0), "脱出");
				}
				else if (i == 2)
				{
					DrawFormatString(1920 / 2 - 450 + i * 300 + GetDrawFormatStringWidth("脱出") / 8 * 3, 550, GetColor(0, 0, 0), "脱出");
				}
				else if (i == 3)
				{
					DrawFormatString(1920 / 2 - 450 + i * 300 + GetDrawFormatStringWidth("脱出") / 8 * 6, 550, GetColor(0, 0, 0), "脱出");
				}
				
			}
			else
			{
				if (i == 0)
				{
					DrawFormatString(1920 / 2 - 450 + i * 300 - GetDrawFormatStringWidth("死亡") / 2, 550, GetColor(0, 0, 0), "死亡");

				}
				else if (i == 1)
				{
					DrawFormatString(1920 / 2 - 450 + i * 300 - GetDrawFormatStringWidth("死亡") / 8 * 3, 550, GetColor(0, 0, 0), "死亡");
				}
				else if (i == 2)
				{
					DrawFormatString(1920 / 2 - 450 + i * 300 + GetDrawFormatStringWidth("死亡") / 8 * 3, 550, GetColor(0, 0, 0), "死亡");
				}
				else if (i == 3)
				{
					DrawFormatString(1920 / 2 - 450 + i * 300 + GetDrawFormatStringWidth("死亡") / 8 * 6, 550, GetColor(0, 0, 0), "死亡");
				}
			}
		}
	}

	DrawFormatString(1920 / 2 + 430, 230, GetColor(0, 0, 0), "脱出人数 : %d人", escapeNum);

	DrawGraph(1920 / 2 - 128 / 2, 220, GameManager::GetInstance()->GetImageHandle(UI_Name::HunterIcon), true);

	DrawFormatString(1920 / 2 - 450 - GetDrawFormatStringWidth("庭師") / 2, 750, GetColor(0, 0, 0), "庭師");
	DrawFormatString(1920 / 2 - 150 - GetDrawFormatStringWidth("くのいち") / 2, 750, GetColor(0, 0, 0), "くのいち");
	DrawFormatString(1920 / 2 + 150, 750, GetColor(0, 0, 0), "弁護士");
	DrawFormatString(1920 / 2 + 450, 750, GetColor(0, 0, 0), "オフェンス");
	DrawGraph(1920 / 2 - 450 - GetDrawFormatStringWidth("庭師") / 8 * 5, 600, GameManager::GetInstance()->GetImageHandle(UI_Name::GardenerIcon), true);
	DrawGraph(1920 / 2 - 150 - GetDrawFormatStringWidth("くのいち") / 8 * 2, 600, GameManager::GetInstance()->GetImageHandle(UI_Name::KunoichiIcon), true);
	DrawGraph(1920 / 2 + 150 + GetDrawFormatStringWidth("弁護士") / 8, 600, GameManager::GetInstance()->GetImageHandle(UI_Name::LawyerIcon), true);
	DrawGraph(1920 / 2 + 450 + GetDrawFormatStringWidth("オフェンス") / 8 * 2, 600, GameManager::GetInstance()->GetImageHandle(UI_Name::OffenseIcon), true);

	SetFontSize(100);
	if (GameManager::GetInstance()->GetWinSide() == Side::Hunter)
	{
		DrawFormatString(1920 / 2 - GetDrawFormatStringWidth("ハンターの勝利") / 2, 100, GetColor(0, 0, 0), "ハンターの勝利");
	}
	else if (GameManager::GetInstance()->GetWinSide() == Side::Survivor)
	{
		DrawFormatString(1920 / 2 - GetDrawFormatStringWidth("サバイバーの勝利") / 2, 100, GetColor(0, 0, 0), "サバイバーの勝利");
	}
	else if (GameManager::GetInstance()->GetWinSide() == Side::None)
	{
		DrawFormatString(1920 / 2 - GetDrawFormatStringWidth("引き分け") / 2, 100, GetColor(0, 0, 0), "引き分け");
	}
	
	SetFontSize(50);
	DrawFormatString(1920 / 2 - GetDrawFormatStringWidth("スタートボタンでタイトルへ") / 2, 900, GetColor(0, 0, 0), "スタートボタンでタイトルへ");

	if (Input::GetInstance()->GetKyeDown(Return) || Input::GetInstance()->GetButtonDown(ButtonName::Button_Start))
	{
		SceneManager::GetInstance()->LoadScene(SceneName::Title);
	}
}

void Scene_Result::Debug()
{

}