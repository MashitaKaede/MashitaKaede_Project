#include "Map.h"

Map::Map()

	: collisionManager(nullptr),
	  cipherMachineHandle(-1),
	  completeMachineNum(0),
	  decodingComplete(false),
	  doorHandle(-1),
	  exitPolyStartIndex(0),
	  keypadHandle(-1),
	  rocketChairHandle(-1),
	  wallPolyNum(0),
 	  mapHandle_AStar(-1),
	  mapWallHandle(-1)
{
}

Map::~Map()
{
	Term();
}

void Map::Init()
{
	// 経路探索用マップ
	mapHandle_AStar = MV1LoadModel("Assets/Model/Map/Map.mv1");

	// マップの壁
	mapWallHandle = MV1LoadModel("Assets/Model/Map/maptest3obj.mv1");

	// ライト
	SetLightEnable(false);
	lightHandleVec.push_back(CreateDirLightHandle(VGet(1.0f, -1.0f, 1.0f)));
	lightHandleVec.push_back(CreateDirLightHandle(VGet(-1.0f, -1.0f, -1.0f)));
	SetGlobalAmbientLight(GetColorF(0.8f, 0.8f, 0.8f, 1.0f));

	// 暗号機
	cipherMachineHandle = MV1LoadModel("Assets/Model/Object/CipherMachine/CipherMachine.mv1");
	posVec_CipherMachine = {
		VGet(300.0f, 0.0f, 3500.0f),
		VGet(1000, 0.0f, 1930.0f),
		VGet(1150.0f, 0.0f, 1150.0f),
		VGet(6100.0f, 0.0f, 2900.0f),
		VGet(4000.0f, 0.0f, 333.0f),
		VGet(4200.0f, 0.0f, 1100.0f),
		VGet(600, 0.0f, 230.0f)
	};

	cipherMachineVec.resize(Map_CipherMachineNum);

	for (int i = 0; i < Map_CipherMachineNum; i++)
	{
		CipherMachine* cipherMachine = new CipherMachine(MV1DuplicateModel(cipherMachineHandle), i);
		cipherMachine->position = posVec_CipherMachine[i];
		cipherMachine->Init();
		cipherMachineVec[i] = cipherMachine;
		collisionManager->AddCollider(cipherMachine);
	}

	// 出口
	doorHandle = MV1LoadModel("Assets/Model/Object/Door/Door.mv1");
	keypadHandle = MV1LoadModel("Assets/Model/Object/Keypad/Keypad.mv1");
	posVec_Exit = {
		VGet(3400.0f, 0.0f, -1050.0f),
		VGet(3400.0f, 0.0f, 4500.0f),
	};
	posVec_Exit_AStar = {
		VGet(3400.0f, 0.0f, -1800.0f),
		VGet(3400.0f, 0.0f, 5200.0f),
	};
	rotVec_Exit = {
		VGet(0.0f, Math::DegToRad(90.0f), 0.0f),
		VGet(0.0f, Math::DegToRad(90.0f), 0.0f),
	};
	exitVec.resize(Map_ExitNum);

	for (int i = 0; i < Map_ExitNum; i++)
	{
		Exit exit = {new Door(MV1DuplicateModel(doorHandle)), new Door(MV1DuplicateModel(doorHandle)), new Keypad(MV1DuplicateModel(keypadHandle), i)};
		exit.left->position = VGet(posVec_Exit[i].x - 120.0f, posVec_Exit[i].y, posVec_Exit[i].z);
		exit.left->rotation = rotVec_Exit[i];
		exit.left->Init();
		exit.right->position = VGet(posVec_Exit[i].x + 120.0f, posVec_Exit[i].y, posVec_Exit[i].z);
		exit.right->rotation = rotVec_Exit[i];
		exit.right->Init();
		if (i == 0)
		{
			exit.keypad->position = VGet(posVec_Exit[i].x + 120.0f + 150.0f, posVec_Exit[i].y + 100.0f, posVec_Exit[i].z + 100.0f);
			exit.keypad->rotation = VGet(0.0f, Math::DegToRad(90.0f), 0.0f);
		}
		else if(i == 1)
		{
			exit.keypad->position = VGet(posVec_Exit[i].x + 120.0f + 150.0f, posVec_Exit[i].y + 100.0f, posVec_Exit[i].z - 100.0f);
			exit.keypad->rotation = VGet(0.0f, Math::DegToRad(90.0f), 0.0f);
		}
		
		exit.keypad->Init();
		exitVec[i] = exit;
	}
	
	// ロケットチェア
	rocketChairHandle = MV1LoadModel("Assets/Model/Object/RocketChair/RocketChair.mv1");
	posVec_RocketChair = {
		VGet(2300.0f, 0.0f, 100.0f),
		VGet(1300.0f, 0.0f, 1000.0f),
		VGet(400.0f, 0.0f, 2000.0f),
		VGet(6500.0f, 0.0f, 3300.0f),
		VGet(6500.0f, 0.0f, 100.0f)
	};

	rocketChairVec.resize(Map_RocketChairNum);

	for (int i = 0; i < Map_RocketChairNum; i++)
	{
		RocketChair* rocketChair = new RocketChair(MV1DuplicateModel(rocketChairHandle), i);
		rocketChair->position = posVec_RocketChair[i];
		rocketChair->Init();
		rocketChairVec[i] = rocketChair;
		collisionManager->AddCollider(rocketChair);
	}

	PolyInit();

	runAwayPointVec = {
		VGet(6478.0f, 0.0f, 98.0f),
		VGet(6394.0f, 0.0f, 975.0f),
		VGet(6412.0f, 0.0f, 1829.0f),
		VGet(6340.0f, 0.0f, 2587.0f),
		VGet(6550.0f, 0.0f, 3356.0f),
		VGet(3458.0f, 0.0f, 3500.0f),
		VGet(3394.0f, 0.0f, 2578.0f),
		VGet(3432.0f, 0.0f, 1859.0f),
		VGet(3381.0f, 0.0f, 970.0f),
		VGet(3409.0f, 0.0f, 235.0f),
		VGet(376.0f, 0.0f, 172.0f),
		VGet(393.0f, 0.0f, 970.0f),
		VGet(278.0f, 0.0f, 2641.0f),
		VGet(295.0f, 0.0f, 3564.0f)
	};
}

