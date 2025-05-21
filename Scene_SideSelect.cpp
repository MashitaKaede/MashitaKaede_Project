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
	DrawFormatString(1920 / 2  - GetDrawFormatStringWidth("�w�c�I��") / 2, 200, GetColor(0, 0, 0), "�w�c�I��");
	SetFontSize(50);
	DrawFormatString(1920 / 2 - 400 - GetDrawFormatStringWidth("�n���^�[") / 2, 400, GetColor(0, 0, 0), "�n���^�[");
	DrawFormatString(1920 / 2 + 400, 400, GetColor(0, 0, 0), "�T�o�C�o�[");
	DrawFormatString(1920 / 2 - GetDrawFormatStringWidth("�X�^�[�g�{�^���Ō���") / 2, 900, GetColor(0, 0, 0), "�X�^�[�g�{�^���Ō���");
	DrawGraph(1920 / 2 - 300 - GetDrawFormatStringWidth("�n���^�[") / 8 * 7, 520, GameManager::GetInstance()->GetImageHandle(UI_Name::HunterIcon), true);
	DrawGraph(1920 / 2 + 450 - GetDrawFormatStringWidth("�T�o�C�o�[") / 8 * 6, 520, GameManager::GetInstance()->GetImageHandle(UI_Name::GardenerIcon), true);
	DrawGraph(1920 / 2 + 450 - GetDrawFormatStringWidth("�T�o�C�o�[") / 8 * 2, 520, GameManager::GetInstance()->GetImageHandle(UI_Name::KunoichiIcon), true);
	DrawGraph(1920 / 2 + 450 + GetDrawFormatStringWidth("�T�o�C�o�[") / 8 * 2, 520, GameManager::GetInstance()->GetImageHandle(UI_Name::LawyerIcon), true);
	DrawGraph(1920 / 2 + 450 + GetDrawFormatStringWidth("�T�o�C�o�[") / 8 * 6, 520, GameManager::GetInstance()->GetImageHandle(UI_Name::OffenseIcon), true);
	if (side == 0)
	{
		DrawFormatString(1920 / 2 - 300 - GetDrawFormatStringWidth("�n���^�[") / 8 * 5, 350, GetColor(0, 0, 0), "��");
		
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
		DrawFormatString(1920 / 2 + 400 + GetDrawFormatStringWidth("�T�o�C�o�[") / 8 * 3, 350, GetColor(0, 0, 0), "��");
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
	DrawFormatString(1920 / 2 - GetDrawFormatStringWidth("�T�o�C�o�[�I��") / 2, 200, GetColor(0, 0, 0), "�T�o�C�o�[�I��"); //�����f�����ς�邾���ł��I
	SetFontSize(30);
	//DrawFormatString(1920 / 2 + 200 + GetDrawFormatStringWidth("�����f�����ς�邾���ł��I") / 2, 230, GetColor(0, 0, 0), "�����f�����ς�邾���ł��I");
	SetFontSize(50);
	DrawFormatString(1920 / 2 - 450 - GetDrawFormatStringWidth("��t") / 2, 500, GetColor(0, 0, 0), "��t");
	DrawFormatString(1920 / 2 - 150 - GetDrawFormatStringWidth("���̂���") / 2, 500, GetColor(0, 0, 0), "���̂���");
	DrawFormatString(1920 / 2 + 150, 500, GetColor(0, 0, 0), "�ٌ�m");
	DrawFormatString(1920 / 2 + 450, 500, GetColor(0, 0, 0), "�I�t�F���X");
	DrawFormatString(1920 / 2 - GetDrawFormatStringWidth("�X�^�[�g�{�^���Ō���") / 2, 900, GetColor(0, 0, 0), "�X�^�[�g�{�^���Ō���");

	DrawGraph(1920 / 2 - 450 - GetDrawFormatStringWidth("��t") / 8 * 6, 650, GameManager::GetInstance()->GetImageHandle(UI_Name::GardenerIcon), true);
	DrawGraph(1920 / 2 - 150 - GetDrawFormatStringWidth("���̂���") / 8 * 2, 650, GameManager::GetInstance()->GetImageHandle(UI_Name::KunoichiIcon), true);
	DrawGraph(1920 / 2 + 150 + GetDrawFormatStringWidth("�ٌ�m") / 8, 650, GameManager::GetInstance()->GetImageHandle(UI_Name::LawyerIcon), true);
	DrawGraph(1920 / 2 + 450 + GetDrawFormatStringWidth("�I�t�F���X") / 8 * 2, 650, GameManager::GetInstance()->GetImageHandle(UI_Name::OffenseIcon), true);

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
		DrawFormatString(1920 / 2 - 450 - GetDrawFormatStringWidth("��t") / 4, 450, GetColor(0, 0, 0), "��");
	}
	else if (survivorType == (int)SurvivorType::Kunoichi)
	{
		DrawFormatString(1920 / 2 - 150 - GetDrawFormatStringWidth("���̂���") / 8, 450, GetColor(0, 0, 0), "��");
	}
	else if (survivorType == (int)SurvivorType::Lawyer)
	{
		DrawFormatString(1920 / 2 + 150 + GetDrawFormatStringWidth("�ٌ�m") / 8 * 3, 450, GetColor(0, 0, 0), "��");
	}
	else if (survivorType == (int)SurvivorType::Offense)
	{
		DrawFormatString(1920 / 2 + 450 + GetDrawFormatStringWidth("�I�t�F���X") / 8 * 3, 450, GetColor(0, 0, 0), "��");
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
