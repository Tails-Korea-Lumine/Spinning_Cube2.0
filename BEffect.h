#pragma once
#pragma warning(disable : 4996)
#include "myLib.h"

//-------------------------------------------------------------------------
//�G�t�F�N�g��b�N���X
//-------------------------------------------------------------------------
//�O��Ŏg�p���Ă�����̍�i�ɂ͎g�p���Ȃ��f�[�^���܂܂�Ă���


class BEffect 
{	

public:
	//�ϐ�
	//---------------------------------------------------------------------------------------------
	//�G�t�F�N�g�n���h��
	//0�ԁF�L�����N�^����
	//1�ԁF�e���|�[�g�A�E�g
	//2�ԁF�e���|�[�g�C��
	//3�ԁF�G�l�~�[���n
	//4�ԁF�G�l�~�[�ƏՓ�
	//5�ԁF�ǂƏՓ�
	//6�ԁF�����ې���
	//7�ԁF�����ۏ���
	//8�ԁF�����ۂƏՓ�
	//9�ԁF�N���A�G�t�F�N�g
	//---------------------------------------------------------------------------------------------
	enum effType
	{
		CLEAR = -1,
		CreateCharactor = 0,
		TeleportOut,
		Teleportin,
		EnemyLanding,
		Hit_to_Enemy,
		Hit_to_Wall,
		CreateTail,
		DestroyTail,
		Hit_to_Tail,
		Game_Clear,
	};
	effType playing_EffectHandle;
	int effect_Life;
	
	ML::Vec3 pos;//�G�t�F�N�g�Đ��ʒu
	ML::Vec3 target;//�^�[�Q�b�g�ʒu(����ꍇ�������炤)
	ML::Vec3 angle;//�G�t�F�N�g�Đ��p�x
	ML::Vec3 scale;//�G�t�F�N�g�̑傫��
	ML::Vec3 moveVec;//�G�t�F�N�g�̈ړ����x
	float alpha;//�����x

	string meshName;//���b�V���̖��O
	//string filePath;//���b�V���̃p�X



	//���\�b�h
	//�������Ń��b�V����ǂݍ��ފ֐�
	void LoadEffect(const effType& handle);

	//���������\�b�h
	//���̏�őS����������G�t�F�N�g
	void Eff_Initialize(const ML::Vec3& pos, const ML::Vec3& angle);
	//�ړI�n�܂ňړ����Ȃ��珈������G�t�F�N�g
	void Eff_Initialize(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle);

	//��������
	//�[���N���A
	BEffect() :
		pos(0, 0, 0),
		target(0,0,0),
		angle(0, 0, 0),
		scale(0,0,0),
		moveVec(0,0,0),
		playing_EffectHandle(CLEAR),
		effect_Life(0),
		alpha(1)
	{
		
	}

	virtual ~BEffect(){}
};