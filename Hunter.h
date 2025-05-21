#pragma once
#include "Survivor.h"
#include "PolyInfo.h"
#include "AStar.h"
#include "CipherMachine.h"
#include "Keypad.h"
#include "RocketChair.h"
#include "SoundManager.h"

enum class State_Hunter
{
	None,
	Idle,
	Move,
	AtkHit,
	PickUpTarget,
	CarryTarget,
	// AI�p
	SearchTarget,
	ChaseTarget,
	Attack,
	Restrain
};

class Hunter : public Character
{
private:
// �v���C���[�AAI����
	State_Hunter state;     // ���
	State_Hunter prevState; // �O�̏��
	Survivor* carryTarget;  // �^��ł�T�o�C�o�[
	float pickUpTime;       // �����グ�n�߂Ă���̎���
	// �U���p
	CapsuleCollider* atkCollider; // �U���p�R���C�_�[
	bool atkCoolTimeNow; // �U���̃N�[���^�C������
	float stopTime;    // ��~���Ă���̎���

// �v���C���[�p

// AI�p
	AStar* aStar;            // �o�H�T���p�N���X
	Survivor* target;        // �ǂ�������T�o�C�o�[
	CipherMachine* targetCM; // �ړI�̈Í��@
	std::vector<Survivor*> survivorVec;           // �T�o�C�o�[�z��
	std::vector<CipherMachine*> cipherMachineVec; // �Í��@�z��
	std::vector<int> check_CM_IndexVec;           // �T�o�C�o�[�{�����Ɋm�F�����Í��@�̔ԍ��z��
	std::vector<VECTOR> exitPosVec;               // �o���̍��W�z��
	std::vector<RocketChair*> rocketChairVec;     // ���P�b�g�`�F�A�z��
	int survivorNum;   // �T�o�C�o�[�̐�
	int targetCMIndex; // �������Ă�Í��@�̔ԍ�
	int targetRCIndex; // �������Ă郍�P�b�g�`�F�A�̔ԍ�
	bool goRC;         // ���P�b�g�`�F�A�Ɍ�����
	bool goCM;         // �Í��@�Ɍ�����
	bool chaseDirect;  // ���ڒǂ������邩�ǂ���
	bool aStarNow;
public:
// �v���C���[�AAI����
	Hunter();
	~Hunter() override;
	void Init() override;
	void Term() override;
	void Update() override;
	void Debug() override;

// �v���C���[�p
	void CheckMove();

// AI�p
	// �o�H�T���p�̃|���S�����̃Z�b�g
	void InitAStar(std::vector<PolyInfo> AStarPoly, int AStarPolyNum, std::vector<PolyInfo> WallPoly, int WallPolyNum); 
	void AI();                            // AI��Update 
	void Idle(); // ��~���̏���
	void ChangeTarget();                  // �ǂ������Ă���ۂɃ^�[�Q�b�g��ύX����֐�
	void AddSurvivor(Survivor* survivor); // �T�o�C�o�[��o�^����
	void Chase_Target();                  // �^�[�Q�b�g��ǂ�������
	void Search_Target();                 // �^�[�Q�b�g��T��
	void DecideNextCipherMachine();       // �Í��@�����񂷂�ۂ̎��̈Í��@�����߂�
	void ResetNextCipherMachine();        // �Í��@���񒆂̏������Z�b�g
	void CarryTarget_AI();                // �T�o�C�o�[���^��
	bool DecideRocketChair(bool startAStar); // �T�o�C�o�[���^�Ԑ�̃��P�b�g�`�F�A�����߂�
	void Attack_AI(float DisFromTarget);     // �U��������
	void AttackHit();
	void PickUpTarget();

	// �v���C���[
	void Player();
	void Move(); 
	void Move_Key();  // �L�[�{�[�h�p
	void Move_XBox(); // XBox�p
	void Attack_Player();
	void CarryTarget_Player();

	// ���ʊ֐�
	void UpdateCollider() override;

	inline void SetCipherMachineVec(std::vector<CipherMachine*> CipherMachineVec) { cipherMachineVec = CipherMachineVec; }
	inline void SetRocketChairVec(std::vector<RocketChair*> RocketChairVec) { rocketChairVec = RocketChairVec; }
	inline void SetExitPosVec(std::vector<VECTOR> ExitPosVec) { exitPosVec = ExitPosVec; }
	inline CapsuleCollider* GetAtkCollider() { return atkCollider; }
	inline void SetState(State_Hunter State) { prevState = state;  state = State; }
	inline State_Hunter GetState() { return state; }
	inline Survivor* GetTarget() { return target; }
	inline bool GetAStarNow() { return aStarNow; }
	void OnCollisionHit(Collider* Collider, GameObject* Parent) override;
};

