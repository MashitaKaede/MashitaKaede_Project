#pragma once
#include "Collider.h"

class CapsuleCollider : public Collider {
public:
	CapsuleCollider();

	VECTOR startPos;
	VECTOR endPos;
	float radius;
};

