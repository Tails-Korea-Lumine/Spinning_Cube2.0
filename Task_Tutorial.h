#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�e���[�g���A��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Task_title.h"

namespace Tutorial
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�e���[�g���A��");	//�O���[�v��
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
		//�I���\�Ȗڎ���3��
		string imageName[3];
		string Bg_Img;
		string bgmName;
		string guide_Img[2];
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
		static  Object::SP  Create(bool flagGameEnginePushBack_, Tutorial_Column tc);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize(Tutorial_Column tc);
		bool  B_Finalize();
		bool  Initialize(Tutorial_Column tc);	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		void  Render3D_L0();
		//�ύX������������������������������������������������������
	
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����

		int countDown;
		bool countDown_Flag;

		int timeCnt;
		bool page_Change_Flag;

		int posx[3];//�`���[�g���A���y�[�W�؂�ւ��p
		int posy;//�e���[�g���A���y�[�W�̕\���ʒu-528=<posy=<720 �ύX�\������

		//bgm volume
		long vol;
		
		Tutorial_Column column;

		//BGM���H�����[�����t�F�C�h�A�E�g������
		void BGM_Fade_Out();

	public:
		void Page_Chage(bool page_Move_Right);
		bool Can_I_Change_the_Page() const;
		//�������ł܂ł̃J�E���g�_�E���J�n���m�F
		bool Is_Count_Down() const;
		//�J�E���g�_�E�����I������̂����m�F
		bool Count_Down_Over() const;
	};
}