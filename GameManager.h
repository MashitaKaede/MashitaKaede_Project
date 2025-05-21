#pragma once
#include <vector>
#include "DxLib.h"
enum class Side
{
	Hunter,
	Survivor,
	None
};

enum class SurvivorType
{
	Gardener,
	Kunoichi,
	Lawyer,
	Offense,
	Num
};

enum class UI_Name
{
	Title,
	Button_B,
	BaseIcon,
	InjuryIcon,
	DownIcon,
	RestraintIcon,
	InjuryText,
	DownText,
	RestraintText,
	CarryText,
	DeathText,
	EscapeText,
	HunterIcon,
	GardenerIcon,
	KunoichiIcon,
	LawyerIcon,
	OffenseIcon
};

class GameManager
{
private:
	static GameManager* instance;
	GameManager();
	~GameManager();
	
	Side side;
	Side winSide;
	SurvivorType survivorType;
	std::vector<int> imageHandleVec;
	std::vector<bool> survivorEscapeFlagVec;
public:
	static void Init();
	static void Term();
	static inline GameManager* GetInstance() { return instance; }
	void LoadImages();
	inline void SetSide(Side SelectSide) { side = SelectSide; }
	inline Side GetSide() { return side; }
	inline void SetWinSide(Side WinSide) { winSide = WinSide; }
	inline Side GetWinSide() { return winSide; }
	inline void SetSurvivorType(SurvivorType SelectSide) { survivorType = SelectSide; }
	inline SurvivorType GetSurvivorType() { return survivorType; }
	inline int GetImageHandle(UI_Name ImageName) { return imageHandleVec[(int)ImageName]; }
	inline void AddSurvivorEscapeFlagVec(bool Flag) { survivorEscapeFlagVec.push_back(Flag); }
	inline std::vector<bool> GetSurvivorEscapeFlagVec() { return survivorEscapeFlagVec; }

};

