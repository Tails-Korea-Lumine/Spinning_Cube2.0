#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�J�[�\��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

enum nowMenu
{
	clear = -10,
	Start_Tutorial = 0,
	difficult = -1,
	TutorialCalum = 1,
};

namespace Cursor
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�J�[�\��");	//�O���[�v��
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
		string imageName;
		string seMoveName;
		string seSelectName;
		string seCancelName;
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
		ML::Vec2 pos;//�`�悷����W
		ML::Vec2 destination;//�ړI�n
		POINT select_pos;
		nowMenu now;
		//�^�X�N���ł܂ł̃J�E���^�ƃt���O
		int countdown;
		bool countdownFlag;
		//�e���j���[�͈̔�
		//0�Ԃ�min�A1�Ԃ�max
		int first_Menu_Range[2];
		int last_Menu_Range[2];

	public:
		//���\�b�h
		int Move_Cursor(POINT& select);
		//���݃��j���[��Ԃ��֐�
		nowMenu Get_Now_Menu();
		//�J�E���g�_�E���t���O��Ԃ��֐�
		bool Is_Count_Down() const;
		//��ʐ؂�ւ����I������̂����m�F
		bool Count_Down_Over() const;
		//�I������
		void Click();
	};
}