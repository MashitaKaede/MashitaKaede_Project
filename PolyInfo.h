#pragma once
#include "DxLib.h"

// ポリゴンの確認状況
enum class Status
{
	None,
	Open,
	Close,
};

// ポリゴン情報の構造体
struct PolyInfo
{
	int linkPolyIndex[3];			// ポリゴンの三つの辺とそれぞれ隣接しているポリゴンのポリゴン番号( -1：隣接ポリゴン無し  -1以外：ポリゴン番号 )
	float linkPolyDistance[3];		// 隣接しているポリゴンとの距離
	VECTOR normal;                  // 法線ベクトル
	VECTOR vertexPos[3];            // 頂点座標3つ
	VECTOR centerPosition;			// ポリゴンの中心座標
	int parentPolyIndex{ -1 };      // 1個前の通ってきたポリゴン番号
	int index{ -1 };				// 何番目のポリゴン
	Status status{ Status::None };	// 確認状況
	float cost{ 0 };                // 実コスト（ここまでの距離）
	float cost_H{ 0 };				// ヒューリスティックコスト（ゴールまでの最短距離）
	float cost_T{ 0 };				// 合計コスト(実コスト＋ヒューリスティックコスト)
};