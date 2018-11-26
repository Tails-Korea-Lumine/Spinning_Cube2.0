#pragma once
#pragma warning(disable:4996)
#include "MyMath.h"

//�d�̓N���X
//---------------------------------------------------------------

class Gravity
{
private:
	const ML::Vec3 G_acceleration;

public:
	//����������
	void Accelerate(ML::Vec3* speed, const float& Weight) const;
	//�ǂƂԂ�������̈ړ������Ƒ��x��Ԃ�
	void Diagonal_Accelerate(ML::Vec3* speed, const ML::Vec3& normal) const;

	//���˃x�N�g�������߂�
	void Reflaction_Vector(ML::Vec3* force, const ML::Vec3& normal) const;


//�������Ŏ�
	Gravity() : G_acceleration(0,-9.8f/60.0f/60.0f,0)
	{

	}

	virtual ~Gravity()
	{

	}
};