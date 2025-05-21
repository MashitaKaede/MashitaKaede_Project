#include "Scene_Main.h"

Scene_Main::Scene_Main()

	: collisionManager(nullptr),
	  hunter(nullptr),
	  camera(nullptr),
	  map(nullptr)
{

}

Scene_Main::~Scene_Main()
{

}

void Scene_Main::Init()
{
	SoundManager::GetInstance()->PlayBGM(BGM_Name::Main);

	// �R���W�����}�l�[�W���[�쐬
	collisionManager = new CollisionManager();

	// �}�b�v�쐬
	map = new Map();
	map->SetCollisionManager(collisionManager);
	map->Init();
	collisionManager->SetPolyInfo_Wall(map->GetPolyInfo_Wall(), (int)map->GetPolyInfo_Wall().size());

	// �A�j���[�V�������X�g�쐬
	animList = new AnimationList();

	// ���̂����쐬
	Survivor* kunoichi = new Kunoichi();
	kunoichi->SetAnimList(animList);
	kunoichi->Init();
	collisionManager->AddCollider(kunoichi);
	kunoichi->SetCollisionManager(collisionManager);
	kunoichi->position = VGet(0.0f, 0.0f, 0.0f);
	kunoichi->Transform(kunoichi->GetModelHandle());

	// ��t�쐬
	Survivor* gardener = new Gardener();
	gardener->SetAnimList(animList);
	gardener->Init();
	collisionManager->AddCollider(gardener);
	gardener->SetCollisionManager(collisionManager);
	gardener->position = VGet(4272.0f, 0.0f, 3455.0f);
	gardener->Transform(gardener->GetModelHandle());

	// �ٌ�m�쐬
	Survivor* lawyer = new Lawyer();
	lawyer->SetAnimList(animList);
	lawyer->Init();
	collisionManager->AddCollider(lawyer);
	lawyer->SetCollisionManager(collisionManager);
	lawyer->position = VGet(5395.0f, 0.0f, 1764.0f);
	lawyer->Transform(lawyer->GetModelHandle());

	// �I�t�F���X�쐬
	Survivor* offense = new Offense();
	offense->SetAnimList(animList);
	offense->Init();
	collisionManager->AddCollider(offense);
	offense->SetCollisionManager(collisionManager);
	offense->position = VGet(6860.0f, 0.0f, -40.0f);
	offense->Transform(offense->GetModelHandle());

	// �n���^�[�쐬
	hunter = new Hunter();
	hunter->SetAnimList(animList);
	hunter->Init();
	collisionManager->AddCollider(hunter);
	collisionManager->AddCollider(hunter->GetAtkCollider());
	hunter->SetCollisionManager(collisionManager);

	// �J�����쐬
	camera = new Camera();

	// �ǂ̃L�������g����
	if (GameManager::GetInstance()->GetSide() == Side::Hunter)
	{
		hunter->mode = Mode::Player;
		camera->SetTarget(hunter);

		gardener->mode = Mode::CPU;
		gardener->InitAStar(map->GetPolyInfo_AStar(), (int)map->GetPolyInfo_AStar().size(), map->GetPolyInfo_Wall(), (int)map->GetPolyInfo_Wall().size());
		gardener->SetCipherMachineVec(map->GetCipherMachineVec());
		survivorNum++;
		survivorVec.push_back(gardener);

		kunoichi->mode = Mode::CPU;
		kunoichi->InitAStar(map->GetPolyInfo_AStar(), (int)map->GetPolyInfo_AStar().size(), map->GetPolyInfo_Wall(), (int)map->GetPolyInfo_Wall().size());
		kunoichi->SetCipherMachineVec(map->GetCipherMachineVec());
		survivorNum++;
		survivorVec.push_back(kunoichi);

		lawyer->mode = Mode::CPU;
		lawyer->InitAStar(map->GetPolyInfo_AStar(), (int)map->GetPolyInfo_AStar().size(), map->GetPolyInfo_Wall(), (int)map->GetPolyInfo_Wall().size());
		lawyer->SetCipherMachineVec(map->GetCipherMachineVec());
		survivorNum++;
		survivorVec.push_back(lawyer);

		offense->mode = Mode::CPU;
		offense->InitAStar(map->GetPolyInfo_AStar(), (int)map->GetPolyInfo_AStar().size(), map->GetPolyInfo_Wall(), (int)map->GetPolyInfo_Wall().size());
		offense->SetCipherMachineVec(map->GetCipherMachineVec());
		survivorNum++;
		survivorVec.push_back(offense);
	}
	else if (GameManager::GetInstance()->GetSide() == Side::Survivor)
	{
		if (GameManager::GetInstance()->GetSurvivorType() == SurvivorType::Gardener)
		{
			camera->SetTarget(gardener);
			gardener->mode = Mode::Player;
			survivorNum++;
			survivorVec.push_back(gardener);
			kunoichi->mode = Mode::CPU;
			survivorNum++;
			survivorVec.push_back(kunoichi);
			lawyer->mode = Mode::CPU;
			survivorNum++;
			survivorVec.push_back(lawyer);
			offense->mode = Mode::CPU;
			survivorNum++;
			survivorVec.push_back(offense);
		}
		else if (GameManager::GetInstance()->GetSurvivorType() == SurvivorType::Kunoichi)
		{
			camera->SetTarget(kunoichi);
			gardener->mode = Mode::CPU;
			survivorNum++;
			survivorVec.push_back(gardener);
			kunoichi->mode = Mode::Player;
			survivorNum++;
			survivorVec.push_back(kunoichi);
			lawyer->mode = Mode::CPU;
			survivorNum++;
			survivorVec.push_back(lawyer);
			offense->mode = Mode::CPU;
			survivorNum++;
			survivorVec.push_back(offense);
		}
		else if (GameManager::GetInstance()->GetSurvivorType() == SurvivorType::Lawyer)
		{
			camera->SetTarget(lawyer);
			gardener->mode = Mode::CPU;
			survivorNum++;
			survivorVec.push_back(gardener);
			kunoichi->mode = Mode::CPU;
			survivorNum++;
			survivorVec.push_back(kunoichi);
			lawyer->mode = Mode::Player;
			survivorNum++;
			survivorVec.push_back(lawyer);
			offense->mode = Mode::CPU;
			survivorNum++;
			survivorVec.push_back(offense);
		}
		else if (GameManager::GetInstance()->GetSurvivorType() == SurvivorType::Offense)
		{
			camera->SetTarget(offense);
			gardener->mode = Mode::CPU;
			survivorNum++;
			survivorVec.push_back(gardener);
			kunoichi->mode = Mode::CPU;
			survivorNum++;
			survivorVec.push_back(kunoichi);
			lawyer->mode = Mode::CPU;
			survivorNum++;
			survivorVec.push_back(lawyer);
			offense->mode = Mode::Player;
			survivorNum++;
			survivorVec.push_back(offense);
		}

		for (int i = 0; i < survivorNum; i++)
		{
			if (survivorVec[i]->mode == Mode::CPU)
			{
				survivorVec[i]->InitAStar(map->GetPolyInfo_AStar(), (int)map->GetPolyInfo_AStar().size(), map->GetPolyInfo_Wall(), (int)map->GetPolyInfo_Wall().size());
				survivorVec[i]->SetCipherMachineVec(map->GetCipherMachineVec());
			}
		}

		hunter->mode = Mode::CPU;
		for (int i = 0; i < survivorNum; i++)
		{
			hunter->AddSurvivor(survivorVec[i]);
		}
		hunter->InitAStar(map->GetPolyInfo_AStar(), (int)map->GetPolyInfo_AStar().size(), map->GetPolyInfo_Wall(), (int)map->GetPolyInfo_Wall().size());
		hunter->SetCipherMachineVec(map->GetCipherMachineVec());
		hunter->SetRocketChairVec(map->GetRocketChairVec());
		hunter->SetExitPosVec(map->GetPosVec_Exit());
	}

	for (int i = 0; i < survivorNum; i++)
	{
		for (int j = 0; j < survivorNum; j++)
		{
			if (i != j)
			{
				survivorVec[i]->AddOtherSurvivor(survivorVec[j]);
			}
		}
		if (survivorVec[i]->mode == Mode::CPU)
		{
			survivorVec[i]->SetRunAwayPointVec(map->GetRunAwayPointVec());
			survivorVec[i]->SetPosVec_Exit_AStar(map->GetPosVec_Exit_AStar());
			survivorVec[i]->AddKeyPad(map->GetExitVec()[0].keypad);
			survivorVec[i]->AddKeyPad(map->GetExitVec()[1].keypad);
		}
		survivorVec[i]->SetHunter(hunter);
	}
}

