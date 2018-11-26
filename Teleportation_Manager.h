#pragma once
#pragma warning(disable:4996)
#include "myLib.h"
//�u�Ԉړ����Ǘ�����N���X

//�h�A��1�ʂ�����1�ȉ������݂��āA�S�̐��͋����ő��݂���
//0<->3
//1<->4
//2<->5
//�̂悤�ɂȂ����Ă���

class Teleportation_Manager
{
private:
	//�u�Ԉړ��̔��̍��W
	ML::Vec3 doorPos[6];
	//�e���̎g�p���ꂽ���𔻕ʂ���t���O
	bool be_Used[6];
	//���Ԕ��ʗp�J�E���^
	int timeCnt[6];

public:
	//���W���X�V
	void Update_Door_Position(const int& sidenum, const ML::Vec3& pos);
	//�o����T���֐� ���� : (�����蔻�肪�N�����ʂ̔ԍ�)
	bool Find_Exit(const int& sidenum, ML::Vec3* exitpos);
	//�t���[����1���J�E���^���グ��֐�
	void Increse_Counter();

	//�R���X�g���N�^�E�f�X�g���N�^
	//�[���N���A
	Teleportation_Manager();	

	~Teleportation_Manager() 
	{}
};