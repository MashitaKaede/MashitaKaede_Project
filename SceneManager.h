#pragma once
#include "Input.h"
#include "Time.h"
#include "SceneBase.h"
#include "Scene_Title.h"
#include "Scene_SideSelect.h"
#include "Scene_Main.h"
#include "Scene_Result.h"
#include "DxLib.h"
#include "Define.h"
#include <vector>

enum SceneName
{
	Title,
	SideSelect,
	Main,
	Result,
	SceneNum,
};

class SceneManager
{
private:
	static SceneManager* instance;
	SceneName nowSceneName = SceneName::Title;
	SceneBase* nowScene;
	Time* time;
	SceneManager();
	~SceneManager();
public:
	static inline SceneManager* GetInstance() { return instance; }
	static void Init();
	static void Term();
	void LoadScene(SceneName LoadSceneName);
	void Update();
	void Debug();
	inline void SetTime(Time* TimePtr) { time = TimePtr; }
};