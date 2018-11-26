#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�����^���}�l�[�W��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include  "Task_CameraMan.h"
#include  "Task_MapCore.h"
#include  "Task_MapSide.h"
#include  "Task_MapFence.h"
#include  "Task_Ball.h"


namespace Physics_Manager
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�����^��");	//�O���[�v��
	const  string  defName("Manager");	//�^�X�N��
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
		ML::QT frame_QTxp;
		ML::QT frame_QTxm;
		ML::QT frame_QTyp;
		ML::QT frame_QTym;
		ML::QT frame_QTzp;
		ML::QT frame_QTzm;
		//��]��
		ML::Vec3 anckerX;
		ML::Vec3 anckerY;
		ML::Vec3 anckerZ;
			
		//���\�b�h
		//�}�b�v�̏����������؂�ɊǗ����� ���� : (�����x)
		void Managing(const unsigned int delicate);
		
	public:
		//��]���Đݒ�
		void Ancker_Calculating();
	};
}