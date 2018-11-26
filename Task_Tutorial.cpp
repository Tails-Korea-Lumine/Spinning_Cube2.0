//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Tutorial.h"
#include  "Task_Title.h"
#include "Task_UI.h"
#include "easing.h"

#define SCROLL_MAX_RANGE -200
namespace  Tutorial
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->imageName[0] = "Tutorial_Outline_Img";
		this->imageName[1] = "Tutorial_Control_Img";
		this->imageName[2] = "Tutorial_Obstacle_Img";
		this->Bg_Img = "Tutorial_BG";
		DG::Image_Create(this->imageName[0], "./data/image/Tutorial_Outline.png");//�T�v�̃e���[�g���A��
		DG::Image_Create(this->imageName[1], "./data/image/Tutorial_Control.png");//����̃e���[�g���A��
		DG::Image_Create(this->imageName[2], "./data/image/Tutorial_Obstacle.png");//��Q���̃e���[�g���A��
		DG::Image_Create(this->Bg_Img, "./data/image/background.jpg");

		this->guide_Img[0] = "page_Guide";
		this->guide_Img[1] = "press_S";

		DG::Image_Create(this->guide_Img[0], "./data/image/pageGuide.png");
		DG::Image_Create(this->guide_Img[1], "./data/image/pressS.png");

		this->bgmName = "tutorialBGM";
		DM::Sound_CreateStream(this->bgmName, "./data/sound/tutorial.wav");
	
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		for (int i = 0; i < 3; i++)
		{
			DG::Image_Erase(this->imageName[i]);
		}
		for (int i = 0; i < 2; i++)
		{
			DG::Image_Erase(this->guide_Img[i]);
		}
		DG::Image_Erase(this->Bg_Img);
		DM::Sound_Erase(this->bgmName);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize(Tutorial_Column tc)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->posy = 0;
		this->column = tc;

		this->countDown = 0;
		this->countDown_Flag = false;

		this->timeCnt = 990;//0~80�ł͂Ȃ����l�ŏ�����
		this->page_Change_Flag = false;

		this->vol = 1000;

		switch(this->column)
		{
		case Tutorial_Column::OutLine:
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] = ge->screenWidth * i;
				
			}
			break;
		case Tutorial_Column::Control:
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] = ge->screenWidth * (i-1);
				
			}
			break;
		case Tutorial_Column::Obstacle:
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] = ge->screenWidth * (i-2);
				
			}
			break;
		}

		DM::Sound_Play(this->res->bgmName, true);
			
		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);
		//���^�X�N�̐���

		auto ui = UI::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���

		ge->KillAll_G("UI");

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
			auto nextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		int scroll_Speed = 8;

		easing::UpDate();

		auto in = DI::GPad_GetState("P1");
		//�X�N���[������
		if (in.LStick.D.on || in.HD.on)
		{
			this->posy -= scroll_Speed;
		}
		if (in.LStick.U.on || in.HU.on)
		{
			this->posy += scroll_Speed;
		}		
		//�y�[�W�؂�ւ�����
		if (in.L1.down || in.L2.down)
		{
			if (this->Can_I_Change_the_Page())
			{
				this->page_Change_Flag = false;
				this->timeCnt = 0;
				this->posy = 0;
			}
		}
		else if (in.R1.down || in.R2.down)
		{
			if (this->Can_I_Change_the_Page())
			{
				this->page_Change_Flag = true;
				this->timeCnt = 0;
				this->posy = 0;
			}
		}
		//�y�[�W�؂�ւ��̓���
		this->Page_Chage(this->page_Change_Flag);

		//posy�͈̔͐ݒ�
		if (this->posy < SCROLL_MAX_RANGE)
		{
			this->posy = SCROLL_MAX_RANGE;
		}
		if (this->posy > 0)
		{
			this->posy = 0;
		}		

		//��ʐ؂�ւ��J�n
		if (in.ST.down)
		{
			ge->GetTask_One_G<UI::Object>("UI")->Start_WipeIn();
			this->countDown_Flag = true;
		}

		//�J�E���g�_�E��
		if (this->Is_Count_Down())
		{
			this->countDown++;
		}
		//�J�E���g�_�E�����I������玩��������
		if (this->Count_Down_Over())
		{
			this->Kill();
		}
		//���ԏ㏸
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		ML::Box2D drawBG(0, 0, ge->screenWidth, ge->screenHeight);
		//�摜�S�̃T�C�Y
		POINT size_BG = DG::Image_Size(this->res->Bg_Img);
		ML::Box2D srcBG(0, 0, size_BG.x, size_BG.y);

		DG::Image_Draw(this->res->Bg_Img, drawBG, srcBG);

		ML::Box2D draw[3] = 
		{
			{0, 0, (int)ge->screenWidth, 1248},
			{ 0, 0, (int)ge->screenWidth, 1248 },
			{ 0, 0, (int)ge->screenWidth, 1248 },
		};
		ML::Box2D src(0, 0,1280, 1248);
		for (int i = 0; i < 3; i++)
		{
			draw[i].Offset(this->posx[i], this->posy);
			DG::Image_Draw(this->res->imageName[i], draw[i], src);
		}
		
		//�K�C�h�C���[�W
		ML::Box2D drawguide[2] =
		{
			{0,(int)ge->screenHeight-60,(int)ge->screenWidth/2,60},
			{(int)ge->screenWidth/2,(int)ge->screenHeight-60,(int)ge->screenWidth / 2,60 }
		};
		ML::Box2D srcguide[2] =
		{
			{ 0,0,2200,140},
			{0,0,1400,140}
		};
		for (int i = 0; i < 2; i++)
		{
			DG::Image_Draw(this->res->guide_Img[i], drawguide[i], srcguide[i]);
		}

	}

	void  Object::Render3D_L0()
	{
		
	}

	//-----------------------------------------------------------------------------------
	//�y�[�W�؂�ւ�
	void Object::Page_Chage(bool page_Move_Right)
	{
		int page_Changing_Speed = 24;
		if (this->Can_I_Change_the_Page())
		{
			return;
		}

		//�E�̃y�[�W������
		if (page_Move_Right)
		{
			if (this->posx[2] <= 0)
			{
				return;
			}
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] -= page_Changing_Speed;
			}
		}
		//���̃y�[�W������
		else
		{
			if (this->posx[0] >= 0)
			{
				return;
			}
			for (int i = 0; i < 3; i++)
			{
				this->posx[i] += page_Changing_Speed;
			}
		}
		
	}

	//-----------------------------------------------------------------------------
	//�������ł܂ł̃J�E���g�_�E���J�n���m�F
	bool Object::Is_Count_Down() const
	{
		return this->countDown_Flag;
	}

	//-------------------------------------------------------------------------------
	//�J�E���g�_�E�����I������̂����m�F
	bool Object::Count_Down_Over() const
	{
		return (this->countDown > 60);
	}
	//----------------------------------------------------------------------------------
	//�y�[�W�؂�ւ����\���𔻒f
	bool Object::Can_I_Change_the_Page() const
	{
		return (this->timeCnt > 80);
	}
	//--------------------------------------------------------------------------------
	//BGM fade out
	void Object::BGM_Fade_Out()
	{
		this->vol -= 5;
		DM::Sound_Volume(this->res->bgmName, this->vol);
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, Tutorial_Column tc)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize(tc)) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(Tutorial_Column tc)
	{
		return  this->Initialize(tc);
	}
	//-------------------------------------------------------------------
	Object::~Object() { this->B_Finalize(); }
	bool  Object::B_Finalize()
	{
		auto  rtv = this->Finalize();
		return  rtv;
	}
	//-------------------------------------------------------------------
	Object::Object() {	}
	//-------------------------------------------------------------------
	//���\�[�X�N���X�̐���
	Resource::SP  Resource::Create()
	{
		if (auto sp = instance.lock()) {
			return sp;
		}
		else {
			sp = Resource::SP(new  Resource());
			if (sp) {
				sp->Initialize();
				instance = sp;
			}
			return sp;
		}
	}
	//-------------------------------------------------------------------
	Resource::Resource() {}
	//-------------------------------------------------------------------
	Resource::~Resource() { this->Finalize(); }
}