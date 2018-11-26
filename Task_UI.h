#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//UserInterface
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

enum WipeInOut
{
	Wipe_In = 0,
	Wipe_Out = 1,
};

namespace  UI
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("UI");	//�O���[�v��
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
		//���L����ϐ��͂����ɒǉ�����\
		//�i�X����������ʂ��B��
		string wipeIn_Img;
		//�i�X����������ʂ𗣂��
		string wipeOut_Img;
		//���ԕ\��
		string uI_Time_Img;
		//�����\
		string number_Img;

		
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
	//�ύX������������������������������������������������������	
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����	
		//�ϐ�

		//�G�ŕ\�����镶���̈ʒu
		//����
		ML::Vec2 ui_Time;

		//�����\�̏��
		//���ԕ\���p�F�܂Ŋ܂߂�11��
		ML::Box2D src_Number[11];

		//���ԗp�J�E���^
		int timeCnt;
		//���Ɏg���t���O
		WipeInOut wipe;
		//�����x
		float wipe_Alpha;

		
	public:
		//���\�b�h
		//���̕`��
		void Draw_Wipe_InOut();
		//���Ԃ̕`��
		void Draw_Time() const;
		//��ʖ��������n�߂�
		void Start_WipeIn();		
	};
}