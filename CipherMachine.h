#pragma once
#include "Define.h"
#include "GameObject.h"
#include "CapsuleCollider.h"

struct DecodingPos
{
	VECTOR pos;
	bool used;
	int index;
};

class CipherMachine : public GameObject, public CapsuleCollider
{
private:
	float decodingRate;
	bool decodingComplete;
	int index;
	std::vector<DecodingPos> decodingPos;
	int dcodingSurvivorNum;
public:
	CipherMachine(int Handle, int CipherMachineIndex);
	void Init() override;
	void Term() override;
	void Update() override;
	void Draw();
	void Debug();
	void Decoding(float AddDecodingRate);
	void OnCollisionHit(Collider* Collider, GameObject* Parent) override;
	inline float GetDecodingRate() { return decodingRate; }
	inline bool GetDecodingComplete() { return decodingComplete; }
	void LeaveDecodeingPos(int Index);
	DecodingPos GetDecodingPos();
};

