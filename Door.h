#pragma once
#include "GameObject.h"
class Door : public GameObject
{
private:

public:
	Door(int Handle);
	void Init() override;
	void Term() override;
	void Update() override;
	void OpenDoor(VECTOR moveVec);
	void Draw();
	void Deubg();
	inline int GetModelHandle() { return modelHandle; }
};

