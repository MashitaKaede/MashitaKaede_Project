#include "AStar.h"

// �X�^�[�g�ƃS�[���̃|���S�����擾
void AStar::SetStartGoalPolyIndex(VECTOR StartPos, VECTOR GoalPos)
{
	// �X�^�[�g���W�ɏc�������
	VECTOR lineStartPos = StartPos;
	lineStartPos.y += 1000.0f;
	VECTOR lineEndPos = StartPos;
	lineEndPos.y -= 1000.0f;

	// �o�H�T���p�|���S�����ׂĂƍ�������œ����蔻��A����������X�^�[�g�̃|���S���ɂ���
	for (int i = 0; i < polyNum_Astar; i++)
	{		
		HITRESULT_LINE result = HitCheck_Line_Triangle(lineStartPos, lineEndPos, polyVec_AStar[i].vertexPos[0], polyVec_AStar[i].vertexPos[1], polyVec_AStar[i].vertexPos[2]);
		if (result.HitFlag)
		{
			startPolyIndex = i;
			break;
		}
	}

	// �X�^�[�g�|���S�����J����
	polyVec_AStar[startPolyIndex].status = Status::Open;
	openPolyVec.push_back(&polyVec_AStar[startPolyIndex]);


	// �S�[�����������Ƃ�����(�S�[���|���S���͊J���Ȃ�)
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

	// �X�^�[�g�ƃS�[���������ꍇ�S�[���t���O�𗧂Ă�
	if (startPolyIndex == goalPolyIndex)
	{
		goal = true;
	}
}

// �q���[���X�e�B�b�N�v�Z
float AStar::Heuristic(float StartPos_X, float StartPos_Z, float GoalPos_X, float GoalPos_Z)
{
	// �������A���������ꂼ��ŒZ�������o��
	float x = abs(StartPos_X - GoalPos_X);
	float z = abs(StartPos_Z - GoalPos_Z);

	// �����ق���Ԃ�
	if (x >= z)
	{
		return x;
	}
	else
	{
		return z;
	}
}

// �o�H�T��
void AStar::Update()
{
	// �J����|���S�������߂�
	std::vector<int> openPolyIndexVec = DecideNextCheckPoly();
	int size = (int)openPolyIndexVec.size();

	// ���߂������J����
	for (int i = 0; i < size; i++)
	{
		CheckPoly(openPolyIndexVec[i]);
	}
}

// �|���S�����m�F����
void AStar::CheckPoly(int PolyIndex)
{
	// �m�F����|���S�������擾
	PolyInfo* checkPoly = &polyVec_AStar[PolyIndex];

	// �m�F�ς݂ɂ���
	checkPoly->status = Status::Close;
	closePolyVec.push_back(checkPoly);

	// �m�F�҂����X�g����폜
	openPolyVec.erase(remove(openPolyVec.begin(), openPolyVec.end(), checkPoly), openPolyVec.end());

	// �m�F���|���S���̎��R�X�g���擾
	float checkPolyCost = checkPoly->cost;

	// �אڃ|���S�����J����
	for (int i = 0; i < 3; i++)
	{
		// �אڃ|���S���������
		int nextPolyIndex = checkPoly->linkPolyIndex[i];
		if (nextPolyIndex == -1)
		{
			continue;
		}

		// �אڃ|���S���̏��擾
		PolyInfo* nextPoly = &polyVec_AStar[nextPolyIndex];

		// �אڃ|���S�����S�[����������t���O�𗧂Ă�
		if (nextPoly->index == goalPolyIndex)
		{
			// �m�F�ς݂ɂ���
			nextPoly->status = Status::Close;
			nextPoly->parentPolyIndex = checkPoly->index;
			closePolyVec.push_back(nextPoly);
			goal = true;
			break;
		}

		// �אڃ|���S�����J���Ă��Ȃ����
		if (nextPoly->status != Status::None)
		{
			continue;
		}

		// �J����
		nextPoly->status = Status::Open;

		// ���R�X�g�A�q���[���X�e�B�b�N�R�X�g�A�g�[�^���R�X�g�i�X�R�A�j�v�Z
		nextPoly->cost = checkPolyCost + checkPoly->linkPolyDistance[i];
		nextPoly->cost_H = Heuristic(nextPoly->centerPosition.x, nextPoly->centerPosition.z, goalPos.x, goalPos.z);
		nextPoly->cost_T = nextPoly->cost + nextPoly->cost_H;

		// 1�O�̃|���S����o�^
		nextPoly->parentPolyIndex = checkPoly->index;

		// �m�F�҂��|���S���ɒǉ�
		openPolyVec.push_back(nextPoly);
	}

	// �m�F�ς݃|���S���̐�
	checkNum++;

	// �S�[��������
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
