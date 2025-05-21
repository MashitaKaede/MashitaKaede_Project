#include "Collider.h"

Collider::Collider()

	:colliderIndex(0),
	 parentObject(nullptr),
	 prev(false),
	 now(false),
	 isCheck(true),
	 colliderName(""),
	 isHit(false)
{
}

Collider::~Collider()
{
	
}

void Collider::OnCollisionHit(Collider* Collider, GameObject* Parent)
{
}
