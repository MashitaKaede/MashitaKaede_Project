#pragma once
#include "Define.h"
#include "DxLib.h"
#include "Door.h"
#include "Keypad.h"
#include "CipherMachine.h"
#include "RocketChair.h"
#include "CollisionManager.h"
#include "PolyInfo.h"
#include "Input.h"

struct Exit
{
	Door* right;
	Door* left;
	Keypad* keypad;
	int openDoorCnt;
};

class Map
{
private:
	CollisionManager* collisionManager;

	// ��
	std::vector<int> lightHandleVec;

	// �Í��@
	int cipherMachineHandle;
	int completeMachineNum;
	std::vector<VECTOR> posVec_CipherMachine;
	std::vector<CipherMachine*> cipherMachineVec;
	bool decodingComplete;

	// �o��
	int doorHandle;
	int exitPolyStartIndex;
	
	std::vector<VECTOR> rotVec_Exit;
	std::vector<VECTOR> posVec_Exit;
	std::vector<Exit> exitVec;
	std::vector<VECTOR> posVec_Exit_AStar;

	// �L�[�p�b�h
	int keypadHandle;

	// ���P�b�g�`�F�A
	int rocketChairHandle;
	std::vector<VECTOR> posVec_RocketChair;
	std::vector<RocketChair*> rocketChairVec;

	// �|���S��
	std::vector<MV1_REF_POLYGON*> polyVec;

	std::vector<PolyInfo> polyVec_Wall;
	int wallPolyNum;

	std::vector<PolyInfo> polyVec_AStar;
	int AStarPolyNum;

	// �o�H�T���p�}�b�v
	int mapHandle_AStar;

	// �}�b�v�̕�
	int mapWallHandle;

	std::vector<VECTOR> runAwayPointVec;
public:
	Map();
	~Map();
	void Init();
	void PolyInit();
	void WallPolyGet(int handle, int frameIndex);
	void AStarPolyGet(int handle, int frameIndex);
	void Term();
	void Update();
	void Draw();
	void Debug();
	inline void SetCollisionManager(CollisionManager* ColManager) { collisionManager = ColManager; }
	inline std::vector<PolyInfo> GetPolyInfo_Wall() { return polyVec_Wall; }
	inline std::vector<PolyInfo> GetPolyInfo_AStar() { return polyVec_AStar; }
	inline std::vector<CipherMachine*> GetCipherMachineVec() { return cipherMachineVec; }
	inline std::vector<VECTOR> GetPosVec_Exit() { return posVec_Exit; }
	inline std::vector<VECTOR> GetPosVec_Exit_AStar() { return posVec_Exit_AStar; }
	inline std::vector<RocketChair*> GetRocketChairVec() { return rocketChairVec; }
	inline bool GetDecodingComplete() { return decodingComplete; }
	inline std::vector<Exit> GetExitVec() { return exitVec; }
	inline std::vector<VECTOR> GetRunAwayPointVec() { return runAwayPointVec; }
};