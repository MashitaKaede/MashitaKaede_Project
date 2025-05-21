#include "AStar.h"

// スタートとゴールのポリゴンを取得
void AStar::SetStartGoalPolyIndex(VECTOR StartPos, VECTOR GoalPos)
{
	// スタート座標に縦線を作る
	VECTOR lineStartPos = StartPos;
	lineStartPos.y += 1000.0f;
	VECTOR lineEndPos = StartPos;
	lineEndPos.y -= 1000.0f;

	// 経路探索用ポリゴンすべてと作った線で当たり判定、当たったらスタートのポリゴンにする
	for (int i = 0; i < polyNum_Astar; i++)
	{		
		HITRESULT_LINE result = HitCheck_Line_Triangle(lineStartPos, lineEndPos, polyVec_AStar[i].vertexPos[0], polyVec_AStar[i].vertexPos[1], polyVec_AStar[i].vertexPos[2]);
		if (result.HitFlag)
		{
			startPolyIndex = i;
			break;
		}
	}

	// スタートポリゴンを開ける
	polyVec_AStar[startPolyIndex].status = Status::Open;
	openPolyVec.push_back(&polyVec_AStar[startPolyIndex]);


	// ゴールも同じことをする(ゴールポリゴンは開けない)
	lineStartPos = GoalPos;
	lineStartPos.y += 1000.0f;
	lineEndPos = GoalPos;
	lineEndPos.y -= 1000.0f;
	for (int i = 0; i < polyNum_Astar; i++)
	{
		HITRESULT_LINE result = HitCheck_Line_Triangle(lineStartPos, lineEndPos, polyVec_AStar[i].vertexPos[0], polyVec_AStar[i].vertexPos[1], polyVec_AStar[i].vertexPos[2]);
		if (result.HitFlag)
		{
			goalPos = polyVec_AStar[i].centerPosition;
			goalPolyIndex = i;
			break;
		}
	}

	// スタートとゴールが同じ場合ゴールフラグを立てる
	if (startPolyIndex == goalPolyIndex)
	{
		goal = true;
	}
}

// ヒューリスティック計算
float AStar::Heuristic(float StartPos_X, float StartPos_Z, float GoalPos_X, float GoalPos_Z)
{
	// ｘ成分、ｚ成分それぞれ最短距離を出す
	float x = abs(StartPos_X - GoalPos_X);
	float z = abs(StartPos_Z - GoalPos_Z);

	// 長いほうを返す
	if (x >= z)
	{
		return x;
	}
	else
	{
		return z;
	}
}

// 経路探索
void AStar::Update()
{
	// 開けるポリゴンを決める
	std::vector<int> openPolyIndexVec = DecideNextCheckPoly();
	int size = (int)openPolyIndexVec.size();

	// 決めた数分開ける
	for (int i = 0; i < size; i++)
	{
		CheckPoly(openPolyIndexVec[i]);
	}
}

