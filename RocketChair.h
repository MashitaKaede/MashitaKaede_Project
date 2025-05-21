#pragma once
#include "Define.h"
#include "GameObject.h"
#include "CapsuleCollider.h"

class Survivor;
class RocketChair : public GameObject, public CapsuleCollider
{
private:
	int index;
	Survivor* restrainSurvivor;
	float releaseRate;
	bool releaseNow;
public:
	bool used;
	RocketChair(int Handle, int RocketChairIndex);
	void Init() override;
	void Term() override;
	void Update() override;
	void Draw();
	void Debug();
	void AddReleseRate(float AddRate);
	void UpdateCollider();
	inline void SetSurvivor(Survivor* RestrainSurvivor) { restrainSurvivor = RestrainSurvivor; }
	inline Survivor* GetSurvivor() { return restrainSurvivor; }
	inline float GetReleaseRate() { return releaseRate; }
};

