#pragma once
#include "myLib.h"
#include "MyMath.h"

//�O�p�`�̊�{���
//���_�Ɩ@���x�N�g��
struct Triangle
{
	ML::Vec3 a, b, c;
	ML::Vec3 normal;
};

//�����蔻��̌�ɕԂ��\����
struct After_Collision
{
	bool collision_Flag;
	string collision_Id;
	ML::Vec3 normal;

	After_Collision()
	{
		this->collision_Flag = false;
		this->collision_Id = "";
		this->normal = ML::Vec3(0, 0, 0);
	}

	//��r���Z�q�I�[�o�[���[�h
	bool operator==(const After_Collision& ac)
	{
		float check;
		MyMath::Vector_Dot(&check, this->normal, ac.normal);
		return check >= _CMATH_::cosf(ML::ToRadian(359));
	}

	bool operator!=(const After_Collision& ac)
	{
		return *this == ac ? false : true;
	}
};

class Collision
{
private:
	//�����蔻���l
	const float judge;
	//���̃t���[���ɐ������������̂����m�F����t���O
	bool pricision_Flag;

	//���\�b�h	
	//��̓_�ƎO�p�`�̏Փ˔�����s��
	bool Check_Collision(const Triangle& tri, const ML::Vec3& p) const;

public:
	//���ۂق��̃v���O�����ŌĂяo���֐�

	//���̂Ɨ����̏Փ˔���֐�
	//�����F(���ʂ�ۑ�����std::vector�̃A�h���X�A���肷��}�X�̎O�p�`�A�{�b�N�X�̒��S�_�A�{�b�N�X�̒����A�{�[���̊O�p�h�b�g�A�{�[���̒��S�_�A�{�[���̔����a�A�{�[���̈ړ��x�N�g���A���[���h��]��)
	bool Hit_Check(std::vector<After_Collision>* result,const std::vector<Triangle>& all_Tri, const ML::Vec3 box_Center, const float& box_Length, std::vector<ML::Vec3>& all_Points, const ML::Vec3& ball_Pos, const float& r, const ML::Vec3& speed);
	
	
	Collision() :judge(_CMATH_::cosf(ML::ToRadian(359)))
	{
		this->pricision_Flag = false;
	}

	Collision operator= (const Collision& c)
	{
		return c;
	}
};

//static Collision col = Collision();