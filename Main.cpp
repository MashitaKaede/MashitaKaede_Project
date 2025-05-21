#include "DxLib.h"
#include "Time.h"
#include "SceneManager.h"
#include "Input.h"
#include "GameManager.h"
#include "SoundManager.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

    ChangeWindowMode(TRUE);
    SetGraphMode(1920, 1080, 32);
    SetUseDirect3DVersion(DX_DIRECT3D_11);
    // DXライブラリの初期化
    if (DxLib_Init() == -1) {
        return -1;
    }
    SetMouseDispFlag(false);
    SetBackgroundColor(0, 0, 0);
    SetDrawScreen(DX_SCREEN_BACK);
    // Ｚバッファを有効にする
    SetUseZBuffer3D(TRUE);

    // Ｚバッファへの書き込みを有効にする
    SetWriteZBuffer3D(TRUE);

    Time* timePtr = new Time();
    SoundManager::Init();
    SoundManager::GetInstance()->LoadSound();
    SceneManager::Init();
    SceneManager::GetInstance()->SetTime(timePtr);
    SceneManager::GetInstance()->LoadScene(SceneName::Title);
    Input::Init();
    Input::GetInstance()->KeyInfoInit();
    Input::GetInstance()->ButtonInfoInit();
    GameManager::Init();
    GameManager::GetInstance()->LoadImages();

    // メインループ
    while (ProcessMessage() == 0) 
    {
        ClearDrawScreen();

        timePtr->Update();
        Input::GetInstance()->Update();
        SceneManager::GetInstance()->Update();

        /*Debug_DispCnt = 0;
        timePtr->Debug();
        Input::GetInstance()->Debug();
        SceneManager::GetInstance()->Debug();*/

        // 画面更新
        ScreenFlip();

        // 終了条件
        if (CheckHitKey(KEY_INPUT_ESCAPE)) {
            break;
        }
    }

    delete timePtr;
    Input::Term();
    SceneManager::Term();
    GameManager::Term();
    SoundManager::Term();
    DxLib_End();
    return 0;
}