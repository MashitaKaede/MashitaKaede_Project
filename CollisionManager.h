#pragma once
#include "CapsuleCollider.h"
#include "PolyInfo.h"
#include "Define.h"

class CollisionManager
{
private:
	int colliderNum;
	std::vector<Collider*> colliderVec;
	
	// ƒ|ƒŠƒSƒ“
	std::vector<PolyInfo> polyVec_Wall;
	int wallPolyNum;

public:
	CollisionManager();
	~CollisionManager();
	void AddCollider(Collider* Collider);
	void RemoveCollider(Collider* Collider);
	void Debug();
	void CheckHit_Capsule_Capsule();
	VECTOR GetSlideVec(VECTOR startPos, VECTOR endPos, VECTOR moveVec);
	bool GetWallHit(VECTOR startPos, VECTOR endPos);
	bool CheckHit_Capsule_Capsule(CapsuleCollider* Collider1, CapsuleCollider* Collider2);
	void Update();
	inline void SetPolyInfo_Wall(std::vector<PolyInfo> PolyVec, int size) { polyVec_Wall = PolyVec; wallPolyNum = size; }
};