void Map::PolyInit()
{
	// マップの壁
	WallPolyGet(mapWallHandle, -1);

	// 暗号機の台座
	for (int i = 0; i < Map_CipherMachineNum; i++)
	{
		WallPolyGet(cipherMachineVec[i]->GetModelHandle(), 1);
	}

	exitPolyStartIndex = wallPolyNum;

	// 出口
	for(int i = 0; i < Map_ExitNum; i++)
	{
		WallPolyGet(exitVec[i].left->GetModelHandle(), 0);
		WallPolyGet(exitVec[i].right->GetModelHandle(), 0);
	}

	AStarPolyGet(mapHandle_AStar, 0);
}

void Map::WallPolyGet(int handle, int frameIndex)
{
	// ステージモデル全体の参照用メッシュを構築する
	MV1SetupReferenceMesh(handle, frameIndex, true);

	// ステージモデル全体の参照用メッシュの情報を取得する
	MV1_REF_POLYGONLIST polyList = MV1GetReferenceMesh(handle, frameIndex, true);

	// ポリゴン数取得
	int polyNum = polyList.PolygonNum;

	// 配列拡張
	int vecSize = (int)polyVec_Wall.size();

	polyVec_Wall.resize(vecSize + polyNum);

	// ポリゴン頂点情報の取得
	MV1_REF_POLYGON* polygons = polyList.Polygons;

	// ポリゴン情報(法線と頂点座標と中心座標)取得
	for (int j = 0; j < polyNum; j++, polygons++)
	{
		// 頂点座標
		for (int k = 0; k < 3; k++)
		{
			polyVec_Wall[wallPolyNum].vertexPos[k] = polyList.Vertexs[polygons->VIndex[k]].Position;
		}

		// 中心座標(３頂点の座標足して1/3)
		VECTOR centerPos = VAdd(polyVec_Wall[wallPolyNum].vertexPos[0], polyVec_Wall[wallPolyNum].vertexPos[1]);
		centerPos = VAdd(centerPos, polyVec_Wall[wallPolyNum].vertexPos[2]);
		centerPos = VScale(centerPos, 1.0f / 3.0f);
		polyVec_Wall[wallPolyNum].centerPosition = centerPos;

		// 法線
		polyVec_Wall[wallPolyNum].normal = Math::GetPolygonNormal(polyVec_Wall[wallPolyNum].vertexPos[0], polyVec_Wall[wallPolyNum].vertexPos[1], polyVec_Wall[wallPolyNum].vertexPos[2]);

		// 法線が垂直に近いものは壁には入れない
		if (polyVec_Wall[wallPolyNum].normal.y > 0.1f || polyVec_Wall[wallPolyNum].normal.y < -0.1f)
		{
			continue;
		}
		
		// ポリゴン数カウント
		wallPolyNum++;
	}

	// 要らない部分削除
	if (wallPolyNum != polyVec_Wall.size())
	{
		polyVec_Wall.erase(polyVec_Wall.begin() + wallPolyNum, polyVec_Wall.end());
	}

	// 参照用メッシュの後始末をする
	MV1TerminateReferenceMesh(handle, frameIndex, true);
}

