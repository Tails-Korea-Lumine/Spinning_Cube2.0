#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�Q�[���{��
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Task_title.h"

namespace Game
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�Q�[��");	//�O���[�v��
	const  string  defName("�{��");	//�^�X�N��
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
		string BG_mesh;
		string bgmName;
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
		static  Object::SP  Create(bool flagGameEnginePushBack_, Difficult_Range di);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize(Difficult_Range di);
		bool  B_Finalize();
		bool  Initialize(Difficult_Range di);	//�u�������v�^�X�N�������ɂP�񂾂��s������
		void  UpDate();		//�u���s�v�P�t���[�����ɍs������
		void  Render2D_AF();	//�u2D�`��v�P�t���[�����ɍs������
		bool  Finalize();		//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
		void  Render3D_L0();
		//�ύX������������������������������������������������������
	
		//�ǉ��������ϐ��E���\�b�h�͂����ɒǉ�����
		//�ϐ�
		//�^�X�N���ł܂ł̃J�E���^�ƃt���O	
		int countdown;
		bool countdownFlag;

		//�w�i����]������p�x
		float angleY;

		Difficult_Range nowdi;

		//�V�񂾃v���[����Result�^�X�N�ɓn������
		int timeCnt;
		//bgm volume
		long vol;

	public:
		//�J�E���g�_�E���t���O��Ԃ��֐�
		bool Is_Count_Down() const;
		//�N���A�t���O�𗧂Ă�֐�
		void Game_Clear();
		//�N���A�����̂���Ԃ��֐�
		bool IS_Cleared() const;
		//����\����Ԃ��֐�
		bool GET_READY() const;
		//��ʐ؂�ւ����I������̂����m�F
		bool Count_Down_Over() const;
		//�Q�[���N���A���o���n�߂�
		bool Start_Clear_Promotion();
		//�J�������o���I������̂����m�F
		bool Is_Camera_Promotion_Over();
		//BGM���H�����[�����t�F�C�h�A�E�g������
		void BGM_Fade_Out();

	};
}