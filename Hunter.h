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
	// AI用
	SearchTarget,
	ChaseTarget,
	Attack,
	Restrain
};

class Hunter : public Character
{
private:
// プレイヤー、AI共通
	State_Hunter state;     // 状態
	State_Hunter prevState; // 前の状態
	Survivor* carryTarget;  // 運んでるサバイバー
	float pickUpTime;       // 持ち上げ始めてからの時間
	// 攻撃用
	CapsuleCollider* atkCollider; // 攻撃用コライダー
	bool atkCoolTimeNow; // 攻撃のクールタイム中か
	float stopTime;    // 停止してからの時間

// プレイヤー用

// AI用
	AStar* aStar;            // 経路探索用クラス
	Survivor* target;        // 追いかけるサバイバー
	CipherMachine* targetCM; // 目的の暗号機
	std::vector<Survivor*> survivorVec;           // サバイバー配列
	std::vector<CipherMachine*> cipherMachineVec; // 暗号機配列
	std::vector<int> check_CM_IndexVec;           // サバイバー捜索中に確認した暗号機の番号配列
	std::vector<VECTOR> exitPosVec;               // 出口の座標配列
	std::vector<RocketChair*> rocketChairVec;     // ロケットチェア配列
	int survivorNum;   // サバイバーの数
	int targetCMIndex; // 向かってる暗号機の番号
	int targetRCIndex; // 向かってるロケットチェアの番号
	bool goRC;         // ロケットチェアに向かう
	bool goCM;         // 暗号機に向かう
	bool chaseDirect;  // 直接追いかけるかどうか
	bool aStarNow;
public:
// プレイヤー、AI共通
	Hunter();
	~Hunter() override;
	void Init() override;
	void Term() override;
	void Update() override;
	void Debug() override;

// プレイヤー用
	void CheckMove();

// AI用
	// 経路探索用のポリゴン情報のセット
	void InitAStar(std::vector<PolyInfo> AStarPoly, int AStarPolyNum, std::vector<PolyInfo> WallPoly, int WallPolyNum); 
	void AI();                            // AIのUpdate 
	void Idle(); // 停止中の処理
	void ChangeTarget();                  // 追いかけている際にターゲットを変更する関数
	void AddSurvivor(Survivor* survivor); // サバイバーを登録する
	void Chase_Target();                  // ターゲットを追いかける
	void Search_Target();                 // ターゲットを探す
	void DecideNextCipherMachine();       // 暗号機を周回する際の次の暗号機を決める
	void ResetNextCipherMachine();        // 暗号機周回中の情報をリセット
	void CarryTarget_AI();                // サバイバーを運ぶ
	bool DecideRocketChair(bool startAStar); // サバイバーを運ぶ先のロケットチェアを決める
	void Attack_AI(float DisFromTarget);     // 攻撃をする
	void AttackHit();
	void PickUpTarget();

	// プレイヤー
	void Player();
	void Move(); 
	void Move_Key();  // キーボード用
	void Move_XBox(); // XBox用
	void Attack_Player();
	void CarryTarget_Player();

	// 共通関数
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