void Map::AStarPolyGet(int handle, int frameIndex)
{
	// ステージモデル全体の参照用メッシュを構築する
	MV1SetupReferenceMesh(handle, frameIndex, true);

	// ステージモデル全体の参照用メッシュの情報を取得する
	MV1_REF_POLYGONLIST polyList = MV1GetReferenceMesh(handle, frameIndex, true);

	// ポリゴン数取得
	int polyNum = polyList.PolygonNum;

	// 配列拡張
	int vecSize = (int)polyVec_AStar.size();

	polyVec_AStar.resize(vecSize + polyNum);
	polyVec.resize(polyNum);
	// ポリゴン頂点情報の取得
	MV1_REF_POLYGON* polygons = polyList.Polygons;

	for (int i = 0; i < polyNum; i++, polygons++)
	{
		polyVec[i] = polygons;
	}

	// ポリゴン情報(法線と頂点座標と中心座標)取得
	for (int j = 0; j < polyNum; j++)
	{
		// 頂点座標
		for (int k = 0; k < 3; k++)
		{
			polyVec_AStar[AStarPolyNum].vertexPos[k] = polyList.Vertexs[polyVec[j]->VIndex[k]].Position;
		}

		// 中心座標(３頂点の座標足して1/3)
		VECTOR centerPos = VAdd(polyVec_AStar[AStarPolyNum].vertexPos[0], polyVec_AStar[AStarPolyNum].vertexPos[1]);
		centerPos = VAdd(centerPos, polyVec_AStar[AStarPolyNum].vertexPos[2]);
		centerPos = VScale(centerPos, 1.0f / 3.0f);
		polyVec_AStar[AStarPolyNum].centerPosition = centerPos;

		// 法線
		polyVec_AStar[AStarPolyNum].normal = Math::GetPolygonNormal(polyVec_AStar[AStarPolyNum].vertexPos[0], polyVec_AStar[AStarPolyNum].vertexPos[1], polyVec_AStar[AStarPolyNum].vertexPos[2]);

		// 何番目のポリゴンか
		polyVec_AStar[AStarPolyNum].index = AStarPolyNum;

		// ポリゴン数カウント
		AStarPolyNum++;
	}

	// ポリゴンの隣接情報取得
	for (int i = 0; i < polyNum; i++)
	{
		// 各辺に隣接するポリゴンを無しにする
		polyVec_AStar[i].linkPolyIndex[0] = -1;
		polyVec_AStar[i].linkPolyIndex[1] = -1;
		polyVec_AStar[i].linkPolyIndex[2] = -1;

		for (int j = 0; j < polyNum; j++)
		{
			// 自分自身のポリゴンだったら何もせず次のポリゴンへ
			if (i == j)
			{
				continue;
			}

			// ポリゴンの頂点番号0と1で形成する辺と隣接していたら隣接情報に追加する
			if (polyVec_AStar[i].linkPolyIndex[0] == -1 &&
				((polyVec[i]->VIndex[0] == polyVec[j]->VIndex[0] && polyVec[i]->VIndex[1] == polyVec[j]->VIndex[2]) ||
				 (polyVec[i]->VIndex[0] == polyVec[j]->VIndex[1] && polyVec[i]->VIndex[1] == polyVec[j]->VIndex[0]) ||
				 (polyVec[i]->VIndex[0] == polyVec[j]->VIndex[2] && polyVec[i]->VIndex[1] == polyVec[j]->VIndex[1])))
			{
				polyVec_AStar[i].linkPolyIndex[0] = j;
				polyVec_AStar[i].linkPolyDistance[0] = VSize(VSub(polyVec_AStar[i].centerPosition, polyVec_AStar[j].centerPosition));
			}
			else
				// ポリゴンの頂点番号1と2で形成する辺と隣接していたら隣接情報に追加する
				if (polyVec_AStar[i].linkPolyIndex[1] == -1 &&
					((polyVec[i]->VIndex[1] == polyVec[j]->VIndex[0] && polyVec[i]->VIndex[2] == polyVec[j]->VIndex[2]) ||
					 (polyVec[i]->VIndex[1] == polyVec[j]->VIndex[1] && polyVec[i]->VIndex[2] == polyVec[j]->VIndex[0]) ||
					 (polyVec[i]->VIndex[1] == polyVec[j]->VIndex[2] && polyVec[i]->VIndex[2] == polyVec[j]->VIndex[1])))
				{
					polyVec_AStar[i].linkPolyIndex[1] = j;
					polyVec_AStar[i].linkPolyDistance[1] = VSize(VSub(polyVec_AStar[i].centerPosition, polyVec_AStar[j].centerPosition));
				}
				else
					// ポリゴンの頂点番号2と0で形成する辺と隣接していたら隣接情報に追加する
					if (polyVec_AStar[i].linkPolyIndex[2] == -1 &&
						((polyVec[i]->VIndex[2] == polyVec[j]->VIndex[0] && polyVec[i]->VIndex[0] == polyVec[j]->VIndex[2]) ||
					     (polyVec[i]->VIndex[2] == polyVec[j]->VIndex[1] && polyVec[i]->VIndex[0] == polyVec[j]->VIndex[0]) ||
						 (polyVec[i]->VIndex[2] == polyVec[j]->VIndex[2] && polyVec[i]->VIndex[0] == polyVec[j]->VIndex[1])))
					{
						polyVec_AStar[i].linkPolyIndex[2] = j;
						polyVec_AStar[i].linkPolyDistance[2] = VSize(VSub(polyVec_AStar[i].centerPosition, polyVec_AStar[j].centerPosition));
					}
		}
	}

	// 参照用メッシュの後始末をする
	MV1TerminateReferenceMesh(handle, frameIndex, true);
}

