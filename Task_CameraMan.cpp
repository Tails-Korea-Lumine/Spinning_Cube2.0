//-------------------------------------------------------------------
//�J�����}��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_CameraMan.h"
#include  "Task_Physics_Manager.h"
#include  "Task_Game.h"
#include  "easing.h"

namespace  CameraMan
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{		
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{		
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
		//�J�����̐ݒ�
		ge->camera[0] = MyPG::Camera::Create(
			ML::Vec3(1050.0f, 50.0f, 1050.0f),				//	�^�[�Q�b�g�ʒu
			ML::Vec3(2050.0f, 1050.0f, -1500.0f),//	�J�����ʒu
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	�J�����̏�����x�N�g��
			ML::ToRadian(35), 10.0f, 4000.0f,	//	����p�E���싗��
			(float)ge->screenWidth / (float)ge->screenHeight);		//	��ʔ䗦		
		DG::EffectState().param.bgColor = ML::Color(1, 0.0f, 0.0f, 0.0f);
		//���C�e�B���O�L����
		DG::EffectState().param.lightsEnable = true;
		//�����̋�����ݒ肷��
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.3f, 0.3f, 0.3f);
		//���s�����̐ݒ�
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.light[0].kind = DG_::Light::Directional;//�����̎��
		DG::EffectState().param.light[0].direction = ML::Vec3(1, -1, 1).Normalize();//�Ǝ˕���
		DG::EffectState().param.light[0].color = ML::Color(1, 1, 1, 1);//�F�Ƌ���		

		easing::Set("Moving_Camera", easing::EXPOIN, 0, 0.1f, 20);

		this->nowPos = ge->camera[0]->pos;
		this->initPos = ge->camera[0]->pos;
		this->distance = 2750.0f;
		this->angle = ML::Vec3(0, 0, 0);
		this->maxAngle = 30;
		this->timeCnt = 0;
		this->destination = this->nowPos;
		this->moving_Flag = false;


		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���


		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
			//auto nextTask = Game::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//auto game = ge->GetTask_One_G<Game::Object>("�Q�[��");

		if (!ge->game.lock()->GET_READY())
		{
			this->ProMotion();
		}
		else
		{
			this->Camera_Move();
		}

		//���C�g���I�t����Ă���3�b��ɁA���C�g���I������
		if (this->It_Passed_3sec())
		{
			this->Turnon_the_Light();
		}
		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{		
	}

	void  Object::Render3D_L0()
	{
		
	}

	//---------------------------------------------------------------------
	//�ǉ����\�b�h
	//���C�g�I�t
	void Object::Turnoff_the_Light()
	{
		DG::EffectState().param.light[0].enable = false;
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.005f, 0.005f, 0.005f);
		this->timeCnt = 0;
	}
	//----------------------------------------------------------------
	//���C�g�I��
	void Object::Turnon_the_Light()
	{
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.3f, 0.3f, 0.3f);
	}
	//-----------------------------------------------------------------
	//3�b���������̂����m�F
	bool Object::It_Passed_3sec() const
	{
		if (this->timeCnt > 180)
		{
			return true;
		}
		return false;
	}

	//------------------------------------------------------------------------
	//�Q�[���X�^�[�g���o
	void Object::ProMotion()
	{
		if (this->timeCnt < 180)
		{
			ge->camera[0]->pos.z += 22;
		}
		else if (this->timeCnt == 180)
		{
			ge->camera[0]->pos = ML::Vec3(50, -950, 1940);
		}
		else if (this->timeCnt < 360)
		{
			ge->camera[0]->pos.z -= 22;
		}
		else if (this->timeCnt == 360)
		{
			ge->camera[0]->pos = ML::Vec3(1050, 50, -1100);
		}
		else
		{
			ge->camera[0]->pos.z -= 13;
		}

		if (ge->camera[0]->pos.z < ge->Map_center.z - this->distance)
		{
			ge->camera[0]->pos.z = ge->Map_center.z - this->distance;
		}
	}

	//--------------------------------------------------------------------------
	//�J�����̈ړ�����
	void Object::Camera_Move()
	{
		//�ړI�n�ƌ��݈ʒu���قȂ�Ƃ��Ɉړ��J�n
		if (this->nowPos != this->destination)
		{	
			easing::Start("Moving_Camera");
			//�ړ����t���O�𗧂Ă�
			this->moving_Flag = true;
			//�ړI�n�ƌ��݈ʒu���قȂ�ꍇ
			//���΋�����25%���ړ�����
			ML::Vec3 d = this->destination - this->nowPos;
			this->nowPos += d * easing::GetPos("Moving_Camera");

			//��苗���ȓ��Ȃ���������
			if (d.Length() <= 20)
			{				
				this->moving_Flag = false;
				this->nowPos = this->destination;
				//���Čv�Z
				ge->GetTask_One_G<Physics_Manager::Object>("�����^��")->Ancker_Calculating();
				//easing reset
				easing::Reset("Moving_Camera");
			}
			//�J�����ʒu�̍X�V
			ge->camera[0]->pos = this->nowPos;
			ge->camera[0]->UpDate();
		}
	}
	//---------------------------------------------------------------------------
	//�ړI�n�ݒ�
	void Object::Set_Destination(const ML::Vec3& side_Normal_On_Ball)
	{
		this->destination = ge->Map_center + (side_Normal_On_Ball.Normalize() * this->distance);
	}
	//----------------------------------------------------------------------------
	//�ړ����Ȃ̂����m�F����
	bool Object::Is_Moving_Now()
	{
		return this->moving_Flag;
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