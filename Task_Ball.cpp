//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ball.h"
#include  "Task_MapSide.h"
#include  "Task_MapCore.h"
#include  "Task_MapFence.h"
#include <iostream>

namespace  Ball
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->meshName = "Item";
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/Item.SOB");
	
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{		
		DG::Mesh_Erase(this->meshName);		
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
		this->pos = ML::Vec3(1000, 500, 900);//���̈ʒu��Œ�����������(2018/04/20)
		this->speed = ML::Vec3(0, 0, 0);
		this->moveVec = ML::Vec3(0, 0, 0);
		this->r = 30.0f;
		this->m = 15.0f;
		this->rot = 0.0f;
		this->collision_Flag.clear();
		this->teleportation_Flag = false;

	
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->eff_Manager.lock()->Add_Effect(this->Get_Pos(), ML::Vec3(0, 0, 0), BEffect::effType::Game_Clear);
		this->collision_Flag.clear();

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
			
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		//�d�͉���
		this->G.Accelerate(&this->speed,this->m);				
			
		//��]�ʏ㏸
		this->rot += this->speed.Length();

		//�e���|�[�g�t���O�𖳌���
		this->teleportation_Flag = false;
		float sl = this->speed.Length();
		if (sl > 4.0f)
		{
			cout << sl << endl;
		}
	}
	//-------------------------------------------------------------------
	//�����蔻��ɂ������]���y�шړ�
	void Object::Move_Ball(const unsigned int& precisioin)
	{
		//1�t���[���̏I�[���x
		const float termination_Speed = 7.0f;

		//�I�[���x�y�ѕ��������x�ɂ�鑬�x����
		auto Clamp_Speed = [&](ML::Vec3& speed)
		{
			//�I�[���x�ɒ�������ꍇ
			if (speed.Length() > termination_Speed)
			{
				speed = this->speed.Normalize();
				speed *= termination_Speed;
			}
			return speed / (float)precisioin;
		};
		
		//����,�ǂ��������蔻��������ɓ������ꍇ
		//���������Ɏ��̃t���[���Ɉڂ�
		if (ge->collision_Result.size() == 0)
		{
			//����̃t���[���ɏՓ˂��Ȃ�������
			//�Փ˃t���O�𖳌��ɂ���
			for (auto& cf : this->collision_Flag)
			{
				cf.second = false;
			}
			
			//�ړ�(�t���[���I�����钼�O�ɍs��)
			this->pos += Clamp_Speed(this->speed);
			return;
		}
		//�O��t���[���̂����蔻�茋�ʂ��m�F
		
		//���ʂ̐������[�v����
		for (auto p : ge->collision_Result)
		{
			//�O�̃t���[���ŏՓ˂�������A����̃t���[���ł̏Փ˔���ł�邱��
			if (this->Is_Collision(p.collision_Id))
			{
				//����̃t���[���ɏՓ˂�������
				//�΂ߐ�����������
				this->G.Diagonal_Accelerate(&this->speed, p.normal);
			}		
			//�O�̃t���[���ŏՓ˂ł͂Ȃ�������A����̃t���[���ł̏Փ˔���ł�邱��			
			else
			{
				//����̃t���[���ɏՓ˂�������
				//���ˊp�Œ��˕Ԃ�
				this->G.Reflaction_Vector(&this->speed, p.normal);				
			}
		}		

		//�t���O���Փ˔��茋�ʂɂ��Ȃ����̂�false�ɕς���
		for (auto& cf : this->collision_Flag)
		{
			cf.second = false;
		}
		for (auto& p : ge->collision_Result)
		{
			this->collision_Flag.find(p.collision_Id)->second = true;
		}
		
		//�ړ�(�t���[���I�����钼�O�ɍs��)
		this->pos += Clamp_Speed(this->speed);
	}
	
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{		
	}

	void  Object::Render3D_L0()
	{
		ML::Mat4x4 matT;
		ML::Mat4x4 matS;
		ML::Mat4x4 matR;
		//�ړ��s��
		matT.Translation(this->pos);
		//�g�k�s��
		matS.Scaling(this->r);
		//��]�s��
		matR.RotationAxis((ge->Map_center - this->pos), ML::ToRadian(this->rot));
		DG::EffectState().param.matWorld = matS * matR * matT;
		DG::Mesh_Draw(this->res->meshName);
	}

	//--------------------------------------------------------------------
	//�ǉ����\�b�h
	//�O�p�̓_��S�����o��
	void Object::Get_Poionts_to_Sphere(std::vector<ML::Vec3>* result) const
	{
		const int increasing_Dgree = 15;
		//���̏�ɂ���_��S�����o������

		//�ŏ��ɉ�]������_���v�Z
		std::vector<ML::Vec3> points;
		ML::Vec3 y = this->pos + ML::Vec3(0, this->r, 0);

		//�c�����ɐ؂����f�ʂ̕Е�����鏈��
		for (int i = 0; i < 180; i += increasing_Dgree)
		{
			//��]�s�񐶐�
			ML::Mat4x4 matRx;
			ML::QT qtX = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian((float)i));

			D3DXMatrixAffineTransformation(&matRx, 1.0f, &this->pos, &qtX, NULL);

			points.push_back(matRx.TransformCoord(y));
		}

		//���o�����_����]���Ȃ��猋�ʕۑ��p���F�N�^�[�Ƀv�b�V���o�b�N
		for (int d = 0; d < 360; d += increasing_Dgree)
		{
			//��]�s�񐶐�
			ML::Mat4x4 matRy;
			ML::QT qtY = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian((float)d));

			D3DXMatrixAffineTransformation(&matRy, 1.0f, &this->pos, &qtY, NULL);

			for (auto& p : points)
			{
				result->push_back(matRy.TransformCoord(p));
			}
		}
	}
	//-------------------------------------------------------------------------------------
	
	//�����蔻��t���O���m�F
	bool Object::Is_Collision(const string& id) const
	{
		if (this->collision_Flag.count(id) == 0)
		{
			return true;
		}
		if (id == "core" || id == "barrier")
		{
			return true;
		}
		return this->collision_Flag.find(id)->second;
	}

	//---------------------------------------------------------------------
	//���̎��̃{�[���̏���Ԃ��֐�
	//�ʒu
	ML::Vec3 Object::Get_Pos() const
	{
		return this->pos;
	}
	//�����a
	float Object::Get_Radious() const
	{
		return this->r;
	}
	//���x
	ML::Vec3 Object::Get_Speed() const
	{
		return this->speed;
	}

	//--------------------------------------------------------------------
	//�ʒu�␳�p��]�֐�
	void Object::Fix_Position_for_Rotate(const ML::QT& qt)
	{
		//�}�b�v�̃t���[����]�ʂŉ�]������
		ML::Mat4x4 matR;
		D3DXMatrixAffineTransformation(&matR, 1, &ge->Map_center, &qt, NULL);

		this->pos = matR.TransformCoord(this->pos);
	}

	//----------------------------------------------------------------------------
	//�e���|�[�g(�w�肵���ꏊ�ɑ��ړ�����)
	void Object::Teleportation(const ML::Vec3& objectPos)
	{
		this->teleportation_Flag = true;
		this->pos = objectPos;
	}

	//------------------------------------------------------------------------------
	//�R�A�Ƃ������ĂȂ����R�A�̕��Ɉ���������
	/*void Object::Graviation_Pull(const ML::Vec3& force)
	{
		this->speed += force;
	}*/
	//-----------------------------------------------------------------------------
	//�e���|�[�g���������m�F
	bool Object::Is_Teleport_Now() const
	{
		return this->teleportation_Flag;
	}
	//�Փ˃t���O��o�^����
	void Object::Set_Id_And_Flag(const string& id)
	{
		this->collision_Flag.insert(std::pair<string, bool>(id, false));
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