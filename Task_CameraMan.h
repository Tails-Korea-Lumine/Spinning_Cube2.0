#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�J�����}��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace CameraMan
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�J�����}��");	//�O���[�v��
	const  string  defName("NoName");	//�^�X�N��
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//���L����ϐ��͂����ɒǉ�����		
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//�ύX�s����������������������������������������������������
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//�������� ������true�Ń^�X�N�V�X�e���֎����o�^
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		void  Render3D_L0();
		//�ύX������������������������������������������������������
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//�ϐ�

		//�J�����A���O��
		ML::Vec3 angle;
		//�A���O���̍ő�l
		float maxAngle;
		//���݃J�����̈ʒu
		ML::Vec3 nowPos;
		//�J�����̏����ʒu
		ML::Vec3 initPos;
		//�J�����ƃ}�b�v���S�̋���
		float distance;
		//���C�g�I���܂ł̎��Ԃ��v��J�E���^
		int timeCnt;

		//�J�������{�[�������݂���ʂɈړ������邽�߂̖ړI�n
		ML::Vec3 destination;
		//�J�����ړ�����\���t���O
		bool moving_Flag;
		//�Q�[���X�^�[�g�O�̉��o
		void ProMotion();
		//�J�����̈ړ�����
		void Camera_Move();

	public:
		//���\�b�h
		//���C�g���I�t���鏈��
		void Turnoff_the_Light();
		//���C�g���I�����鏈��
		void Turnon_the_Light();
		//3�b���������̂��𔻒�
		bool It_Passed_3sec() const;	
		//�ړI�n�ݒ�
		void Set_Destination(const ML::Vec3&);
		//�J�����������Ă��邩���m�F����
		bool Is_Moving_Now();
	};
}