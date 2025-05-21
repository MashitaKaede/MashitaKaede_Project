#pragma once
#include "GameObject.h"
#include "Define.h"
#include "Input.h"

class Camera
{
private:
	GameObject* chaseTarget;
	VECTOR position;
	float rotH;
	float rotV;
public:
	Camera();
	~Camera();
	void Init();
	void Term();
	void Debug();
	void Update();
	void Rotate();
	void Rotate_Key();
	void Rotate_XBox();
	void ChaseCharacter();
	inline void SetTarget(GameObject* Target) { chaseTarget = Target; }
	inline float GetRotH() { return rotH; }
};

