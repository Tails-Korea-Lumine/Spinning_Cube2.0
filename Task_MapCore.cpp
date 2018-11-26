//-------------------------------------------------------------------
//�}�b�v�̃R�A
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapCore.h"
#include  "Task_Ball.h"

namespace  Map_Core
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->meshName = "MapCore";
		
		DG::Mesh_CreateFromSOBFile(this->meshName, "./data/mesh/Core.sob");
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
	bool  Object::Initialize(Difficult_Range di)
	{
		//ID�o�^�̂��߂Ƀ{�[���^�X�N��������Ă���
		auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������	
		this->mapSize = 8;//��{��8X8��Փx�ɂ���đ��������	
		this->map_QT = ML::QT(0.0f);
		
		//�R�A�̏�����
		this->core = Bbox(BoxType::Core, ge->Map_center, ML::Box3D(-50 * this->mapSize, -50 * this->mapSize, -50 * this->mapSize, 100 * this->mapSize, 100 * this->mapSize, 100 * this->mapSize), this->map_QT,"core");
		ball->Set_Id_And_Flag("core");
		//�o���A�̏�����
		//�e�ʂ��ƃo���A��u����ver0.5
		////�e�ʂɉ����邠���蔻��͈�
		//ML::Box3D XZ, XY, YZ;
		//XZ = ML::Box3D(-100 * (this->mapSize -2), -50 , -100 * (this->mapSize -2), 100 * (this->mapSize + 4), 100, 100 * (this->mapSize + 4));
		//XY = ML::Box3D(-100 * (this->mapSize - 2), -100 * (this->mapSize - 2), -50 , 100 * (this->mapSize + 4), 100 * (this->mapSize + 4), 100);
		//YZ = ML::Box3D(-50 , -100 * (this->mapSize - 2), -100 * (this->mapSize - 2), 100, 100 * (this->mapSize + 4), 100 * (this->mapSize + 4));
		////�ʂ��Ƃ̏����ʒu		
		//this->b_ini_pos[0] = ge->Map_center + ML::Vec3(0.0f, 53.0f * (this->mapSize + 3), 0.0f);
		//this->b_ini_pos[1] = ge->Map_center + ML::Vec3(0.0f, 0.0f, 53.0f * (this->mapSize + 3));
		//this->b_ini_pos[2] = ge->Map_center + ML::Vec3(-53.0f * (this->mapSize + 3), 0.0f, 0.0f);
		//this->b_ini_pos[3] = ge->Map_center + ML::Vec3(53.0f * (this->mapSize + 3), 0.0f, 0.0f);
		//this->b_ini_pos[4] = ge->Map_center + ML::Vec3(0.0f, 0.0f, -53.0f * (this->mapSize + 3));
		//this->b_ini_pos[5] = ge->Map_center + ML::Vec3(0.0f, -53.0f * (this->mapSize + 3), 0.0f);
		////Bbox������
		//this->barrier[0] = Bbox(BoxType::Barrier, b_ini_pos[0] , XZ, this->map_QT);
		//this->barrier[1] = Bbox(BoxType::Barrier, b_ini_pos[1] , XY, this->map_QT);
		//this->barrier[2] = Bbox(BoxType::Barrier, b_ini_pos[2] , YZ, this->map_QT);
		//this->barrier[3] = Bbox(BoxType::Barrier, b_ini_pos[3] , YZ, this->map_QT);
		//this->barrier[4] = Bbox(BoxType::Barrier, b_ini_pos[4] , XY, this->map_QT);
		//this->barrier[5] = Bbox(BoxType::Barrier, b_ini_pos[5] , XZ, this->map_QT);

		//�t�����̃o���A��ł��܂�ver0.6
		this->barrier = Bbox(BoxType::Barrier, ge->Map_center, ML::Box3D(50 * (this->mapSize+2), 50 * (this->mapSize+2), 50 * (this->mapSize+2), -100 * (this->mapSize+2), -100 * (this->mapSize+2), -100 * (this->mapSize+2)), this->map_QT,"barrier");
		ball->Set_Id_And_Flag("barrier");

		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		this->col_Poligons.clear();


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
		//auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");
		//this->frame_QT = ML::QT(0.0f);
		//auto in1 = DI::GPad_GetState("P1");

		////�����蔻��̐����������d�|���� ver0.5
		//
		//{
		//	//�X�e�B�b�N���|���ꂽ�ʂ��X�V
		//	if (in1.B1.on)
		//	{
		//		//�X�e�B�b�N�œ���
		//		if (in1.LStick.axis.x != 0)
		//		{
		//			this->frame_QT = ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-in1.LStick.axis.x ));
		//			this->map_QT *= this->frame_QT;
		//		}
		//	}
		//	//������Ă��Ȃ�����Y����]��X����]
		//	else
		//	{

		//		if (in1.LStick.axis.y != 0)
		//		{
		//			this->frame_QT = ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-in1.LStick.axis.y));
		//			this->map_QT *= this->frame_QT;
		//		}

		//		if (in1.LStick.axis.x != 0)
		//		{
		//			this->frame_QT = ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-in1.LStick.axis.x));
		//			this->map_QT *= this->frame_QT;
		//		}
		//	}

		//	//���[���h��]�ʂɔ��f
		//	ge->World_Rotation = this->map_QT;


		//	//��]
		//	this->Rotate_Core_and_Barrier();


		//	//�����蔻��͖���}�b�v�̂ق��ōs��
		//	//�{�[���̏����C��

		//	if (ball != nullptr)
		//	{
		//		this->Core_Check_Hit(ball->Get_Pos(), ball->Get_Radious(), ball->Get_Speed());
		//	}
		//	//�ʒu�␳���d�|����
		//	/*for (auto p : this->col_Poligons)
		//	{
		//		if (p.collision_Flag)
		//		{
		//			ball->Fix_Position_for_Rotate(this->frame_QT);
		//			break;
		//		}
		//	}*/
		//}
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{

	}

	void  Object::Render3D_L0()
	{
		//�����_�����O�����A�o���A�̓����_�����O�����Ȃ�
		ML::Mat4x4 matW;

		//�A�t�B���ϊ�
		D3DXMatrixAffineTransformation(&matW, this->mapSize * 100.0f, NULL, &this->map_QT,&this->core.Get_Pos());

		DG::EffectState().param.matWorld = matW;

		DG::Mesh_Draw(this->res->meshName);

	}

	//-----------------------------------------------------------------------------------
	//�ǉ����\�b�h

	//��]
	void Object::Rotate_Core_and_Barrier(const ML::QT& qt)
	{
		//�S�̂̉�]�l�X�V
		this->UpDate_Quartanion(qt);

		ML::Mat4x4 matR;
		//matR���A�t�B���ϊ��s��ɍ��
		D3DXMatrixAffineTransformation(&matR, 1.0f, &ge->Map_center, &qt,NULL);
		
		//��]�J�n
		ML::Vec3 temp = matR.TransformCoord(ge->Map_center);
		this->core.Rotate_Box(&matR, qt);
		
		this->barrier.Rotate_Box(&matR, qt);
	}

	//------------------------------------------------------------------------------------
	//�����蔻��

	void Object::Core_Check_Hit(std::vector<ML::Vec3>& all_Points, const ML::Vec3& pos, const float& r, const ML::Vec3& speed)
	{
		//�ڐG�O�p�`�𔻒�O�ɃN���A����	
		this->col_Poligons.clear();

		//����֐�
		//std::vector<After_Collision> poligonC
		this->core.Get_Collision_Poligon(&this->col_Poligons, all_Points, pos, r, speed);
		
		this->barrier.Get_Collision_Poligon(&this->col_Poligons, all_Points, pos, r, speed);

		//�S�̏Փˌ��ʂɕۑ�����
		for (auto& c : this->col_Poligons)
		{
			ge->collision_Result.push_back(c);
		}
		//Graviation_Pull�̍폜�ɂ���Ė߂�l���s�v�ɂȂ���
		//return ball_was_Collision_to_Core;
	}
	//-------------------------------------------------------------------------------------------
	//�N�H�[�^�j�I�����X�V����֐�
	void Object::UpDate_Quartanion(const ML::QT& qt)
	{
		this->map_QT *= qt;
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