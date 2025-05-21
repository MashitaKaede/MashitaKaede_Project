#pragma once
#include "GameObject.h"
#include "CollisionManager.h"
#include "CapsuleCollider.h"
#include "Input.h"
#include "Animator.h"
#include "GameManager.h"

// CPU�ƃv���C���[���ʗp
enum class Mode
{
	CPU,
	Player
};

class Character : public GameObject, public CapsuleCollider
{
private:

protected:
	float cameraRotH;       // �J�����̐����p�x(�i�s�����o���p)
	float defaultMoveSpeed; // ��{�ړ����x
	float moveSpeed;        // �ړ����x
	VECTOR moveVec;         // �ړ�����
	VECTOR slideVec;        // �ǂ���x�N�g��
	Animator* animator;     // �A�j���[�^�[
	CollisionManager* collisionMnagaer; // �����蔻��n�p
public:
	Mode mode;        // CPU�ƃv���C���[�̂ǂ��炩����p
	Character();
	
	// �`��
	void Draw();

	// �����X�V
	void Rotate();

	// �J�v�Z���R���C�_�[�X�V
	virtual void UpdateCollider();

	// �f�o�b�N�\��
	virtual void Debug();

	// �Z�b�g�֐�
	inline void SetCameraRotH(float RotH) { cameraRotH = RotH + DX_PI_F; }
	inline void SetCollisionManager(CollisionManager* CollisionManager) { collisionMnagaer = CollisionManager; }
	inline void SetMode(Mode SelectMode) { mode = SelectMode; }
	inline void SetAnimList(AnimationList* AnimList) { animator->SetAnimList(AnimList); }
};