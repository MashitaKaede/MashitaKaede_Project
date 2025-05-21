#pragma once
#include "Define.h"
#include "GameObject.h"
#include "CapsuleCollider.h"

class Keypad : public GameObject, public CapsuleCollider
{
private:
	int index;
	float inputRate;
	bool inputComplete;

public:
	Keypad(int Handle, int KeypadIndex);
	void Init() override;
	void Term() override;
	void Update() override;
	void Draw();
	void Debug();
	void InputPassward(float AddInputRate);
	void OnCollisionHit(Collider* Collider, GameObject* Parent) override;
	inline bool GetInputComplete() { return inputComplete; }
	inline float GetInputRate() { return inputRate; }
};

