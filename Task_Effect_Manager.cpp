//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Effect_Manager.h"
#include  "easing.h"

namespace  EffectManager
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
		//�G�t�F�N�g���X�g���N���A
		this->efList.clear();
		this->play_Effect_List.clear();

		//�g�p����G�t�F�N�g���b�V���̐���
		//�e���|�[�g�C��
		DG::Mesh_CreateFromSOBFile("DestroyItem", "./data/mesh/effect/DestroyItem.SOB");
		//�e���|�[�g�A�E�g
		DG::Mesh_CreateFromSOBFile("CreateItem", "./data/mesh/effect/CreateItem.SOB");
		//�Q�[���N���A
		DG::Mesh_CreateFromSOBFile("DestroyCharactor", "./data/mesh/effect/DestroyPlayer.SOB");
		
		//�K�v�ȃG�t�F�N�g����
		this->Create_Effect(BEffect::effType::Teleportin);
		this->Create_Effect(BEffect::effType::TeleportOut);
		this->Create_Effect(BEffect::effType::Game_Clear);

		this->render3D_Priority[0] = 1.0f;		

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		DG::Mesh_Clear();
		if (this->efList.size() != 0)		
		{
			//���X�g�̊J��
			for (auto e : this->efList)
			{
				delete e;
			}
		}
		this->efList.clear();
		this->play_Effect_List.clear();
		/*for (auto it = this->efList.begin(); it != this->efList.end(); it++)
		{
			if ((*it) != nullptr)
			{
				delete (*it);
			}
		}*/
		
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
		//easing function update
		easing::UpDate();
		//�O�̂��߃v���C���X�g�T�C�Y�̏�������߂Ă���
		if (this->play_Effect_List.size() > 20)
		{
			this->play_Effect_List.pop_front();
		}
		//�G�t�F�N�g�̃t���[�����̕ω�
		for (auto it = this->play_Effect_List.begin(); it != this->play_Effect_List.end(); it++)
		{
			if ((*it).Get_Type() == BEffect::effType::CLEAR)
			{
				continue;
			}
			(*it).UpDate_Effect();
		}
		//��������
		this->Dec_Effect_Life();
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
	}

	void  Object::Render3D_L0()
	{
		//�G�t�F�N�g�̍Đ�
		for (auto it = this->play_Effect_List.begin(); it != this->play_Effect_List.end(); it++)
		{
			if ((*it).Get_Type() == BEffect::effType::CLEAR)
			{
				continue;
			}
			(*it).Playing_Effect();
		}
	}

	//--------------------------------------------------------------------------------------
	//�ǉ����\�b�h
	//-----------------------------------------------------------------------------
	void Object::Add_Effect(const ML::Vec3& pos, const ML::Vec3& angle, const BEffect::effType& handle)
	{	
		Effect ef(BEffect::effType::CLEAR);
		//�v���C���X�g�ɓ���G�t�F�N�g���R�s�[����
		for (auto& el : this->efList)
		{
			if (el->Get_Type() == handle)
			{
				ef = *el;
			}
		}
		//�G�t�F�N�g���ݒ�
		ef.Load_Eff(pos, angle);
		//�v���C���X�g��push_back
		this->play_Effect_List.push_back(ef);
	}
	void Object::Add_Effect(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle, const BEffect::effType& handle)
	{
		Effect ef(BEffect::effType::CLEAR);
		//�v���C���X�g�ɓ���G�t�F�N�g���R�s�[����
		for (auto& el : this->efList)
		{
			if (el->Get_Type() == handle)
			{
				ef = *el;
			}
		}
		//�G�t�F�N�g���ݒ�
		ef.Load_Eff(pos, target, angle);
		//�v���C���X�g��push_back
		this->play_Effect_List.push_back(ef);
	}
	//-----------------------------------------------------------------------------------
	//�V�����G�t�F�N�g����
	void Object::Create_Effect(const BEffect::effType& handle)
	{
		Effect* NewEf = new Effect(handle);
		//�S�̃��X�g��push_back
		this->efList.push_back(NewEf);
	}
	//----------------------------------------------------------------------------------
	//�G�t�F�N�g�������炵�y�э폜
	void Object::Dec_Effect_Life()
	{
		if (this->play_Effect_List.size() == 0)
		{
			return;
		}
		for (auto it = this->play_Effect_List.begin(); it != this->play_Effect_List.end(); it++)
		{
			if ((*it).Is_Alive())
			{
				(*it).Dec_Eff();
			}
		}
		//���C�t���O�ɂȂ����G�t�F�N�g�̓��X�g����O��		
		this->play_Effect_List.remove_if([](Effect& e) {return e.Eff_Judge(); });
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