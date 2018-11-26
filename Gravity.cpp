#include "Gravity.h"
#include "MyPG.h"

void Gravity::Accelerate(ML::Vec3* speed, const float& Weight) const
{
	*speed += (Weight * this->G_acceleration );
}

void Gravity::Diagonal_Accelerate(ML::Vec3* speed, const ML::Vec3& normal) const
{
	float fn;
	MyMath::Get_Vector_Dot(&fn, *speed, normal);
	//fn *= speed.Length();
	//��Βl�ɕς���
	fn = abs(fn);
	
	//������͂ɍ��킹����̃x�N�g���̑傫��
	ML::Vec3 after_Normal;

	after_Normal = normal * fn;

	//���ˊp�ɕϊ������x�N�g��
	//ML::Vec3 after_Collision;

	*speed += after_Normal;
	//�d���������ɉe����^����
	//return after_Collision;
}

void Gravity::Reflaction_Vector(ML::Vec3* force, const ML::Vec3& normal) const
{
	//�P�ʃx�N�g����菬�����Ȃ����ꍇ�͎΂߉�����������
	if (force->Length() <= 1.0f)
	{
		this->Diagonal_Accelerate(force, normal);
		return;
	}

	//�ǖʂ̖@���x�N�g���̑傫����ύX
	float fn;
	MyMath::Get_Vector_Dot(&fn, *force, normal);
	//fn *= force.Length();
	//��Βl�ɕς���
	fn = abs(fn);

	//������͂ɍ��킹����̃x�N�g���̑傫��
	ML::Vec3 after_Normal;

	after_Normal = normal * fn;

	//���ˊp�ɕϊ������x�N�g��
	*force += (2 * after_Normal);
	//�������l����60���ɂ���
	*force *= 0.6f;	
}
