//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Game.h"
#include  "Task_title.h"
#include  "Task_MapSide.h"
#include  "Task_MapCore.h"
#include  "Task_Ball.h"
#include  "Task_MapFence.h"
#include  "Task_Physics_Manager.h"
#include  "Task_Result.h"
#include  "Task_UI.h"
#include  "Task_CameraMan.h"
#include "easing.h"

namespace  Game
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->bgmName = "stageBgm";
		this->BG_mesh = "under the sea";
		DM::Sound_CreateStream(this->bgmName, "./data/sound/stage.wav");
		DG::Mesh_CreateFromSOBFile(this->BG_mesh, "./data/mesh/under_the_sea.SOB");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DM::Sound_Erase(this->bgmName);
		DG::Mesh_Erase(this->BG_mesh);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize(Difficult_Range di)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->countdown = 0;
		this->countdownFlag = false;
		this->timeCnt = 0;
		this->nowdi = di;
		this->render2D_Priority[1] = 1.0f;
		ge->gameClearFlag = false;
		ge->getReadyFlag = true;
		this->vol = 1000;

		//���^�X�N�̐���
		//UI�̐���
		auto ui = UI::Object::Create(true);

		//�J�����}���𐶐�
		auto cameraman = CameraMan::Object::Create(true);
		
		//�f�o�b�O�p�̕�������
		//DG::Font_Create("FontA", "HGS�޼��M", 12, 16);

		//bgm�Đ�
		DM::Sound_Play(this->res->bgmName, true);

		
		//�w�i�̊p�x��������
		this->angleY = 0.0f;

		//�}�b�v�̒��S�n
		ge->Map_center = ML::Vec3(1050, 50, 1050);
		//����̌��ʂ��[���N���A
		ge->collision_Result.clear();

		//�{�[���̐���
		auto ball = Ball::Object::Create(true);
		//���S�̌��Ȃ�L���[�u
		auto core = Map_Core::Object::Create(true , di);
		//�}�b�v����
		for (int i = 0; i < 6; i++)
		{
			auto map = Map3d::Object::Create(true, i,di);
		}
		//�t�F���X����
		for (int f = 0; f < 12; f++)
		{
			auto fence = MapFence::Object::Create(true, f,di);
		}		

		//�}�b�v�}�l�[�W������
		auto manager = Physics_Manager::Object::Create(true);

		//�G�t�F�N�g�}�l�[�W���[����
		ge->eff_Manager = EffectManager::Object::Create(true);
		

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("�}�b�v");
		ge->KillAll_G("�{�[��");
		ge->KillAll_G("UI");
		ge->KillAll_G("�J�����}��");
		ge->KillAll_G("�G�t�F�N�g");
		ge->KillAll_G("�����^��");
	//	DG::Font_Erase("FontA");

		DM::Sound_Stop(this->res->bgmName);

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
			
			auto nextTask = Result::Object::Create(true,this->timeCnt,this->nowdi);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");

		if (this->Start_Clear_Promotion())
		{
			ge->KillAll_G("�{�[��");
			ge->GetTask_One_G<UI::Object>("UI")->Start_WipeIn();
			this->countdownFlag = true;
		}

		//�����I��
		if ((in.ST.on && in.SE.on) && ge->getReadyFlag == false)
		{
			this->Game_Clear();

			this->timeCnt = 0xffff;
		}

		//�J�E���g�_�E��
		if (this->Is_Count_Down())
		{
			this->countdown++;
			this->BGM_Fade_Out();
		}
		//1�b��Ƀ^�X�N����
		if (this->Count_Down_Over())
		{
			this->Kill();
		}		
		ge->TM.Increse_Counter();

		//�n�܂�O�̉��o���I������珀�����t���O�𖳌��ɂ���
		if (!this->GET_READY())
		{
			if (this->Is_Camera_Promotion_Over())
			{
				ge->getReadyFlag = false;
				this->timeCnt = 0;
			}
		}
		//���Ԃ��X�V
		this->timeCnt++;
		//�w�i�p�x�X�V
		this->angleY += 0.04f;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		/*auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");
		if (ball == nullptr)
		{
			return;
		}
		char buf[1024];
		sprintf(buf, "pos : %4.3f , %4.3f , %4.3f \n"
			"speed : %4.3f , %4.3f , %4.3f \n"
			"WorldR : %4.3f , %4.3f , %4.3f",
			 ball->Get_Pos().x, ball->Get_Pos().y, ball->Get_Pos().z, ball->Get_Speed().x, ball->Get_Speed().y, ball->Get_Speed().z,
			ge->World_Rotation.x, ge->World_Rotation.y, ge->World_Rotation.z);

		ML::Box2D moji(100, 0, 600, 600);
		DG::Font_Draw("FontA", moji, buf, ML::Color(1, 1, 0, 1));

		ML::Box2D draw(0, 0, ge->screenWidth, ge->screenHeight);
		ML::Box2D src(0, 0, 960, 539);*/

		//DG::Image_Draw(this->res->BG_Image, draw, src);
	}

	void  Object::Render3D_L0()
	{
		//�C�̒��������_�����O
		ML::Mat4x4 matS, matR, matT;
		
		matS.Scaling(200.0f);
		matR.RotationY(ML::ToRadian(this->angleY));
		matT.Translation(ge->camera[0]->pos);		

		DG::EffectState().param.matWorld = matS * matR * matT;
		DG::EffectState().param.lightsEnable = false;
		if (DG::EffectState().param.light[0].enable)
		{
			DG::Mesh_Draw(this->res->BG_mesh);
		}

		DG::EffectState().param.lightsEnable = true;
	}

	//--------------------------------------------------------------------------------------
	//�ǉ����\�b�h
	//�J�E���g�_�E���t���O��Ԃ��֐�
	bool Object::Is_Count_Down() const
	{
		return this->countdownFlag;
	}

	//------------------------------------------------------------------------------------
	//�N���A�t���O�𗧂Ă�
	void Object::Game_Clear()
	{
		ge->gameClearFlag = true;
	}

	//---------------------------------------------------------------------------------------
	//�N���A�����̂����m�F
	bool Object::IS_Cleared() const
	{
		return ge->gameClearFlag;
	}

	//--------------------------------------------------------------------------------------
	//����\�����m�F
	bool Object::GET_READY() const
	{
		return !ge->getReadyFlag;
	}
	//--------------------------------------------------------------------------
	//��ʐ؂�ւ����I������̂����m�F
	bool Object::Count_Down_Over() const
	{
		return (this->countdown > 130);
	}
	//---------------------------------------------------------------------------
	//�Q�[���N���A���o���n�߂�
	bool Object::Start_Clear_Promotion()
	{
		return (this->IS_Cleared() && this->countdown == 0);
	}

	//------------------------------------------------------------------------------
	//�J�������o���I������̂����m�F
	bool Object::Is_Camera_Promotion_Over()
	{
		return this->timeCnt > 420;
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
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, Difficult_Range di)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize(di)) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(Difficult_Range di)
	{
		return  this->Initialize(di);
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