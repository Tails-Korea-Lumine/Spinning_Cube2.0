#include "Collision.h"


bool Collision::Check_Collision(const Triangle& tri, const ML::Vec3& p) const
{
	//�Ƃ���_p�ƎO�p�`�̓����蔻��
	//p����O�p�`�̊e���_�ւ̃x�N�g��A,B,C
	//����A,B,C���\������p�x�̘a��360	��������
	//�_���͎O�p�`�̕ӂ̏�܂��͓���Ă���B

	ML::Vec3 A, B, C;
	A = tri.a - p;
	B = tri.b - p;
	C = tri.c - p;

	//�O�p�`�̒��_�Ɠ������Ȃ����ꍇ
	//if (A.Length() == 0.0f || B.Length() == 0.0f || C.Length() == 0.0f)
	if(A.Is_Zero_Vec() || B.Is_Zero_Vec() || C.Is_Zero_Vec())
	{
		return true; //�v�Z�s�\���O�p�`�ɓ����Ă���̂ł������Ă锻��ŕԂ�
	}	
	

	//alpha �� A��B�̊p�x
	//beta = B��C�̊p�x
	//gamma = A��C�̊p�x
	float cosAlpha;
	float cosBeta;
	float cosGamma;
	float sinAlpha;
	float sinBeta;
	float sinGamma;

	MyMath::Vector_Dot(&cosAlpha,A, B);
	MyMath::Vector_Dot(&cosBeta, B, C);
	MyMath::Vector_Dot(&cosGamma, A, C);
	MyMath::Vector_Cross(&sinAlpha, A, B);
	MyMath::Vector_Cross(&sinBeta, B, C);
	MyMath::Vector_Cross(&sinGamma, A, C);

	float check;

	//cos(alpha + beta + gamma)�̓W�J��
	check = (cosGamma*((cosAlpha * cosBeta) - (sinAlpha * sinBeta))) - (sinGamma * ((cosAlpha*sinBeta) + (sinAlpha*cosBeta)));

	//�덷�܂Ŋm�F(�덷�͈͕̔͂ς��]�n������ 2018/03/16)
	//0.99756405026
	return check >= this->judge;
}

//�}�X�ʂɌĂ΂��֐�
bool Collision::Hit_Check(std::vector<After_Collision>* result, const std::vector<Triangle>& all_Tri, const ML::Vec3 box_Center, const float& box_Length, std::vector<ML::Vec3>& all_Points, const ML::Vec3& ball_Pos, const float& r, const ML::Vec3& speed)
{		
	//�R���X�g���N�^�ɂ���ă[���x�N�g����false�Ő��������
	After_Collision collision_True;


	//�ȑO�ɔ��f�������Ƃɑ΂��ď�����ʂɂ���
	//�ȑO�̔���ŏՓ˂��N����\�����������̂Ő������������
	if (this->pricision_Flag)
	{	
		//�Փ˔���X�^�[�g
		for (const auto& tri : all_Tri)
		{			
			//�}�X�ƃ}�X�ڐG�ʂł�������������h��
			//�ړ��x�N�g���ƏՓ˂����O�p�`�̖@���x�N�g����cos�l
			float cosSN;
			MyMath::Vector_Dot(&cosSN, speed, tri.normal);
			//cos�l��1�Ƃ������Ƃ͓��p��0�x���Ƃ������ƁA�܂蕨���I�ɂ��蓾�Ȃ��Փ�
			//�������̂��߂Ɍ덷�͈͂܂Ŋm�F
			if (cosSN >= this->judge)
			{
				//�Ȃ̂Ŗ�������
				continue;
			}

			collision_True = After_Collision();
			for (const auto& p : all_Points)
			{
				//����ȊO�̏ꍇ�ł����蔻����s��
				if (this->Check_Collision(tri, p))
				{
					//�ȉ����������O�p�`�̖@���x�N�g���ƃt���O��Ԃ�����
					collision_True.collision_Flag = true;
					collision_True.normal = tri.normal;
					//�����o�^����Ă�����̂Ȃ̂��m�F����
					if (result->size() == 0 || collision_True != result->at(result->size() - 1))
					{
						result->push_back(collision_True);
					}
					//�|���S��1������1�̓_�̏Փ˂��N�����炻��Ŏ��̃|���S���̔��������
					break;
				}
			}
		}
	}	
	
	//��������͌��݈ʒu�ōs���Đ�������t���O�͈ړ������Ɨ\�z�����Ƃ���Ŕ��������
	ML::Vec3 next_Pos = ball_Pos + speed;
	//���̈ʒu���{�b�N�X�̑Ίp���𔼒��a�Ƃ��鋅�Ƃ������Ă���Ɛ�������t���O�𗧂Ă�
	this->pricision_Flag = ML::Vec3(next_Pos - box_Center).Length() <= r + abs( ROOT3 / 2.0f*(box_Length));
	
	
	if (collision_True.collision_Flag)
	{
		return true;
	}
	//������Ȃ�������false��Ԃ�	
	return false;
}

