#include "Scene_SideSelect.h"

Scene_SideSelect::Scene_SideSelect()
	:side(0),
	 survivorType(0),
	 decideSide(false)
{

}

Scene_SideSelect::~Scene_SideSelect()
{

}

void Scene_SideSelect::Init()
{
	side = 0;
	survivorType = 0;
	decideSide = false;
	SoundManager::GetInstance()->PlayBGM(BGM_Name::SideSelect);
}

void Scene_SideSelect::Term()
{
	SoundManager::GetInstance()->StopBGM(BGM_Name::SideSelect);
}

void Scene_SideSelect::Update()
{
	if (decideSide == false)
	{
		SideSelect();
	}
	else
	{
		SurvivorTypeSelect();
	}
}

void Scene_SideSelect::Debug()
{

}

void Scene_SideSelect::SideSelect()
{
	SetFontSize(100);
	DrawFormatString(1920 / 2  - GetDrawFormatStringWidth("陣営選択") / 2, 200, GetColor(0, 0, 0), "陣営選択");
	SetFontSize(50);
	DrawFormatString(1920 / 2 - 400 - GetDrawFormatStringWidth("ハンター") / 2, 400, GetColor(0, 0, 0), "ハンター");
	DrawFormatString(1920 / 2 + 400, 400, GetColor(0, 0, 0), "サバイバー");
	DrawFormatString(1920 / 2 - GetDrawFormatStringWidth("スタートボタンで決定") / 2, 900, GetColor(0, 0, 0), "スタートボタンで決定");
	DrawGraph(1920 / 2 - 300 - GetDrawFormatStringWidth("ハンター") / 8 * 7, 520, GameManager::GetInstance()->GetImageHandle(UI_Name::HunterIcon), true);
	DrawGraph(1920 / 2 + 450 - GetDrawFormatStringWidth("サバイバー") / 8 * 6, 520, GameManager::GetInstance()->GetImageHandle(UI_Name::GardenerIcon), true);
	DrawGraph(1920 / 2 + 450 - GetDrawFormatStringWidth("サバイバー") / 8 * 2, 520, GameManager::GetInstance()->GetImageHandle(UI_Name::KunoichiIcon), true);
	DrawGraph(1920 / 2 + 450 + GetDrawFormatStringWidth("サバイバー") / 8 * 2, 520, GameManager::GetInstance()->GetImageHandle(UI_Name::LawyerIcon), true);
	DrawGraph(1920 / 2 + 450 + GetDrawFormatStringWidth("サバイバー") / 8 * 6, 520, GameManager::GetInstance()->GetImageHandle(UI_Name::OffenseIcon), true);
	if (side == 0)
	{
		DrawFormatString(1920 / 2 - 300 - GetDrawFormatStringWidth("ハンター") / 8 * 5, 350, GetColor(0, 0, 0), "↓");
		
		if (Input::GetInstance()->GetButtonDown(Button_Right))
		{
			side++;
		}
		if (Input::GetInstance()->GetButtonDown(Button_Start))
		{
			GameManager::GetInstance()->SetSide(Side::Hunter);

			if (Input::GetInstance()->GetKyeDown(Return) || Input::GetInstance()->GetButtonDown(ButtonName::Button_Start))
			{
				SetFontSize(16);
				SceneManager::GetInstance()->LoadScene(SceneName::Main);
			}
		}
	}
	if (side == 1)
	{
		DrawFormatString(1920 / 2 + 400 + GetDrawFormatStringWidth("サバイバー") / 8 * 3, 350, GetColor(0, 0, 0), "↓");
		if (Input::GetInstance()->GetButtonDown(Button_Left))
		{
			side--;
		}
		if (Input::GetInstance()->GetButtonDown(Button_Start))
		{
			GameManager::GetInstance()->SetSide(Side::Survivor);
			decideSide = true;
		}
	}
}