void Map::Term()
{
	lightHandleVec.clear();
	DeleteLightHandleAll();
	if (cipherMachineHandle != -1)
	{
		MV1DeleteModel(cipherMachineHandle);
	}
	for (int i = 0; i < Map_CipherMachineNum; i++)
	{
		delete cipherMachineVec[i];
	}
	for (int i = 0; i < Map_ExitNum; i++)
	{
		delete exitVec[i].left;
		delete exitVec[i].right;
		delete exitVec[i].keypad;
	}
	cipherMachineVec.clear();
	rocketChairVec.clear();
}

void Map::Update()
{
	// 暗号機
	int completeCnt = 0;
	for (int i = 0; i < Map_CipherMachineNum; i++)
	{
		cipherMachineVec[i]->Update();
		if (decodingComplete == false)
		{
			if (cipherMachineVec[i]->GetDecodingComplete())
			{
				completeCnt++;
				completeMachineNum = completeCnt;
			}
		}
	}

	if (completeMachineNum >= 5 && decodingComplete == false)
	{
		for (int i = 0; i < Map_CipherMachineNum; i++)
		{
			cipherMachineVec[i]->Decoding(100.0f);
			collisionManager->RemoveCollider(cipherMachineVec[i]);
		}
		completeMachineNum = Map_CipherMachineNum;

		for (int i = 0; i < Map_ExitNum; i++)
		{
			collisionManager->AddCollider(exitVec[i].keypad);
		}

		decodingComplete = true;
	}

	// 出口
	for (int i = 0; i < Map_ExitNum; i++)
	{
		exitVec[i].left->Update();
		exitVec[i].right->Update();
		exitVec[i].keypad->Update();

		if (exitVec[i].keypad->GetInputComplete() && exitVec[i].openDoorCnt <= Map_OpenDoorNum)
		{
			VECTOR leftMoveVec = VNorm(VSub(exitVec[i].left->position, exitVec[i].right->position));
			VECTOR rightMoveVec = VScale(leftMoveVec, -1.0f);
			exitVec[i].left->OpenDoor(leftMoveVec);
			exitVec[i].right->OpenDoor(rightMoveVec);

			// ゴールのドアのポリゴン情報だけ更新
			wallPolyNum = exitPolyStartIndex;
			for (int i = 0; i < Map_ExitNum; i++)
			{
				WallPolyGet(exitVec[i].left->GetModelHandle(), 0);
				WallPolyGet(exitVec[i].right->GetModelHandle(), 0);
			}
			// コリジョンマネージャーの方も更新
			collisionManager->SetPolyInfo_Wall(polyVec_Wall, (int)polyVec_Wall.size());

			exitVec[i].openDoorCnt++;
		}
	}

	// ロケットチェア
	for (int i = 0; i < Map_RocketChairNum; i++)
	{
		rocketChairVec[i]->Update();
	}

	Draw();
}

