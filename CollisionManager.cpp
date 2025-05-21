#include "CollisionManager.h"

CollisionManager::CollisionManager()

	: colliderNum(0),
	  wallPolyNum(0)
{
}

CollisionManager::~CollisionManager()
{
}

void CollisionManager::AddCollider(Collider* Collider)
{
	Collider->colliderIndex = colliderNum;
	colliderVec.push_back(Collider);
	colliderNum++;
}

void CollisionManager::RemoveCollider(Collider* Collider)
{
	colliderVec.erase(remove(colliderVec.begin(), colliderVec.end(), Collider), colliderVec.end());
	colliderNum--;
}

void CollisionManager::Debug()
{
	
}

void CollisionManager::CheckHit_Capsule_Capsule()
{
	for (int i = 0; i < colliderNum; i++)
	{
		CapsuleCollider* collider1 = (CapsuleCollider*)colliderVec[i];

		if (collider1->isCheck == true)
		{
			//DrawCapsule3D(collider1->startPos, collider1->endPos, collider1->radius, 4, GetColor(255, 0, 0), GetColor(255, 255, 255), false);
			for (int j = i + 1; j < colliderNum; j++)
			{
				CapsuleCollider* collider2 = (CapsuleCollider*)colliderVec[j];

				if (collider2->isCheck == true)
				{
					bool hit = HitCheck_Capsule_Capsule(collider1->startPos, collider1->endPos, collider1->radius,
						collider2->startPos, collider2->endPos, collider2->radius);

					if (hit)
					{
						collider1->OnCollisionHit(collider2, collider2->GetParent());
						collider2->OnCollisionHit(collider1, collider1->GetParent());
					}
				}
				
			}
		}
	}
}

VECTOR CollisionManager::GetSlideVec(VECTOR startPos, VECTOR endPos, VECTOR moveVec)
{
	VECTOR slideVec = moveVec;
	
	for (int i = 0; i < wallPolyNum; i++)
	{
		HITRESULT_LINE result = HitCheck_Line_Triangle(startPos, endPos, polyVec_Wall[i].vertexPos[0], polyVec_Wall[i].vertexPos[1], polyVec_Wall[i].vertexPos[2]);

		if (result.HitFlag)
		{
			VECTOR normalVec = polyVec_Wall[i].normal;
			float dot = VDot(slideVec, normalVec);
			if (dot < 0.0f)
			{
				normalVec = VScale(normalVec, dot);
				slideVec = VSub(slideVec, normalVec);
			}			
		}
	}
	return slideVec;
}

bool CollisionManager::GetWallHit(VECTOR startPos, VECTOR endPos)
{
	VECTOR lineStartPos = startPos;
	lineStartPos.y = 100.0f;
	VECTOR lineEndPos = endPos;
	lineEndPos.y = 100.0f;
	for (int i = 0; i < wallPolyNum; i++)
	{
		HITRESULT_LINE result = HitCheck_Line_Triangle(lineStartPos, lineEndPos, polyVec_Wall[i].vertexPos[0], polyVec_Wall[i].vertexPos[1], polyVec_Wall[i].vertexPos[2]);

		if (result.HitFlag)
		{
			return true;
		}
	}
	return false;
}

bool CollisionManager::CheckHit_Capsule_Capsule(CapsuleCollider* Collider1, CapsuleCollider* Collider2)
{
	bool hit = HitCheck_Capsule_Capsule(Collider1->startPos, Collider1->endPos, Collider1->radius,
		                                Collider2->startPos, Collider2->endPos, Collider2->radius);

	return hit;
}

void CollisionManager::Update()
{
	CheckHit_Capsule_Capsule();
}