#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "Task_cursor.h"

enum Difficult_Range
{
	Easy = -1,
	Normal = 1,
	Hard = 3,
};

enum Tutorial_Column
{
	OutLine = -1,
	Control = 1,
	Obstacle = 3,
};

namespace Title
{
	//�^�X�N�Ɋ��蓖�Ă�O���[�v���ƌŗL��
	const  string  defGroupName("�^�C�g��");	//�O���[�v��
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
		string Title_Name_Image[12];
		string press_Any_Key_Image;
		string Difficalt_Image[3];
		string Column_Image[3];
		string start_Image;
		string tutorial_Image;
		string select_Guide_Image;
		string BG_ImageName;
		string bgm_Title;
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
		//�^�C�g�������̈ړ�����ѕ`����W
		ML::Vec2 Title_Name[12];
		//���j���[�I�𒆂Ȃ̂����m�F����t���O
		bool select_now;
		//���Ԋm�F�p�̃J�E���^
		int timeCnt;
		//���j���[�����E�ɓ�������ړ���
		int moving_Menu;
		//�^�C�g��������ɓ�������ړ���
		int moving_Title_Name;
		//���Ɉړ�����^�X�N�����߂�ϐ�
		POINT next_Task_Index;
		//���݃��j���[
		nowMenu n;
		//�O���f�[�V�����E�F�[�u�p
		float waveR, waveG, waveB;
		float dgreeR, dgreeG, dgreeB;
		//bgm volume
		long vol;
	
		//���\�b�h
		//�Q�[���p�b�h������͂����炦�邷�ׂĂ̂��̂��m�F
		bool Press_Any_Key() const;
		//PAK��`�悵�Ȃ��Ă������̂��𔻕ʂ���
		bool Is_Need_to_Draw_PAK() const;
		//�^�C�g�����ʒu�X�V
		void UpDate_Title_Name();
		//�^�C�g�����`��֐�
		void Draw_Title_Name() const;
		//press any key��`��
		void Draw_PAK() const;
		//���j���[��`��
		void Draw_Menu() const;
		//2�Ԗڂ̃��j���[(��Փx�܂��̓e���[�g���A���ڎ�)��`��
		void Draw_Dif_Col(nowMenu now) const;
		//�F�����񂾂�ύX����
		void Color_Wave();

	public:
		//���̃^�X�N�����߂�֐�
		void I_Select(POINT select);
		//BGM���H�����[�����t�F�C�h�A�E�g������
		void BGM_Fade_Out();
	};
}