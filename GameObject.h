#pragma once
#include "Dxlib.h"
#include <string>
#include <vector>
#include "Math.h"

class GameObject
{
protected:
	int modelHandle;
	MATRIX matrix;  // 座標変換行列
public:
	VECTOR position;// 座標
	VECTOR rotation;// 回転
	VECTOR scale;   // 拡縮
	VECTOR forward; // 前
	VECTOR right; // 右
	std::string tag;     // タグ
	std::string name;    // オブジェクト名

	GameObject();
	virtual ~GameObject();
	void Transform(int modelHandle);
	virtual void Init() = 0;
	virtual void Term() = 0;
	virtual void Update() = 0;
	inline int GetModelHandle() { return modelHandle; }
};

