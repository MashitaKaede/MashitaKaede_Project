#pragma once
#include "PolyInfo.h"
#include "Dxlib.h"
#include <string>
#include <vector>
#include "Math.h"

class AStar
{
public:
	std::vector<PolyInfo*> openPolyVec;  // �m�F�҂��|���S��
	std::vector<PolyInfo*> closePolyVec; // �m�F�ς݃|���S��
	std::vector<VECTOR> finalPosVec;     // �ŏI�I�ɒʂ��Ă������W(�o�H)
	std::vector<PolyInfo> polyVec_AStar; // �o�H�T���Ɏg���|���S��
	int polyNum_Astar;                   // �o�H�T���Ɏg���|���S���̐�
	bool goal{ false };                  // �o�H�T�����I�������
	int checkNum{ 0 };                   // �J����(�m�F�҂��ɂ����|���S���̐�)
	VECTOR goalPos;                      // �S�[���ɂȂ�|���S���̍��W
	int startPolyIndex;                  // �X�^�[�g�ɂȂ�|���S���̔ԍ�
	int goalPolyIndex;                   // �S�[���ɂȂ�|���S���̔ԍ�
	std::vector<PolyInfo> polyVec_Wall;  // �ǃ|���S��
	int wallPolyNum;                     // �ǃ|���S���̐�
	int rootNum;                         // �o�H���\�����Ă�����W�̐�
	int nowRootNum;                      // ���Ԗڂ̍��W�܂ŒH������

	// �o�H�T���Ɏg���|���S���̃Z�b�g
	inline void SetPolyVec_AStar(std::vector<PolyInfo> AStarPoly, int AStarPolyNum) { polyVec_AStar = AStarPoly; polyNum_Astar = AStarPolyNum; }

	// �ǃ|���S���̃Z�b�g
	inline void SetPolyVec_Wall(std::vector<PolyInfo> WallPoly, int WallPolyNum) { polyVec_Wall = WallPoly; wallPolyNum = WallPolyNum; }

	// �S�[�����W�̃Z�b�g
	inline void SetGoalPos(VECTOR GoalPos) { goalPos = GoalPos; }

	// �X�^�[�g�ƃS�[���̃|���S�����擾
	void SetStartGoalPolyIndex(VECTOR StartPos, VECTOR GoalPos);

	// �q���[���X�e�B�b�N�R�X�g�v�Z
	float Heuristic(float StartPos_X, float StartPos_Z, float GoalPos_X, float GoalPos_Z);

	// �X�V
	void Update();

	// �|���S�����m�F����
	void CheckPoly(int PolyIndex);

	// ���Ɋm�F����|���S�������߂�
	std::vector<int> DecideNextCheckPoly();

	// �o�H�T���̃��Z�b�g
	void ResetPolyVec();
};

