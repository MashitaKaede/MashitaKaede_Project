#include "SoundManager.h"
SoundManager* SoundManager::instance = nullptr;

SoundManager::SoundManager()
{
}

SoundManager::~SoundManager()
{
}

void SoundManager::Init()
{
	if (instance != nullptr)
	{
		return;
	}

	instance = new SoundManager();
}

void SoundManager::Term()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
	InitSoundMem();
}

void SoundManager::LoadSound()
{
	bgmVec = {
		{"Assets/Sound/BGM/Title.mp3", 150, -1},
		{"Assets/Sound/BGM/Select.mp3", 150, -1},
		{"Assets/Sound/BGM/Main.mp3", 100, -1},
		{"Assets/Sound/BGM/Result.mp3", 150, -1},
		{"Assets/Sound/BGM/Approach.mp3", 200, -1},
	};

	int bgmNum = (int)bgmVec.size();

	for (int i = 0; i < bgmNum; i++)
	{
		bgmVec[i].soundHandle = LoadSoundMem(bgmVec[i].fileName.c_str());
		ChangeVolumeSoundMem(bgmVec[i].volume, bgmVec[i].soundHandle);
	}

	seVec = {
		{"Assets/Sound/SE/AttackHit.mp3", 200, -1},
		{"Assets/Sound/SE/Decoding.mp3", 200, -1},
		{"Assets/Sound/SE/HeartBeat.mp3", 200, -1},
	};

	int seNum = (int)seVec.size();

	for (int i = 0; i < seNum; i++)
	{
		seVec[i].soundHandle = LoadSoundMem(seVec[i].fileName.c_str());
		ChangeVolumeSoundMem(seVec[i].volume, seVec[i].soundHandle);
	}
}

void SoundManager::PlaySE(SE_Name Name)
{
	if (CheckSoundMem(seVec[(int)Name].soundHandle) == false)
	{
		PlaySoundMem(seVec[(int)Name].soundHandle, DX_PLAYTYPE_BACK, true);
	}
}

void SoundManager::StopSE(SE_Name Name)
{
	if (CheckSoundMem(seVec[(int)Name].soundHandle) == true)
	{
		StopSoundMem(seVec[(int)Name].soundHandle);
	}
}

void SoundManager::PlayBGM(BGM_Name Name)
{
	if (CheckSoundMem(bgmVec[(int)Name].soundHandle) == false)
	{
		PlaySoundMem(bgmVec[(int)Name].soundHandle, DX_PLAYTYPE_LOOP, true);
	}
}

void SoundManager::StopBGM(BGM_Name Name)
{
	if (CheckSoundMem(bgmVec[(int)Name].soundHandle) == true)
	{
		StopSoundMem(bgmVec[(int)Name].soundHandle);
	}	
}

void SoundManager::ChangeVolume_BGM(BGM_Name Name, int Volume)
{
	ChangeVolumeSoundMem(Volume, bgmVec[(int)Name].soundHandle);
}

void SoundManager::ChangeVolume_SE(SE_Name Name, int Volume)
{
	ChangeVolumeSoundMem(Volume, seVec[(int)Name].soundHandle);
}

