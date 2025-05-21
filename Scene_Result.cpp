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
					DrawFormatString(1920 / 2 - 450 + i * 300 - GetDrawFormatStringWidth("�E�o") / 2, 550, GetColor(0, 0, 0), "�E�o");
					
				}
				else if(i == 1)
				{
					DrawFormatString(1920 / 2 - 450 + i * 300 - GetDrawFormatStringWidth("�E�o") / 8 * 3, 550, GetColor(0, 0, 0), "�E�o");
				}
				else if (i == 2)
				{
					DrawFormatString(1920 / 2 - 450 + i * 300 + GetDrawFormatStringWidth("�E�o") / 8 * 3, 550, GetColor(0, 0, 0), "�E�o");
				}
				else if (i == 3)
				{
					DrawFormatString(1920 / 2 - 450 + i * 300 + GetDrawFormatStringWidth("�E�o") / 8 * 6, 550, GetColor(0, 0, 0), "�E�o");
				}
				
			}
			else
			{
				if (i == 0)
				{
					DrawFormatString(1920 / 2 - 450 + i * 300 - GetDrawFormatStringWidth("���S") / 2, 550, GetColor(0, 0, 0), "���S");

				}
				else if (i == 1)
				{
					DrawFormatString(1920 / 2 - 450 + i * 300 - GetDrawFormatStringWidth("���S") / 8 * 3, 550, GetColor(0, 0, 0), "���S");
				}
				else if (i == 2)
				{
					DrawFormatString(1920 / 2 - 450 + i * 300 + GetDrawFormatStringWidth("���S") / 8 * 3, 550, GetColor(0, 0, 0), "���S");
				}
				else if (i == 3)
				{
					DrawFormatString(1920 / 2 - 450 + i * 300 + GetDrawFormatStringWidth("���S") / 8 * 6, 550, GetColor(0, 0, 0), "���S");
				}
			}
		}
	}

	DrawFormatString(1920 / 2 + 430, 230, GetColor(0, 0, 0), "�E�o�l�� : %d�l", escapeNum);

	DrawGraph(1920 / 2 - 128 / 2, 220, GameManager::GetInstance()->GetImageHandle(UI_Name::HunterIcon), true);

	DrawFormatString(1920 / 2 - 450 - GetDrawFormatStringWidth("��t") / 2, 750, GetColor(0, 0, 0), "��t");
	DrawFormatString(1920 / 2 - 150 - GetDrawFormatStringWidth("���̂���") / 2, 750, GetColor(0, 0, 0), "���̂���");
	DrawFormatString(1920 / 2 + 150, 750, GetColor(0, 0, 0), "�ٌ�m");
	DrawFormatString(1920 / 2 + 450, 750, GetColor(0, 0, 0), "�I�t�F���X");
	DrawGraph(1920 / 2 - 450 - GetDrawFormatStringWidth("��t") / 8 * 5, 600, GameManager::GetInstance()->GetImageHandle(UI_Name::GardenerIcon), true);
	DrawGraph(1920 / 2 - 150 - GetDrawFormatStringWidth("���̂���") / 8 * 2, 600, GameManager::GetInstance()->GetImageHandle(UI_Name::KunoichiIcon), true);
	DrawGraph(1920 / 2 + 150 + GetDrawFormatStringWidth("�ٌ�m") / 8, 600, GameManager::GetInstance()->GetImageHandle(UI_Name::LawyerIcon), true);
	DrawGraph(1920 / 2 + 450 + GetDrawFormatStringWidth("�I�t�F���X") / 8 * 2, 600, GameManager::GetInstance()->GetImageHandle(UI_Name::OffenseIcon), true);

	SetFontSize(100);
	if (GameManager::GetInstance()->GetWinSide() == Side::Hunter)
	{
		DrawFormatString(1920 / 2 - GetDrawFormatStringWidth("�n���^�[�̏���") / 2, 100, GetColor(0, 0, 0), "�n���^�[�̏���");
	}
	else if (GameManager::GetInstance()->GetWinSide() == Side::Survivor)
	{
		DrawFormatString(1920 / 2 - GetDrawFormatStringWidth("�T�o�C�o�[�̏���") / 2, 100, GetColor(0, 0, 0), "�T�o�C�o�[�̏���");
	}
	else if (GameManager::GetInstance()->GetWinSide() == Side::None)
	{
		DrawFormatString(1920 / 2 - GetDrawFormatStringWidth("��������") / 2, 100, GetColor(0, 0, 0), "��������");
	}
	
	SetFontSize(50);
	DrawFormatString(1920 / 2 - GetDrawFormatStringWidth("�X�^�[�g�{�^���Ń^�C�g����") / 2, 900, GetColor(0, 0, 0), "�X�^�[�g�{�^���Ń^�C�g����");

	if (Input::GetInstance()->GetKyeDown(Return) || Input::GetInstance()->GetButtonDown(ButtonName::Button_Start))
	{
		SceneManager::GetInstance()->LoadScene(SceneName::Title);
	}
}

void Scene_Result::Debug()
{

}