void Scene_SideSelect::SurvivorTypeSelect()
{
	SetFontSize(100);
	DrawFormatString(1920 / 2 - GetDrawFormatStringWidth("サバイバー選択") / 2, 200, GetColor(0, 0, 0), "サバイバー選択"); //＊モデルが変わるだけです！
	SetFontSize(30);
	//DrawFormatString(1920 / 2 + 200 + GetDrawFormatStringWidth("＊モデルが変わるだけです！") / 2, 230, GetColor(0, 0, 0), "＊モデルが変わるだけです！");
	SetFontSize(50);
	DrawFormatString(1920 / 2 - 450 - GetDrawFormatStringWidth("庭師") / 2, 500, GetColor(0, 0, 0), "庭師");
	DrawFormatString(1920 / 2 - 150 - GetDrawFormatStringWidth("くのいち") / 2, 500, GetColor(0, 0, 0), "くのいち");
	DrawFormatString(1920 / 2 + 150, 500, GetColor(0, 0, 0), "弁護士");
	DrawFormatString(1920 / 2 + 450, 500, GetColor(0, 0, 0), "オフェンス");
	DrawFormatString(1920 / 2 - GetDrawFormatStringWidth("スタートボタンで決定") / 2, 900, GetColor(0, 0, 0), "スタートボタンで決定");

	DrawGraph(1920 / 2 - 450 - GetDrawFormatStringWidth("庭師") / 8 * 6, 650, GameManager::GetInstance()->GetImageHandle(UI_Name::GardenerIcon), true);
	DrawGraph(1920 / 2 - 150 - GetDrawFormatStringWidth("くのいち") / 8 * 2, 650, GameManager::GetInstance()->GetImageHandle(UI_Name::KunoichiIcon), true);
	DrawGraph(1920 / 2 + 150 + GetDrawFormatStringWidth("弁護士") / 8, 650, GameManager::GetInstance()->GetImageHandle(UI_Name::LawyerIcon), true);
	DrawGraph(1920 / 2 + 450 + GetDrawFormatStringWidth("オフェンス") / 8 * 2, 650, GameManager::GetInstance()->GetImageHandle(UI_Name::OffenseIcon), true);

	if (survivorType < (int)SurvivorType::Num)
	{
		if (Input::GetInstance()->GetButtonDown(Button_Right))
		{
			survivorType++;
		}
	}
	if (survivorType > 0)
	{
		if (Input::GetInstance()->GetButtonDown(Button_Left))
		{
			survivorType--;
		}
	}

	if (survivorType == (int)SurvivorType::Gardener)
	{
		DrawFormatString(1920 / 2 - 450 - GetDrawFormatStringWidth("庭師") / 4, 450, GetColor(0, 0, 0), "↓");
	}
	else if (survivorType == (int)SurvivorType::Kunoichi)
	{
		DrawFormatString(1920 / 2 - 150 - GetDrawFormatStringWidth("くのいち") / 8, 450, GetColor(0, 0, 0), "↓");
	}
	else if (survivorType == (int)SurvivorType::Lawyer)
	{
		DrawFormatString(1920 / 2 + 150 + GetDrawFormatStringWidth("弁護士") / 8 * 3, 450, GetColor(0, 0, 0), "↓");
	}
	else if (survivorType == (int)SurvivorType::Offense)
	{
		DrawFormatString(1920 / 2 + 450 + GetDrawFormatStringWidth("オフェンス") / 8 * 3, 450, GetColor(0, 0, 0), "↓");
	}
	
	if (Input::GetInstance()->GetButtonDown(Button_Start))
	{
		if (Input::GetInstance()->GetKyeDown(Return) || Input::GetInstance()->GetButtonDown(ButtonName::Button_Start))
		{
			SetFontSize(16);
			GameManager::GetInstance()->SetSurvivorType((SurvivorType)survivorType);
			SceneManager::GetInstance()->LoadScene(SceneName::Main);
		}
	}
}
