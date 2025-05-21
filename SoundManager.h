#pragma once
#include "DxLib.h"
#include <vector>
#include <string>

enum class SE_Name
{
	AttackHit,
	Decoding,
	HeartBeat
};

enum class BGM_Name
{
	Title,
	SideSelect,
	Main,
	Result,
	Approach
};

struct BGM
{
	std::string fileName;
	int volume;
	int soundHandle;
};

struct SE
{
	std::string fileName;
	int volume;
	int soundHandle;
};

class SoundManager
{
private:
	static SoundManager* instance;
	SoundManager();
	~SoundManager();
	std::vector<BGM> bgmVec;
	std::vector<SE> seVec;

public:
	static inline SoundManager* GetInstance() { return instance; }
	static void Init();
	static void Term();
	void LoadSound();
	void PlaySE(SE_Name Name);
	void StopSE(SE_Name Name);
	void PlayBGM(BGM_Name Name);
	void StopBGM(BGM_Name Name);
	void ChangeVolume_BGM(BGM_Name Name, int Volume);
	void ChangeVolume_SE(SE_Name Name, int Volume);
};