void Scene_Main::Term()
{
	SoundManager::GetInstance()->StopBGM(BGM_Name::Main);
	for (int i = 0; i < survivorNum; i++)
	{
		delete survivorVec[i];
	}
	delete hunter;
	delete camera;
	delete map;
	delete collisionManager;
}

void Scene_Main::Update()
{
	map->Update();

	for (int i = 0; i < survivorNum; i++)
	{
		survivorVec[i]->SetDecodingComplete(map->GetDecodingComplete());
		survivorVec[i]->SetCameraRotH(camera->GetRotH());
		survivorVec[i]->Update();
	}
	
	hunter->Update();
	hunter->SetCameraRotH(camera->GetRotH());
	collisionManager->Update();
	camera->Update();
	CheckGameEnd();
	UI();
	DrawCharStateForMovie();
}

void Scene_Main::CheckGameEnd()
{
	int survivorCnt_GameClear = 0;
	int survivorCnt_GameOver = 0;

	for (int i = 0; i < survivorNum; i++)
	{
		if (survivorVec[i]->GetState() == State_Survivor::GameOver)
		{
			survivorCnt_GameOver++;
		}
		else if (survivorVec[i]->GetState() == State_Survivor::GameClear)
		{
			survivorCnt_GameClear++;
		}
	}

	if (survivorCnt_GameClear + survivorCnt_GameOver == survivorNum)
	{
		if (survivorCnt_GameClear > survivorCnt_GameOver)
		{
			GameManager::GetInstance()->SetWinSide(Side::Survivor);
		}
		else if (survivorCnt_GameClear < survivorCnt_GameOver)
		{
			GameManager::GetInstance()->SetWinSide(Side::Hunter);
		}
		else
		{
			GameManager::GetInstance()->SetWinSide(Side::None);
		}

		for (int i = 0; i < survivorNum; i++)
		{
			bool flag = false;
			if (survivorVec[i]->GetState() == State_Survivor::GameClear)
			{
				flag = true;
			}
			GameManager::GetInstance()->AddSurvivorEscapeFlagVec(flag);
		}

		SceneManager::GetInstance()->LoadScene(SceneName::Result);
	}
}

