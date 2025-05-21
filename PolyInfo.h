#pragma once
#include "DxLib.h"

// �|���S���̊m�F��
enum class Status
{
	None,
	Open,
	Close,
};

// �|���S�����̍\����
struct PolyInfo
{
	int linkPolyIndex[3];			// �|���S���̎O�̕ӂƂ��ꂼ��אڂ��Ă���|���S���̃|���S���ԍ�( -1�F�אڃ|���S������  -1�ȊO�F�|���S���ԍ� )
	float linkPolyDistance[3];		// �אڂ��Ă���|���S���Ƃ̋���
	VECTOR normal;                  // �@���x�N�g��
	VECTOR vertexPos[3];            // ���_���W3��
	VECTOR centerPosition;			// �|���S���̒��S���W
	int parentPolyIndex{ -1 };      // 1�O�̒ʂ��Ă����|���S���ԍ�
	int index{ -1 };				// ���Ԗڂ̃|���S��
	Status status{ Status::None };	// �m�F��
	float cost{ 0 };                // ���R�X�g�i�����܂ł̋����j
	float cost_H{ 0 };				// �q���[���X�e�B�b�N�R�X�g�i�S�[���܂ł̍ŒZ�����j
	float cost_T{ 0 };				// ���v�R�X�g(���R�X�g�{�q���[���X�e�B�b�N�R�X�g)
};