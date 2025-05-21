#pragma once
#include "Character.h"
#include "CipherMachine.h"
#include "Keypad.h"
#include "PolyInfo.h"
#include "AStar.h"
#include "SoundManager.h"

class Hunter;
class RocketChair;
enum class State_Survivor
{
// �v���C���[�AAI����
	None, // ��������
	Idle, // ��~��
	Move, // �ړ���
	Fall,   // �_�E����
	LiftedUp, // �S����Ă�
	Restrainting, // �߂܂��Ă���
	Decoding,     // ��ǒ�
	InputPassward,// �p�X���[�h����
	Rescue,   // �~����
    Flying,   // ��΂��ꒆ
	GameOver, // �Q�[���I�[�o�[
	GameClear,// �Q�[���N���A

// AI�p
    GoCipherMachine, // �Í��@�Ɍ�����
	GoKeyPad, // �L�[�p�b�h�Ɍ�����
	GoRescue, // �~���Ɍ������Ă�r��
	RunAway,  // ������
	GoExit    // �o���Ɍ�����
};

class Survivor : public Character
{
private:
// �v���C���[�AAI����
	State_Survivor state;     // ���
	CipherMachine* decodingCipherMachine; // ��ǒ��̈Í��@
	Keypad* inputKeyPad;                  // ���͒��̃L�[�p�b�h
	RocketChair* rocketChair;             // �߂܂��Ă�Ƃ��̃��P�b�g�`�F�A
	Hunter* hunter;                       // �n���^�[
	Survivor* rescueSurvivor; // �~���Ώۂ̃T�o�C�o�[
	int hp;                   // HP
	int restraintCnt;   // �߂܂�����
	int decodingCMIndex;
	float gameOverRate; // 100%�ŃQ�[���I�[�o�[
	float addDecodingRate;    // �Í��@�̉�Ǒ��x
	float addInputRate;       // �p�X���[�h���͑��x
	bool isVulnerable; // �U�����󂯂��Ԃ�
	bool isRescuing; // �~�����󂯂Ă��邩

// �v���C���[�p
	bool actionDisabled; // �s���s�\�t���O

// AI�p
	AStar* aStar;             // �o�H�T���p
	CipherMachine* targetCipherMachine;   // �������Ă�Í��@
	Keypad* targetKeyPad;                 // �������Ă���L�[�p�b�h
	std::vector<CipherMachine*> cipherMachineVec; // �Í��@�z��
	std::vector<Keypad*> keyPadVec;				  // �L�[�p�b�h�z��
	std::vector<Survivor*> otherSurvivorVec;      // ���̃T�o�C�o�[�z��
	std::vector<VECTOR> posVec_Exit_AStar;        // �E�o����ۂ̖ڕW���W�z��
	std::vector<VECTOR> runAwayPointVec;          // �n���^�[���瓦����ۂɎg���ڕW���W�z��
	int inputKeyPadIndex;                 // ���͒��̃L�[�p�b�h�ԍ�
	bool decodingComplete;    // �K�v������Ǌ���
	int runAwayPointIndex;
	bool aStarNow;
public:
// �v���C���[�AAI���ʊ֐�
	Survivor();            // �R���X�g���N�^
	~Survivor() override;
	void DefaultSurvivorUpdate(); // ��{�I�ȃT�o�C�o�[�̏���
	void Decoding();       // ���
	void InputPassward();  // ����
	void Restrainting();   // �S����
	void Flying();         // ��΂����
	void RescueSurvivor(); // �~��
	void AddRestraintCnt();// �߂܂����񐔂𑝂₵gameOverRate�𒲐�
	void ChangeSpeed();    // �X�e�[�g�ɉ������X�s�[�h�ɕύX
	void CheckGameClear(); // �N���A����
	void ResetActionTargets(); // �s���Ώۂ̃|�C���^�̃��Z�b�g
	void Debug() override; // �f�o�b�N�p
	void Reset();
  // �����蔻��
	void OnCollisionHit(Collider* Collider, GameObject* Parent) override;

	inline void AddHP(int AddAmount) { hp += AddAmount; } // ��

  // �Q�b�g�Z�b�g
	// �X�e�[�g
	inline State_Survivor GetState() { return state; } 
	inline void SetState(State_Survivor State) { state = State; }
	// �߂܂��Ă郍�P�b�g�`�F�A
	inline RocketChair* GetRocketChair() { return rocketChair; }
	inline void SetRocketChair(RocketChair* RC) { rocketChair = RC; }
	// �n���^�[
	inline void SetHunter(Hunter* HunterPtr) { hunter = HunterPtr; }
	// �~�����󂯂Ă��邩
	inline void SetIsRescuing(bool Flag) { isRescuing = Flag; }

// �v���C���[�p�֐�
	void Player(); // Player��Update�֐�

	void Move();      // �ړ��֐�(������܂Ƃ߂��֐�)
	void Move_Key();  // �L�[�{�[�h�p
	void Move_XBox(); // XBox�p

	void CheckMove(); // �ړ����邩�`�F�b�N
	void CheckIdle(); // ��~���邩�`�F�b�N

// AI�p�֐�
	void AI();            // AI��Update�֐�
	void CheckGoRescue(); // �~���ɍs�����̃`�F�b�N
	void CheckRunAway();  // �����邩�̃`�F�b�N
	void Idle();           // ��~���̏���
	void DecideDecodigCipherMachine(); // ��ǂ���Í��@�����߂�
	void GoTargetCipherMachine();      // ���߂��Í��@�Ɍ�����
	void DecideInputKeyPad();          // ���͂���L�[�p�b�h�����߂�
	void GoTargetKeyPad();             // ���߂��L�[�p�b�h�Ɍ�����
	void Escape();                     // �E�o
	void GoRescue();                   // �~���Ɍ�����
	void DecideRunAwayRoot();          // �����o�H�����߂�
	void RunAway();                    // ������
	
  // �Q�b�g�Z�b�g
    // �o�H�T���p�̃|���S�����̃Z�b�g
	void InitAStar(std::vector<PolyInfo> AStarPoly, int AStarPolyNum, std::vector<PolyInfo> WallPoly, int WallPolyNum);
	// �Í��@�z��
	inline void SetCipherMachineVec(std::vector<CipherMachine*> CipherMachineVec) { cipherMachineVec = CipherMachineVec; }
	// �L�[�p�b�h�z��
	inline void AddKeyPad(Keypad* KeyPadPtr) { keyPadVec.push_back(KeyPadPtr); }
	// ���̃T�o�C�o�[�z��
	inline void AddOtherSurvivor(Survivor* Other) { otherSurvivorVec.push_back(Other); }
	// �E�o����ۂ̖ڕW���W�z��
	inline void SetPosVec_Exit_AStar(std::vector<VECTOR> PosVec_Exit_AStar) { posVec_Exit_AStar = PosVec_Exit_AStar; }
	// �n���^�[���瓦����ۂɎg���ڕW���W�z��
	inline void SetRunAwayPointVec(std::vector<VECTOR> RunAwayPointVec) { runAwayPointVec = RunAwayPointVec; };
	// �K�v������Ǌ���
	inline void SetDecodingComplete(bool Flag) { decodingComplete = Flag; }

	inline bool GetAstarNow() { return aStarNow; }

	inline int GetHp() { return hp; }

	inline float GetGameOverRate() { return gameOverRate; }

};