void Scene_Main::UI()
{
	int drawPosX = 1920 - UI_SurvivorIconPosDis_X;
	int drawPosY = UI_SurvivorIconPosDis_Y;
	
	for (int i = 0; i < survivorNum; i++)
	{
		// �x�[�X
		DrawRectGraph(drawPosX - i * UI_SurvivorIconPosDis_X, drawPosY,
			          0, 0, 
			          128, 128,
			          GameManager::GetInstance()->GetImageHandle(UI_Name::BaseIcon),
			          true, false);

		// ���O
		DrawFormatString((1920 - 85) - i * 149  - GetDrawFormatStringWidth(const_cast<char*>(survivorVec[i]->name.c_str())) / 2, 200, GetColor(0, 0, 0), "%s", const_cast<char*>(survivorVec[i]->name.c_str()));

		// �������
		if (survivorVec[i]->GetHp() == 2 && survivorVec[i]->GetState() != State_Survivor::GameClear)
		{
			DrawRectGraph(drawPosX - i * UI_SurvivorIconPosDis_X, drawPosY,
				0, 0,
				128, 128,
				GameManager::GetInstance()->GetImageHandle(UI_Name::InjuryIcon),
				true, false);
			DrawRectGraph(drawPosX - i * UI_SurvivorIconPosDis_X, drawPosY,
				0, 0,
				128, 128,
				GameManager::GetInstance()->GetImageHandle(UI_Name::InjuryText),
				true, false);
		}

		// �_�E�����
		if (survivorVec[i]->GetState() == State_Survivor::Fall)
		{
			DrawRectGraph(drawPosX - i * UI_SurvivorIconPosDis_X, drawPosY,
				0, 0,
				128, 128,
				GameManager::GetInstance()->GetImageHandle(UI_Name::DownIcon),
				true, false);
			DrawRectGraph(drawPosX - i * UI_SurvivorIconPosDis_X, drawPosY,
				0, 0,
				128, 128,
				GameManager::GetInstance()->GetImageHandle(UI_Name::DownText),
				true, false);
		}

		// �^�΂���
		if (survivorVec[i]->GetState() == State_Survivor::LiftedUp)
		{
			DrawRectGraph(drawPosX - i * UI_SurvivorIconPosDis_X, drawPosY,
				0, 0,
				128, 128,
				GameManager::GetInstance()->GetImageHandle(UI_Name::CarryText),
				true, false);
		}

		// �S�����
		if (survivorVec[i]->GetState() == State_Survivor::Restrainting)
		{
			DrawRectGraph(drawPosX - i * UI_SurvivorIconPosDis_X, (int)((float)(drawPosY + (128 * (1.0f - survivorVec[i]->GetGameOverRate() / 100.0f)))),
				0, 0 + (int)((float)128 * (1.0f - survivorVec[i]->GetGameOverRate() / 100.0f)),
				128, 128,
				GameManager::GetInstance()->GetImageHandle(UI_Name::RestraintIcon),
				true, false);
			DrawRectGraph(drawPosX - i * UI_SurvivorIconPosDis_X, drawPosY,
				0, 0,
				128, 128,
				GameManager::GetInstance()->GetImageHandle(UI_Name::RestraintText),
				true, false);
		}

		// �Q�[���I�[�o�[���
		if (survivorVec[i]->GetState() == State_Survivor::GameOver || survivorVec[i]->GetState() == State_Survivor::Flying)
		{
			DrawRectGraph(drawPosX - i * UI_SurvivorIconPosDis_X, drawPosY,
				0, 0,
				128, 128,
				GameManager::GetInstance()->GetImageHandle(UI_Name::DeathText),
				true, false);
		}

		// �Q�[���N���A���
		if (survivorVec[i]->GetState() == State_Survivor::GameClear)
		{
			DrawRectGraph(drawPosX - i * UI_SurvivorIconPosDis_X, drawPosY,
				0, 0,
				128, 128,
				GameManager::GetInstance()->GetImageHandle(UI_Name::EscapeText),
				true, false);
		}
	}
}

