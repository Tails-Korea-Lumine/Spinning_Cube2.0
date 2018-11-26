//-------------------------------------------------------------------
//�}�b�v�}�l�[�W��
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Physics_Manager.h"
#include "easing.h"
#include "MyMath.h"

#define PRECISION 3

namespace  Physics_Manager
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
		

		this->frame_QTxp = ML::QT(0.0f);
		this->frame_QTxm = ML::QT(0.0f);
		this->frame_QTyp = ML::QT(0.0f);
		this->frame_QTym = ML::QT(0.0f);
		this->frame_QTzp = ML::QT(0.0f);
		this->frame_QTzm = ML::QT(0.0f);

		//���̏����l
		this->anckerX = ML::Vec3(1, 0, 0);
		this->anckerY = ML::Vec3(0, 1, 0);
		this->anckerZ = ML::Vec3(0, 0, 1);
	
		//easing set
		easing::Set("Decrese_StickVolumeXP", easing::QUARTIN, 1.7f, 0, 7);
		easing::Set("Decrese_StickVolumeXM", easing::QUARTIN, 1.7f, 0, 7);
		easing::Set("Decrese_StickVolumeYP", easing::QUARTIN, 1.7f, 0, 7);
		easing::Set("Decrese_StickVolumeYM", easing::QUARTIN, 1.7f, 0, 7);
		easing::Set("Decrese_StickVolumeZP", easing::QUARTIN, 1.7f, 0, 7);
		easing::Set("Decrese_StickVolumeZM", easing::QUARTIN, 1.7f, 0, 7);
		//easing set end						 
		easing::Set_End("Decrese_StickVolumeXP");
		easing::Set_End("Decrese_StickVolumeXM");
		easing::Set_End("Decrese_StickVolumeYP");
		easing::Set_End("Decrese_StickVolumeYM");
		easing::Set_End("Decrese_StickVolumeZP");
		easing::Set_End("Decrese_StickVolumeZM");

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
		//�}�b�v�Ɋւ��邱�Ƃ͑S���}�l�[�W�����ł�� ver0.6
		this->Managing(PRECISION);
		
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		
	}

	void  Object::Render3D_L0()
	{
		
		
	}

	//----------------------------------------------------------------------------------
	//�ǉ����\�b�h
	//�}�b�v�̏����������؂�ɊǗ�����
	void Object::Managing(const unsigned int precision)
	{
		
		auto in1 = DI::GPad_GetState("P1");
		//�ق��̃^�X�N�̏���������Ă���
		auto map = ge->GetTask_Group_GN<Map3d::Object>("�}�b�v", "Side");
		auto fence = ge->GetTask_Group_GN<MapFence::Object>("�}�b�v", "Fence");
		auto core = ge->GetTask_One_G<Map_Core::Object>("�}�b�v");
		auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");
		auto cm = ge->GetTask_One_G<CameraMan::Object>("�J�����}��");

		//�{�[�������݂��Ȃ��ꍇ�������Ȃ�
		if (ball == nullptr)
		{
			return;
		}
		//�J�����ړ����Ȃ珈�����Ȃ�
		if (cm->Is_Moving_Now() == true)
		{
			return;
		}

		//�{�[�������݂��Ă���ʂ̖@���x�N�g��
		ML::Vec3 side_Normal(0, 0, 0);
		float delicate = 0.0f;
		//�������@�\
		if (in1.B2.on)
		{
			delicate = (float)precision * 2.0f;
		}
		else
		{
			delicate = (float)precision;
		}

		//�{�[���̑S�h�b�g���i�[����ꏊ
		std::vector<ML::Vec3> all_Points;

		//std::vector<After_Collision> Result;
		for (unsigned int i = 0; i < precision; i++)
		{			
			//��]��easing�Ō����^��������ver0.3
			
			if (ge->game.lock()->GET_READY())
			{
				//���͂���Ă鏊��easing�f�[�^�����Z�b�g������
				if (in1.LStick.axis.y > 0)
				{					
					easing::Re_Start("Decrese_StickVolumeXM");
				}
				else if (in1.LStick.axis.y < 0)
				{
					easing::Re_Start("Decrese_StickVolumeXP");
				}
				if (in1.LStick.axis.x > 0)
				{
					easing::Re_Start("Decrese_StickVolumeYM");
				}
				else if (in1.LStick.axis.x < 0)
				{
					easing::Re_Start("Decrese_StickVolumeYP");
				}
				if (in1.Triger.axis.x <0)
				{
					easing::Re_Start("Decrese_StickVolumeZM");
				}
				else if (in1.Triger.axis.x >0)
				{
					easing::Re_Start("Decrese_StickVolumeZP");
				}
			}
			//easing�f�[�^����]�ʂōX�V�����
			this->frame_QTxp = ML::QT(this->anckerX, ML::ToRadian(-(easing::GetPos("Decrese_StickVolumeXM"))  / delicate));
						
			this->frame_QTxm = ML::QT(this->anckerX, ML::ToRadian((easing::GetPos("Decrese_StickVolumeXP")) / delicate));
			
			
			this->frame_QTym = ML::QT(this->anckerY, ML::ToRadian(-(easing::GetPos("Decrese_StickVolumeYM")) / delicate));

			this->frame_QTyp = ML::QT(this->anckerY, ML::ToRadian((easing::GetPos("Decrese_StickVolumeYP")) / delicate));
						
			
			this->frame_QTzm = ML::QT(this->anckerZ, ML::ToRadian(-(easing::GetPos("Decrese_StickVolumeZM")) / delicate));

			this->frame_QTzp = ML::QT(this->anckerZ, ML::ToRadian((easing::GetPos("Decrese_StickVolumeZP")) / delicate));

			//�t���[����]�ʂ̂܂Ƃ�				
			ML::QT frame_QT_All = this->frame_QTxp * this->frame_QTxm * this->frame_QTyp * this->frame_QTym *this->frame_QTzm * this->frame_QTzp;

			//�e�^�X�N�̉�]�ʂ��X�V�y�щ�]
			core->Rotate_Core_and_Barrier(frame_QT_All);
			for (auto m = map->begin(); m != map->end(); m++)
			{
				(*m)->Map_Rotate(frame_QT_All);
			}
			for (auto f = fence->begin(); f != fence->end(); f++)
			{
				(*f)->Map_Rotate(frame_QT_All);
			}
			
			//�{�[���̊O�p�h�b�g�����o���Ă���
			all_Points.clear();
			ball->Get_Poionts_to_Sphere(&all_Points);

			//�����蔻��͖���}�b�v�̂ق��ōs��	
			ge->collision_Result.clear();					
			
			
			//�{�[���ƃ}�b�v�̂����蔻��y�ѕۑ�
			//�R�A
			core->Core_Check_Hit(all_Points, ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed());
			//��
			for (auto m = map->begin(); m != map->end(); m++)
			{
				//�{�[�������݂��Ă���}�b�v�̖@���x�N�g��������
				if ((*m)->Map_Check_Hit(all_Points, ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed()))
				{
					(*m)->Get_Normal_Side(&side_Normal);
				}
			}
			//�t�F���X
			for (auto f = fence->begin(); f != fence->end(); f++)
			{
				(*f)->Map_Check_Hit(all_Points, ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed());
			}		
			
			//���茋�ʂɖ��ʂȃf�[�^�������Ă���Ȃ��菜��
			ge->collision_Result.erase(remove_if(ge->collision_Result.begin(), ge->collision_Result.end(), [](const After_Collision& c) {return !c.collision_Flag; })
				,ge->collision_Result.end());

			//�{�[�����ړ�������
			ball->Move_Ball(precision);
			//�ʒu�␳���d�|����
			if (ge->collision_Result.size() != 0)
			{
				ball->Fix_Position_for_Rotate(frame_QT_All);
			}			
		}

		
		//�J�����ړI�n���{�[��������ʂ�������悤�ɐݒ�
		if (side_Normal.Is_Zero_Vec() == false && (in1.B4.down || ball->Is_Teleport_Now()))
		{
			cm->Set_Destination(side_Normal);
		}
	}
	//------------------------------------------------------------------------
	//��]���Čv�Z
	void Object::Ancker_Calculating()
	{
		//�J��������}�b�v���S�̃x�N�g�����V����Z��]��
		ML::Vec3 NEW_AnckerZ = ge->Map_center - ge->camera[0]->pos;

		//�V�A��Z��]���̖@���x�N�g���ł܂�������Ȃ���]��������1�v�Z
		ML::Vec3 NEW_AnckerY, NEW_AnckerX;
		//�J�����̏�����x�N�g�����V����Y��]��
		NEW_AnckerY = ge->camera[0]->up;
		//X��]���͐V������̎��̖@��
		MyMath::Get_Normal_to_Vector_Cross(&NEW_AnckerX, NEW_AnckerY, NEW_AnckerZ);

		this->anckerX = NEW_AnckerX.Normalize();
		this->anckerY = NEW_AnckerY.Normalize();
		this->anckerZ = NEW_AnckerZ.Normalize();
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