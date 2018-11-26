#pragma warning(disable:4996)
#pragma once
//Base of Box Class

//#include "GameEngine_Ver3_7.h"
#include "Collision.h"
//#include "MyPG.h"

enum BoxType
{
	Clear = -1, //�����Ȃ�
	Road = 0, //��
	Wall = 1, //��
	Goal = 2, //�S�[���ʒu
	Teleportaion = 3, //�e���|�[�e�[�V����
	LightSwitch = 4, //���C�g�̃X�C�b�`//�X�C�b�`�͐�΂ɍs���~�܂�ɐݒu���Ȃ�
	Obstacle3 = 5, //��Q��3��(����)

	Core = 40, //�^�񒆂̂ł����L���[�u
	Barrier = 50, //�O�ɔ�΂��Ȃ��悤�ɂ��铧���ȕ�
	Start = 60,//�X�^�[�g�ʒu
	//�ǉ��̗]�n����
};

//�{�b�N�X�̖�
//�����蔻��s�v�|���S���\���Ɏg�p����
enum Box_Side
{
	Xplus, Xminus,
	Yplus, Yminus,
	Zplus, Zminus,
};

class Bbox 
{
private:
	BoxType chip;//�}�X�̃}�b�v�`�b�v
	ML::Vec3 pos;//���S�_�̍��W
	ML::Box3D collision_Base;//���̔���͈�
	ML::QT boxQT;//�}�b�v�̉�]��
	Collision col;//�����蔻��R���|�l���g
	//�{�b�N�X��ID
	string box_Id;
	//�ق��̃{�b�N�X�Əd�Ȃ��Ďg���Ȃ��ʂ��m�F����t���O
	bool unusable_Triagle[12];
	
	
	//�����蔻��͈͂��O�p�`�ŕϊ�������
	//SUKA�v���O������Box3d����12�̎O�p�`�����o��
	void Get_Triangle_Box3D(std::vector<Triangle>* result, const ML::Box3D& box, const ML::QT& rotation) const;
	//Box3d�Ƌ��̂̍ŒZ�����̓_����� ���� : (�{�b�N�X�̂����蔻��͈�, ���̒��S, �����a)
	void Get_ShortisetPoints_Box_to_Sphere(std::vector<ML::Vec3>* result, const ML::Box3D& box) const;
	
public:

	//���\�b�h	
	//��]�֐��@���� : (��]�����ʒu , ��]��)
	void Rotate_Box(ML::Mat4x4* mat, const ML::QT& q);

	//�����蔻��ɕK�v�Ȃ��O�p�`��\�����Ă���
	void Marking_On_Unusable_Poligon(const Box_Side&);
	
	//BoxType���m�F����
	BoxType What_Type_Is_this_Box() const;
	//�ʒu��Ԃ��֐�
	const ML::Vec3& Get_Pos() const;
	//id��Ԃ��֐�
	string Get_Id() const;

	//�Փ˔���
	//����̌��ʒl�����炤�֐�
	bool Get_Collision_Poligon(std::vector<After_Collision>* result,std::vector<ML::Vec3> all_Points , const ML::Vec3& pos, const float& r, const ML::Vec3& speed);

	//�R���X�g���N�^�E�f�X�g���N�^
	//�����Ȃ��R���X�g���N�^(�[���N���A)
	Bbox();
	//���� : (���̃^�C�v,�ʒu,�����蔻���`,������]��,�{�b�N�X��ID)
	Bbox(const BoxType& chip, const ML::Vec3& pos, const ML::Box3D& base, const ML::QT& qt, const string id);
	//�R�s�[�R���X�g���N�^
	Bbox operator= (const Bbox& b)
	{		
		this->boxQT = b.boxQT;
		this->chip = b.chip;
		this->collision_Base = b.collision_Base;
		this->box_Id = b.box_Id;
		this->pos = b.pos;
		for (int i = 0; i < 12; i++)
		{
			this->unusable_Triagle[i] = b.unusable_Triagle[i];
		}
		this->col = Collision();

		return b;
	}

	~Bbox(){}
};