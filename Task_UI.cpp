//-------------------------------------------------------------------
//
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_UI.h"
#include  "easing.h"
#include  "Task_Game.h"
#include "Task_Result.h"

namespace  UI
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		//���O������
		this->uI_Time_Img = "UI_Time";
		this->wipeIn_Img = "WipeIn";
		this->wipeOut_Img = "WipeOut";
		this->number_Img = "Number";
		
		//�I�t�X�N���[������
		DG::Image_Create(this->uI_Time_Img, "./data/image/UI_Time.png");
		DG::Image_Create(this->wipeIn_Img, "./data/image/wipeIn.png");
		DG::Image_Create(this->wipeOut_Img, "./data/image/wipeOut.png");
		DG::Image_Create(this->number_Img, "./data/image/Number.png");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		//�摜�̊J��
		DG::Image_Erase(this->uI_Time_Img);
		DG::Image_Erase(this->wipeIn_Img);
		DG::Image_Erase(this->wipeOut_Img);
		DG::Image_Erase(this->number_Img);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize()
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->render2D_Priority[0] = 1.0f;
		this->timeCnt = 0;
		this->wipe = WipeInOut::Wipe_Out;
		this->wipe_Alpha = 1.0f;

		//ui��y���W��easing�œ������̂ŉ�ʊO�K���ȂƂ���ɏ�����
		this->ui_Time = ML::Vec2(0, -1000.0f);
		//�����\�̓ǂݍ��݃`�b�v��������
		for (int i = 0; i < 11; i++)
		{
			this->src_Number[i].x = (140 * i);
			this->src_Number[i].y = 0;
			this->src_Number[i].w = 140;
			this->src_Number[i].h = 140;
		}
		//���v�̕\����
		easing::Set("Up_to_Down", easing::SINEOUT, -50, 0, 30);		
		easing::Start("Up_to_Down");

		//����easingSet
		easing::Set("wipeOut", easing::SINEOUT, -200, (float)ge->screenWidth, 200);
		easing::Set("wipeIn", easing::SINEOUT, -2200, 0, 130);
		
		//���̓^�X�N�����̎���ʂ�������čs����
		//�^�X�N���őO�ɉ�ʂ̏�܂ňړ�����
		easing::Start("wipeOut");

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//�������p���^�X�N�̐���
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		this->ui_Time.y = easing::GetPos("Up_to_Down");
		//UI�Ō����鎞�v�͏������I�������ɗ����
		if (ge->game.lock()->GET_READY())
		{
			this->timeCnt++;
		}

		//�����x�͈͎̔w��
		if (this->wipe_Alpha < 0.0f)
		{
			this->wipe_Alpha = 0.0f;
		}
		if (this->wipe_Alpha > 1.0f)
		{
			this->wipe_Alpha = 1.0f;
		}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		if (ge->GetTask_One_G<Game::Object>("�Q�[��") != nullptr)	
		{
			this->Draw_Time();
		}		
		this->Draw_Wipe_InOut();
	}
	//----------------------------------------------------------------------------------
	//���Ԃ̕`��
	void Object::Draw_Time() const
	{
		int min10, min1, sec10, sec1;
		//�e�ʂ̐��l��ʁX�ɂ��炤
		min10 = this->timeCnt / 60 / 60 / 10;
		min1 = this->timeCnt / 60 / 60 % 10;
		sec10 = this->timeCnt / 60 % 60 / 10;
		sec1 = this->timeCnt / 60 % 60 % 10;

		ML::Box2D draw_UI_Time(0, 0, 200, 64);
		ML::Box2D src_UI_Time(0, 0, 430, 140);
		ML::Box2D draw_Min10(200, 0, 64, 64);
		ML::Box2D draw_Min1(250, 0, 64, 64);
		ML::Box2D draw_Sec10(310, 0, 64, 64);
		ML::Box2D draw_Sec1(360, 0, 64, 64);
		ML::Box2D draw_colon(280, 0, 50, 64);

		//easing�ł�Y���W�X�V
		draw_UI_Time.Offset(this->ui_Time);
		draw_Min10.Offset(this->ui_Time);
		draw_Min1.Offset(this->ui_Time);
		draw_colon.Offset(this->ui_Time);
		draw_Sec10.Offset(this->ui_Time);
		draw_Sec1.Offset(this->ui_Time);

		//�`��
		DG::Image_Draw(this->res->uI_Time_Img, draw_UI_Time, src_UI_Time);
		DG::Image_Draw(this->res->number_Img, draw_Min10, this->src_Number[min10]);
		DG::Image_Draw(this->res->number_Img, draw_Min1, this->src_Number[min1]);
		DG::Image_Draw(this->res->number_Img, draw_Sec10, this->src_Number[sec10]);
		DG::Image_Draw(this->res->number_Img, draw_Sec1, this->src_Number[sec1]);
		DG::Image_Draw(this->res->number_Img, draw_colon, this->src_Number[10]);
	}

	//--------------------------------------------------------------------------------------
	//���̕`��
	void Object::Draw_Wipe_InOut()
	{
		ML::Box2D drawOut(-200, -160, 3000, 1500);
		ML::Box2D drawIN(-2200, 0, 3000, 1500);		
		ML::Box2D src(0, 0, 1920, 1080);
		
		switch (this->wipe)
		{			
			//��ʖ�����
		case WipeInOut::Wipe_Out:			

			this->wipe_Alpha -= 0.01f;
			
			drawOut.x = int(easing::GetPos("wipeOut"));

			DG::Image_Draw(this->res->wipeOut_Img, drawOut, src, ML::Color(this->wipe_Alpha,1,1,1));
			break;
						
			//��ʉB��
		case WipeInOut::Wipe_In:
			

			drawIN.x = int(easing::GetPos("wipeIn"));
			this->wipe_Alpha += 0.02f;			

			DG::Image_Draw(this->res->wipeIn_Img, drawIN, src, ML::Color(this->wipe_Alpha, 1, 1, 1));
			break;
		}
	}

	//-----------------------------------------------------------------------
	//��ʉB�����n�߂�
	void Object::Start_WipeIn()
	{
		easing::Start("wipeIn");
		this->wipe = WipeInOut::Wipe_In;
		this->wipe_Alpha = 0.1f;
	}
	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
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