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
// プレイヤー、AI共通
	None, // 何も無し
	Idle, // 停止中
	Move, // 移動中
	Fall,   // ダウン中
	LiftedUp, // 担がれてる
	Restrainting, // 捕まっている
	Decoding,     // 解読中
	InputPassward,// パスワード入力
	Rescue,   // 救助中
    Flying,   // 飛ばされ中
	GameOver, // ゲームオーバー
	GameClear,// ゲームクリア

// AI用
    GoCipherMachine, // 暗号機に向かう
	GoKeyPad, // キーパッドに向かう
	GoRescue, // 救助に向かってる途中
	RunAway,  // 逃走中
	GoExit    // 出口に向かう
};

class Survivor : public Character
{
private:
// プレイヤー、AI共通
	State_Survivor state;     // 状態
	CipherMachine* decodingCipherMachine; // 解読中の暗号機
	Keypad* inputKeyPad;                  // 入力中のキーパッド
	RocketChair* rocketChair;             // 捕まってるときのロケットチェア
	Hunter* hunter;                       // ハンター
	Survivor* rescueSurvivor; // 救助対象のサバイバー
	int hp;                   // HP
	int restraintCnt;   // 捕まった回数
	int decodingCMIndex;
	float gameOverRate; // 100%でゲームオーバー
	float addDecodingRate;    // 暗号機の解読速度
	float addInputRate;       // パスワード入力速度
	bool isVulnerable; // 攻撃を受ける状態か
	bool isRescuing; // 救助を受けているか

// プレイヤー用
	bool actionDisabled; // 行動不能フラグ

// AI用
	AStar* aStar;             // 経路探索用
	CipherMachine* targetCipherMachine;   // 向かってる暗号機
	Keypad* targetKeyPad;                 // 向かっているキーパッド
	std::vector<CipherMachine*> cipherMachineVec; // 暗号機配列
	std::vector<Keypad*> keyPadVec;				  // キーパッド配列
	std::vector<Survivor*> otherSurvivorVec;      // 他のサバイバー配列
	std::vector<VECTOR> posVec_Exit_AStar;        // 脱出する際の目標座標配列
	std::vector<VECTOR> runAwayPointVec;          // ハンターから逃げる際に使う目標座標配列
	int inputKeyPadIndex;                 // 入力中のキーパッド番号
	bool decodingComplete;    // 必要数分解読完了
	int runAwayPointIndex;
	bool aStarNow;
public:
// プレイヤー、AI共通関数
	Survivor();            // コンストラクタ
	~Survivor() override;
	void DefaultSurvivorUpdate(); // 基本的なサバイバーの処理
	void Decoding();       // 解読
	void InputPassward();  // 入力
	void Restrainting();   // 拘束中
	void Flying();         // 飛ばされる
	void RescueSurvivor(); // 救助
	void AddRestraintCnt();// 捕まった回数を増やしgameOverRateを調整
	void ChangeSpeed();    // ステートに応じたスピードに変更
	void CheckGameClear(); // クリア判定
	void ResetActionTargets(); // 行動対象のポインタのリセット
	void Debug() override; // デバック用
	void Reset();
  // 当たり判定
	void OnCollisionHit(Collider* Collider, GameObject* Parent) override;

	inline void AddHP(int AddAmount) { hp += AddAmount; } // 回復

  // ゲットセット
	// ステート
	inline State_Survivor GetState() { return state; } 
	inline void SetState(State_Survivor State) { state = State; }
	// 捕まってるロケットチェア
	inline RocketChair* GetRocketChair() { return rocketChair; }
	inline void SetRocketChair(RocketChair* RC) { rocketChair = RC; }
	// ハンター
	inline void SetHunter(Hunter* HunterPtr) { hunter = HunterPtr; }
	// 救助を受けているか
	inline void SetIsRescuing(bool Flag) { isRescuing = Flag; }

// プレイヤー用関数
	void Player(); // PlayerのUpdate関数

	void Move();      // 移動関数(下二つをまとめた関数)
	void Move_Key();  // キーボード用
	void Move_XBox(); // XBox用

	void CheckMove(); // 移動するかチェック
	void CheckIdle(); // 停止するかチェック

// AI用関数
	void AI();            // AIのUpdate関数
	void CheckGoRescue(); // 救助に行くかのチェック
	void CheckRunAway();  // 逃げるかのチェック
	void Idle();           // 停止中の処理
	void DecideDecodigCipherMachine(); // 解読する暗号機を決める
	void GoTargetCipherMachine();      // 決めた暗号機に向かう
	void DecideInputKeyPad();          // 入力するキーパッドを決める
	void GoTargetKeyPad();             // 決めたキーパッドに向かう
	void Escape();                     // 脱出
	void GoRescue();                   // 救助に向かう
	void DecideRunAwayRoot();          // 逃走経路を決める
	void RunAway();                    // 逃げる
	
  // ゲットセット
    // 経路探索用のポリゴン情報のセット
	void InitAStar(std::vector<PolyInfo> AStarPoly, int AStarPolyNum, std::vector<PolyInfo> WallPoly, int WallPolyNum);
	// 暗号機配列
	inline void SetCipherMachineVec(std::vector<CipherMachine*> CipherMachineVec) { cipherMachineVec = CipherMachineVec; }
	// キーパッド配列
	inline void AddKeyPad(Keypad* KeyPadPtr) { keyPadVec.push_back(KeyPadPtr); }
	// 他のサバイバー配列
	inline void AddOtherSurvivor(Survivor* Other) { otherSurvivorVec.push_back(Other); }
	// 脱出する際の目標座標配列
	inline void SetPosVec_Exit_AStar(std::vector<VECTOR> PosVec_Exit_AStar) { posVec_Exit_AStar = PosVec_Exit_AStar; }
	// ハンターから逃げる際に使う目標座標配列
	inline void SetRunAwayPointVec(std::vector<VECTOR> RunAwayPointVec) { runAwayPointVec = RunAwayPointVec; };
	// 必要数分解読完了
	inline void SetDecodingComplete(bool Flag) { decodingComplete = Flag; }

	inline bool GetAstarNow() { return aStarNow; }

	inline int GetHp() { return hp; }

	inline float GetGameOverRate() { return gameOverRate; }

};