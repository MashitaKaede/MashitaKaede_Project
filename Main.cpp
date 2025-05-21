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
    // DX���C�u�����̏�����
    if (DxLib_Init() == -1) {
        return -1;
    }
    SetMouseDispFlag(false);
    SetBackgroundColor(0, 0, 0);
    SetDrawScreen(DX_SCREEN_BACK);
    // �y�o�b�t�@��L���ɂ���
    SetUseZBuffer3D(TRUE);

    // �y�o�b�t�@�ւ̏������݂�L���ɂ���
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

    // ���C�����[�v
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

        // ��ʍX�V
        ScreenFlip();

        // �I������
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