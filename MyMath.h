#pragma once
#pragma warning(disable:4996)
#include "myLib.h"

#define ROOT3 1.732050f

//���ρE�O�ς��s���l�[���X�y�[�X
namespace MyMath
{
	//���ρA�O��
	void Vector_Dot(float* result, const ML::Vec3& v1, const ML::Vec3& v2);//�����F�ia,b)�@a�Eb
	void Vector_Cross(float* result, const ML::Vec3& v1, const ML::Vec3& v2);//�����F�ia,b)�@a X b
	void Get_Normal_to_Vector_Cross(ML::Vec3* normal, const ML::Vec3& v1, const ML::Vec3& v2);//�@���x�N�g�������߂�֐�
	void Get_Vector_Dot(float* result, const ML::Vec3& v1, const ML::Vec3& v2);//���ς����߂�֐�
};