#pragma once
#include "GameObject.h"
class Collider
{
private:
	GameObject* parentObject;

public:
	int colliderIndex;
	bool prev;
	bool now;
	bool isCheck;
	bool isHit;
	std::string colliderName;

	Collider();
	virtual ~Collider();

	inline void SetParent(GameObject* Parent) { parentObject = Parent; }
	inline GameObject* GetParent() { return parentObject; }

	virtual void OnCollisionHit(Collider* Collider, GameObject* Parent);
};

