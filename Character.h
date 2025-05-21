#pragma once
#include "GameObject.h"
#include "CollisionManager.h"
#include "CapsuleCollider.h"
#include "Input.h"
#include "Animator.h"
#include "GameManager.h"

// CPUとプレイヤー判別用
enum class Mode
{
	CPU,
	Player
};

class Character : public GameObject, public CapsuleCollider
{
private:

protected:
	float cameraRotH;       // カメラの水平角度(進行方向出す用)
	float defaultMoveSpeed; // 基本移動速度
	float moveSpeed;        // 移動速度
	VECTOR moveVec;         // 移動方向
	VECTOR slideVec;        // 壁ずりベクトル
	Animator* animator;     // アニメーター
	CollisionManager* collisionMnagaer; // 当たり判定系用
public:
	Mode mode;        // CPUとプレイヤーのどちらか判定用
	Character();
	
	// 描画
	void Draw();

	// 向き更新
	void Rotate();

	// カプセルコライダー更新
	virtual void UpdateCollider();

	// デバック表示
	virtual void Debug();

	// セット関数
	inline void SetCameraRotH(float RotH) { cameraRotH = RotH + DX_PI_F; }
	inline void SetCollisionManager(CollisionManager* CollisionManager) { collisionMnagaer = CollisionManager; }
	inline void SetMode(Mode SelectMode) { mode = SelectMode; }
	inline void SetAnimList(AnimationList* AnimList) { animator->SetAnimList(AnimList); }
};