void Map::Draw()
{
	MV1DrawModel(mapHandle_AStar);
	MV1DrawModel(mapWallHandle);
}

void Map::Debug()
{
	for (int i = 0; i < Map_Debug_GridNum; i++)
	{
		for (int j = 0; j < Map_Debug_GridNum; j++)
		{
			DrawLine3D(VGet(0.0f, 0.0f, i * Map_Debug_GridDis), VGet(Map_Debug_GridDis * Map_Debug_GridNum, 0.0f, 0.0f), GetColor(255, 255, 255));
			DrawLine3D(VGet(i * Map_Debug_GridDis, 0.0f, 0.0f), VGet(0.0f, 0.0f, Map_Debug_GridDis * Map_Debug_GridNum), GetColor(255, 255, 255));
		}
	}

	for (int i = 0; i < wallPolyNum; i++)
	{
		DrawTriangle3D(polyVec_Wall[i].vertexPos[0], polyVec_Wall[i].vertexPos[1], polyVec_Wall[i].vertexPos[2], GetColor(0, 255, 0), false);
		DrawLine3D(polyVec_Wall[i].centerPosition, VAdd(polyVec_Wall[i].centerPosition, VScale(polyVec_Wall[i].normal, 100.0f)), GetColor(0, 0, 255));
	}

	for (int i = 0; i < Map_CipherMachineNum; i++)
	{
		cipherMachineVec[i]->Debug();
	}

	for (int i = 0; i < Map_ExitNum; i++)
	{
		exitVec[i].keypad->Debug();
	}
}
