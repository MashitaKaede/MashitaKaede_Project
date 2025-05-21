#pragma once
// Debug
constexpr int Debug_Dis_Y = 16; // デバック表示される文の距離（Y）
extern int Debug_DispCnt;       // デバック表示される文の数

// Time
constexpr int Time_CheckFrameCnt = 100; // 何フレームあたりの平均フレームタイムを取るか
constexpr float Time_DefaultFps = 60.0f;
extern float FPS;
extern float FrameTime;

// コントローラー
constexpr int XBox_Invalid_Range = 10000;

// マップ
constexpr int Map_Debug_GridNum = 100;
constexpr float Map_Debug_GridDis = 1000.0f;
constexpr int Map_CipherMachineNum = 7;
constexpr int Map_RocketChairNum = 5;
constexpr int Map_ExitNum = 2;
constexpr int Map_OpenDoorNum = 400;

//---------------オブジェクト----------------//
// 暗号機
constexpr float CipherMachine_ColliderRadius = 90.0f;
constexpr float CipherMachine_ColliderHeight = 130.0f;

// きーぱっど
constexpr float KeyPad_ColliderRadius = 80.0f;
constexpr float KeyPad_ColliderHeight = 130.0f;

// ロケットチェア
constexpr float RocketChair_ColliderRadius = 90.0f;
constexpr float RocketChair_ColliderHeight = 130.0f;
//------------------------------------------//

// カメラ
constexpr float Camera_TargetDis = 500.0f;
constexpr float Camera_TargetHeight = 300.0f;
constexpr float Camera_RotateSpeed_X = 3.0f;
constexpr float Camera_RotateSpeed_Y = 1.5f;
constexpr float Camera_Max_AngleV = 89.0f;
constexpr float Camera_Min_AngleV = -89.0f;

//-------------- キャラクター-----------------//
constexpr float Character_Default_ColliderRadius = 50.0f;
constexpr float Character_Default_ColliderHeight = 200.0f;

//----サバイバー----//
constexpr float Survivor_Default_MoveSpeed = 500.0f;
constexpr float Survivor_Default_AddDecodingRate = 0.1f;
constexpr float Survivor_Default_AddInputRate = 0.3f;
constexpr float Survivor_Default_AddRescueRate = 1.4f;
constexpr float Survivor_FallMoveSpped = 100.0f;
constexpr float Survivor_Default_AddGameOverRate = 0.03f;
constexpr float Survivor_Max_FlyingHeight = 3000.0f;
constexpr float Survivor_StartRunAway_Dis = 1500.0f;
constexpr float Survivor_EndRunAway_Dis = 2500.0f;
constexpr float Survivor_AStar_GoNextPoint_Dis = 50.0f;
constexpr float Survivor_FlyingSpeed = 15.0f;

// 庭師
//-----------------//

// ハンター
constexpr float Hunter_Default_MoveSpeed = 750.0f;
constexpr float Hunter_StartChaseSurvivorDis = 2000.0f;
constexpr float Hunter_MissSurvivorDis = 3000.0f;
constexpr float Hunter_MinDis_Survivor = 100.0f;
// 攻撃系
constexpr float Hunter_AtkRadius = 80.0f;
constexpr float Hunter_AtkRange = 200.0f;
constexpr float Hunter_AtkHeight = 150.0f;
constexpr float Hunter_AtkCoolTime = 1.0f;
constexpr float Hunter_ContinueAtkCollisionTime = 0.5f;

constexpr float Hunter_PickUpTragetTime = 2.0f;

//-------------------------------------------//

// アニメーション
constexpr float Animation_Default_PlaySpeed = 25.0f;
constexpr float Animation_BlendRateSpeed = 10.0f;

// 当たり判定
constexpr float Collision_Line_AddHeight = 50.0f;

// UI
constexpr int UI_SurvivorIconPosDis_X = 150.0f;
constexpr int UI_SurvivorIconPosDis_Y = 50.0f;