// ポリゴンを確認する
void AStar::CheckPoly(int PolyIndex)
{
	// 確認するポリゴン情報を取得
	PolyInfo* checkPoly = &polyVec_AStar[PolyIndex];

	// 確認済みにする
	checkPoly->status = Status::Close;
	closePolyVec.push_back(checkPoly);

	// 確認待ちリストから削除
	openPolyVec.erase(remove(openPolyVec.begin(), openPolyVec.end(), checkPoly), openPolyVec.end());

	// 確認中ポリゴンの実コストを取得
	float checkPolyCost = checkPoly->cost;

	// 隣接ポリゴンを開ける
	for (int i = 0; i < 3; i++)
	{
		// 隣接ポリゴンがあれば
		int nextPolyIndex = checkPoly->linkPolyIndex[i];
		if (nextPolyIndex == -1)
		{
			continue;
		}

		// 隣接ポリゴンの情報取得
		PolyInfo* nextPoly = &polyVec_AStar[nextPolyIndex];

		// 隣接ポリゴンがゴールだったらフラグを立てる
		if (nextPoly->index == goalPolyIndex)
		{
			// 確認済みにする
			nextPoly->status = Status::Close;
			nextPoly->parentPolyIndex = checkPoly->index;
			closePolyVec.push_back(nextPoly);
			goal = true;
			break;
		}

		// 隣接ポリゴンが開いていなければ
		if (nextPoly->status != Status::None)
		{
			continue;
		}

		// 開ける
		nextPoly->status = Status::Open;

		// 実コスト、ヒューリスティックコスト、トータルコスト（スコア）計算
		nextPoly->cost = checkPolyCost + checkPoly->linkPolyDistance[i];
		nextPoly->cost_H = Heuristic(nextPoly->centerPosition.x, nextPoly->centerPosition.z, goalPos.x, goalPos.z);
		nextPoly->cost_T = nextPoly->cost + nextPoly->cost_H;

		// 1つ前のポリゴンを登録
		nextPoly->parentPolyIndex = checkPoly->index;

		// 確認待ちポリゴンに追加
		openPolyVec.push_back(nextPoly);
	}

	// 確認済みポリゴンの数
	checkNum++;

	// ゴールしたら
	if (goal)
	{
		PolyInfo poly;
		PolyInfo prevPoly;
		for (int i = 0; i < checkNum; i++)
		{
			if (i == 0)
			{
				poly = polyVec_AStar[goalPolyIndex];
				int prevIndex = polyVec_AStar[goalPolyIndex].parentPolyIndex;
				prevPoly = polyVec_AStar[prevIndex];
				finalPosVec.push_back(poly.centerPosition);
			}
			else
			{
				poly = prevPoly;
				int prevIndex = poly.parentPolyIndex;
				prevPoly = polyVec_AStar[prevIndex];
			}

			VECTOR start = finalPosVec.back();
			start.y += 100.0f;
			VECTOR target = prevPoly.centerPosition;
			target.y += 100.0f;
			for (int j = 0; j < wallPolyNum; j++)
			{
				HITRESULT_LINE hitResult = HitCheck_Line_Triangle(
					start,
					target,
					polyVec_Wall[j].vertexPos[0],
					polyVec_Wall[j].vertexPos[1],
					polyVec_Wall[j].vertexPos[2]
				);
				if ((bool)hitResult.HitFlag == true)
				{
					finalPosVec.push_back(poly.centerPosition);
					break;
				}
			}

			if (prevPoly.index == startPolyIndex)
			{
				finalPosVec.push_back(polyVec_AStar[startPolyIndex].centerPosition);
				break;
			}
		}
	}

	rootNum = (int)finalPosVec.size() - 1;
	nowRootNum = rootNum;
}

std::vector<int> AStar::DecideNextCheckPoly()
{
	int openPolyVecSize = (int)openPolyVec.size();
	float minScore = 0.0f;
	std::vector<int> minScorePolyIndexVec;
	for (int i = 0; i < openPolyVecSize; i++)
	{
		PolyInfo* checkPoly = openPolyVec[i];
		float checkPolyScore = openPolyVec[i]->cost_T;

		if (i == 0)
		{
			minScore = checkPolyScore;
			minScorePolyIndexVec.push_back(checkPoly->index);
		}
		else
		{
			if (minScore >= checkPolyScore)
			{
				if (minScore == checkPolyScore)
				{
					minScorePolyIndexVec.push_back(checkPoly->index);
				}
				else
				{
					minScore = checkPolyScore;
					minScorePolyIndexVec.clear();
					minScorePolyIndexVec.push_back(checkPoly->index);
				}
			}
		}
	}

	return minScorePolyIndexVec;
}

void AStar::ResetPolyVec()
{
	goal = false;
	int openPolyNum = (int)openPolyVec.size();
	for (int i = 0; i < openPolyNum; i++)
	{
		openPolyVec[i]->status = Status::None;
	}
	openPolyVec.clear();
	openPolyVec.shrink_to_fit();

	int closePolyNum = (int)closePolyVec.size();
	for (int i = 0; i < closePolyNum; i++)
	{
		closePolyVec[i]->status = Status::None;
	}
	closePolyVec.clear();
	closePolyVec.shrink_to_fit();

	finalPosVec.clear();
	finalPosVec.shrink_to_fit();

	nowRootNum = 0;
	rootNum = 0;
}
