#pragma once
#include "Dxlib.h"
#include <string>
#include <vector>
#include "Math.h"

class GameObject
{
protected:
	int modelHandle;
	MATRIX matrix;  // ���W�ϊ��s��
public:
	VECTOR position;// ���W
	VECTOR rotation;// ��]
	VECTOR scale;   // �g�k
	VECTOR forward; // �O
	VECTOR right; // �E
	std::string tag;     // �^�O
	std::string name;    // �I�u�W�F�N�g��

	GameObject();
	virtual ~GameObject();
	void Transform(int modelHandle);
	virtual void Init() = 0;
	virtual void Term() = 0;
	virtual void Update() = 0;
	inline int GetModelHandle() { return modelHandle; }
};