void Scene_Main::DrawCharStateForMovie()
{
	if (Input::GetInstance()->GetButtonDown(Button_Back))
	{
		if (drawCharState == true)
		{
			drawCharState = false;
		}
		else
		{
			drawCharState = true;
		}
	}

	if (drawCharState == true)
	{
		DrawBox(0, 0, 340, 570, GetColor(200,200,200), true);
		int disY_LineSpace = 20;
		int disY_BlockSpace = 100;
		int drawPosX = 30;
		int drawPosY = 0;
		for (int i = 0; i < survivorNum; i++)
		{
			drawPosY = i * disY_BlockSpace + 30;
			DrawFormatString(drawPosX, drawPosY, GetColor(255, 0, 0), "%s", const_cast<char*>(survivorVec[i]->name.c_str()));

			drawPosY += disY_LineSpace;
			if (survivorVec[i]->mode == Mode::CPU)
			{
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "Mode ");
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 255, 0),   "     : CPU");

				drawPosY += disY_LineSpace;
				if (survivorVec[i]->GetAstarNow() == true)
				{
					DrawFormatString(drawPosX, drawPosY, GetColor(255, 255, 0), "�o�H�T����");
				}
				else
				{
					DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�o�H�T���I��");
				}
			}
			else
			{
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "Mode ");
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 255),   "     : Player");
			}
			
			

			State_Survivor state = survivorVec[i]->GetState();
			drawPosY += disY_LineSpace;
			switch (state)
			{
			case State_Survivor::None:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �����Ȃ�");
				break;
			case State_Survivor::Idle:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : ��~��");
				break;
			case State_Survivor::Move:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �ړ���");
				break;
			case State_Survivor::Fall:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �_�E����");
				break;
			case State_Survivor::LiftedUp:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �S���ꒆ");
				break;
			case State_Survivor::Restrainting:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �߂܂����I");
				break;
			case State_Survivor::Decoding:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : ��ǒ�");
				break;
			case State_Survivor::InputPassward:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �p�X���[�h���͒�");
				break;
			case State_Survivor::Rescue:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �~����");
				break;
			case State_Survivor::Flying:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : ��΂��ꂽ..");
				break;
			case State_Survivor::GameOver:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �Q�[���I�[�o�[......");
				break;
			case State_Survivor::GameClear:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �Q�[���N���A�I�I�I�I�I�I�I�I");
				break;
			case State_Survivor::GoCipherMachine:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �Í��@�Ɍ������Ă���");
				break;
			case State_Survivor::GoKeyPad:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �L�[�p�b�h�Ɍ������Ă���");
				break;
			case State_Survivor::GoRescue:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �~���Ɍ������Ă���");
				break;
			case State_Survivor::RunAway:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : ������");
				break;
			case State_Survivor::GoExit:
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �o���Ɍ������Ă���");
				break;
			default:
				break;
			}
		}
		drawPosY = disY_BlockSpace * survivorNum + disY_LineSpace * 3;
		DrawFormatString(drawPosX, drawPosY, GetColor(255, 0, 0), "�n���^�[");

		drawPosY += disY_LineSpace;
		if (hunter->mode == Mode::CPU)
		{
			DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0),     "Mode ");
			DrawFormatString(drawPosX, drawPosY, GetColor(0, 255, 0),   "     : CPU");

			drawPosY += disY_LineSpace;
			if (hunter->GetAStarNow() == true)
			{
				DrawFormatString(drawPosX, drawPosY, GetColor(255, 255, 0), "�o�H�T����");
			}
			else
			{
				DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�o�H�T���I��");
			}
		}
		else
		{
			DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0),   "Mode ");
			DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 255), "     : Player");
		}

		State_Hunter state_H = hunter->GetState();

		drawPosY += disY_LineSpace;

		switch (state_H)
		{
		case State_Hunter::None:
			DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �����Ȃ�");
			break;
		case State_Hunter::Idle:
			DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : ��~��");
			break;
		case State_Hunter::Move:
			DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �ړ���");
			break;
		case State_Hunter::AtkHit:
			DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �U���q�b�g���[�V������");
			break;
		case State_Hunter::PickUpTarget:
			DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �T�o�C�o�[�����グ��");
			break;
		case State_Hunter::CarryTarget:
			DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �T�o�C�o�[�^�ђ�");
			break;
		case State_Hunter::SearchTarget:
			DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �T�o�C�o�[�{����");
			break;
		case State_Hunter::ChaseTarget:
			DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �T�o�C�o�[�ǂ�������");
			break;
		case State_Hunter::Attack:
			DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �U����");
			break;
		case State_Hunter::Restrain:
			DrawFormatString(drawPosX, drawPosY, GetColor(0, 0, 0), "�s����� : �T�o�C�o�[�֎q�ɍS����");
			break;
		default:
			break;
		}

	}
}

void Scene_Main::Debug()
{
	map->Debug();
	Debug_DispCnt = 0;
	for (int i = 0; i < survivorNum; i++)
	{
		survivorVec[i]->Debug();
	}
	hunter->Debug();
	camera->Debug();
	collisionManager->Debug();
}

