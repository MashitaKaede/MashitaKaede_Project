#pragma once
#include "PolyInfo.h"
#include "Dxlib.h"
#include <string>
#include <vector>
#include "Math.h"

class AStar
{
public:
	std::vector<PolyInfo*> openPolyVec;  // 確認待ちポリゴン
	std::vector<PolyInfo*> closePolyVec; // 確認済みポリゴン
	std::vector<VECTOR> finalPosVec;     // 最終的に通っていく座標(経路)
	std::vector<PolyInfo> polyVec_AStar; // 経路探索に使うポリゴン
	int polyNum_Astar;                   // 経路探索に使うポリゴンの数
	bool goal{ false };                  // 経路探索が終わったか
	int checkNum{ 0 };                   // 開けた(確認待ちにしたポリゴンの数)
	VECTOR goalPos;                      // ゴールになるポリゴンの座標
	int startPolyIndex;                  // スタートになるポリゴンの番号
	int goalPolyIndex;                   // ゴールになるポリゴンの番号
	std::vector<PolyInfo> polyVec_Wall;  // 壁ポリゴン
	int wallPolyNum;                     // 壁ポリゴンの数
	int rootNum;                         // 経路を構成している座標の数
	int nowRootNum;                      // 何番目の座標まで辿ったか

	// 経路探索に使うポリゴンのセット
	inline void SetPolyVec_AStar(std::vector<PolyInfo> AStarPoly, int AStarPolyNum) { polyVec_AStar = AStarPoly; polyNum_Astar = AStarPolyNum; }

	// 壁ポリゴンのセット
	inline void SetPolyVec_Wall(std::vector<PolyInfo> WallPoly, int WallPolyNum) { polyVec_Wall = WallPoly; wallPolyNum = WallPolyNum; }

	// ゴール座標のセット
	inline void SetGoalPos(VECTOR GoalPos) { goalPos = GoalPos; }

	// スタートとゴールのポリゴンを取得
	void SetStartGoalPolyIndex(VECTOR StartPos, VECTOR GoalPos);

	// ヒューリスティックコスト計算
	float Heuristic(float StartPos_X, float StartPos_Z, float GoalPos_X, float GoalPos_Z);

	// 更新
	void Update();

	// ポリゴンを確認する
	void CheckPoly(int PolyIndex);

	// 次に確認するポリゴンを決める
	std::vector<int> DecideNextCheckPoly();

	// 経路探索のリセット
	void ResetPolyVec();
};

