//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include "Task_MapFence.h"
#include  "Task_Ball.h"

namespace  MapFence
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
	bool  Object::Initialize(int sideNum, Difficult_Range di)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();


		//���f�[�^������		
		this->fenceNumber = sideNum;
		this->mapSize = 8;//��{��8X8��Փx�ɂ���đ��������(2018/04/21)
		this->render3D_Priority[0] = 1.0f;
		//�f�[�^�̃[���N���A
		//ZeroMemory(this->arr, sizeof(this->arr));
		//this->sizeX = 0;
		this->size = 0;
		this->chipSize = 100.0f;
		for (int i = 0; i < _countof(this->chipName); i++)
		{
			this->chipName[i] = "";
		}
		this->col_Poligons.clear();
		//�O���t�@�C������̓ǂݍ���
		switch (di)
		{
		case Difficult_Range::Easy:
			this->Map_Load("./data/fence/Easy/line" + to_string(sideNum) + ".txt");
			break;
		case Difficult_Range::Normal:
			this->Map_Load("./data/fence/Normal/line" + to_string(sideNum) + ".txt");
			break;
		case Difficult_Range::Hard:
			this->Map_Load("./data/fence/Hard/line" + to_string(sideNum) + ".txt");
			break;
		}
		//�ʂ��Ƃɕʂ̏����l��^����
		switch(fenceNumber)
		{
		case 0:
		case 8:
			this->Map_Rotate( ML::QT(0.0f));
			break;
		case 9:
		case 1:
			this->Map_Rotate(ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(90)));
			break;
		case 10:
		case 2:
			this->Map_Rotate(ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(180)));
			break;
		case 11:
		case 3:
			this->Map_Rotate(ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-90)));
			break;
		case 4:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)));
			break;
		case 5:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)) * ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(90)));
			break;
		case 6:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)) * ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(180)));
			break;
		case 7:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)) * ML::QT(ML::Vec3(0, 1, 0), ML::ToRadian(-90)));
			break;
		}
		this->Check_Unusable_Side();
		this->Array_Sorting();
		this->Insert_Id_To_Ball();
		//���^�X�N�̐���

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		for (int i = 0; i < _countof(this->chipName); i++)
		{
			if (this->chipName[i] != "")
			{
				DG::Mesh_Erase(this->chipName[i]);
			}
		}
		//this->sizeX = 0;
		this->size = 0;
		this->col_Poligons.clear();

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
		
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		
	}

	void  Object::Render3D_L0()
	{		
		//��Ƀ����_�����O���K�v�Ȃ̂����m�F����
		/*if (this->Is_Need_Render(0) == false)
		{
			return;
		}*/
		
		ML::Mat4x4 matW;
		//matS.Scaling(this->chipSize);
		for (size_t i = 0; i < this->size; i++)
		{
			
			BoxType now_Type = this->arr[i].What_Type_Is_this_Box();
			//���̓����_�����O���Ȃ�
			if (now_Type == BoxType::Road || now_Type == BoxType::Clear)
			{
				return;
			}
			//�ʂɂ������_�����O���K�v�����m�F(�t���[�����オ�邩�̃e�X�g)
			if (this->Is_Need_Render(i) == false)
			{
				continue;
			}

			//�`��
			D3DXMatrixAffineTransformation(&matW, this->chipSize, NULL, &this->map_QT, &this->arr[i].Get_Pos());

			DG::EffectState().param.matWorld = matW;
			DG::Mesh_Draw(this->chipName[(int)now_Type]);
				
			
		}
	}

	//-----------------------------------------------------------------------------------
	//�ǉ����\�b�h		
	//�O���t�@�C������̃}�b�v���[�h
	bool Object::Map_Load(string f_)
	{		
		//�O���t�@�C������ǂݍ���
		ifstream fin(f_);
		if (!fin)
		{
			return false;
		}
		int chipNum;
		fin >> chipNum;
		//�`�b�v�t�@�C�����ǂݍ��݂ƁA���b�V���̃��[�h
		for (int c = 1; c <= chipNum; c++)
		{
			string chipFileName, chipFilePath;
			fin >> chipFileName;
			chipFilePath = "./data/mesh/" + chipFileName;
			this->chipName[c] = "Fence_Chip" + std::to_string(c);
			DG::Mesh_CreateFromSOBFile(this->chipName[c], chipFilePath);
		}
		//�}�b�v�z��T�C�Y�̓ǂݍ���
		fin >> this->size;
		//�}�b�v�z��f�[�^�̓ǂ݂���
		for (size_t i = 0; i < this->size; i++)
		{			
			//�`�b�v�ԍ�(�{�b�N�X�^�C�v)�ǂݍ���
			int chip;
			fin >> chip;
			//�}�b�v��������ɂ��������ʒu�Z�o
			ML::Vec3 pos;

			if (this->fenceNumber < 8)
			{
				pos = ML::Vec3(i * this->chipSize + this->chipSize / 2, this->chipSize + this->chipSize / 2, this->chipSize + this->chipSize / 2);
				pos += ge->Map_center - ML::Vec3((this->mapSize*this->chipSize / 2), -((this->mapSize-2)*this->chipSize / 2), -((this->mapSize - 2)*this->chipSize / 2));
			}
			else
			{
				pos = ML::Vec3(this->chipSize + this->chipSize / 2, i * this->chipSize + this->chipSize / 2, this->chipSize + this->chipSize / 2);
				pos += ge->Map_center - ML::Vec3(((this->mapSize + 4)*this->chipSize / 2), (this->mapSize*this->chipSize / 2), -((this->mapSize - 2)*this->chipSize / 2));
			}
			//�����蔻��p��`
			ML::Box3D base = ML::Box3D(-this->chipSize / 2, -this->chipSize / 2, -this->chipSize / 2, this->chipSize, this->chipSize, this->chipSize);				

			//�{�b�N�X��ID����
			string id = to_string(this->fenceNumber) + to_string(i);			
			//�z��ɓo�^
			this->arr[i] = Bbox(BoxType(chip), pos, base, this->map_QT,id);
				
			
		}
		fin.close();
		return true;
	}
	//-----------------------------------------------------------------------
	void Object::Map_Check_Hit(std::vector<ML::Vec3>& all_Points, const ML::Vec3& pos, const float& r, const ML::Vec3& speed)
	{
		//���d�Փ˂܂œK�p����ver0.3(2018/04/16)

		//�ڐG�O�p�`�𔻒�O�ɃN���A����
		this->col_Poligons.clear();

		//����X�^�[�g
		for (size_t i = 0; i < this->size; i++)
		{
			auto now_Type = this->arr[i].What_Type_Is_this_Box();
			//���͔z��̌��ɐς߂Ă������̂Ŕ��������炻�̌�͏���������break
			if (now_Type == BoxType::Road || now_Type == BoxType::Clear)
			{
				break;
			}
			//��苗���ȓ��̂��̂������������
			ML::Vec3 d = this->arr[i].Get_Pos() - pos;
			//d�͐�Βl�̋���					
			//��苗���ȏゾ�����画�肹�����ɍ��ڂ�
			if (d.Length() > this->chipSize)
			{
				continue;
			}
													
			//this->collision_Tri = this->col.Hit_Check(Mass, pos, r, this->map_QT); //(ver0.2�Ŏg��������)
			//std::vector<After_Collision> poligon 
			if (!this->arr[i].Get_Collision_Poligon(&this->col_Poligons, all_Points, pos, r, speed))
			{
				continue;
			}
					

			//�S�̏Փˌ��ʂɕۑ�����
			for (auto& c : this->col_Poligons)
			{
				ge->collision_Result.push_back(c);
			}
			
		}
	}

	//------------------------------------------------------------------------
	//��]������
	void Object::Map_Rotate(const ML::QT& qt)
	{
		//�S�̂̉�]�l�X�V
		this->UpDate_Quartanion(qt);

		for (size_t i = 0; i < this->size; i++)
		{			
			//��]�s�񐶐�
			ML::Mat4x4 matR;
			D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, &ge->Map_center, &qt, NULL);
			//�{�b�N�X�Ɍʂœn��
			this->arr[i].Rotate_Box(&matR, qt);				
			
		}
	}

	//-------------------------------------------------------------------------------------------
	//�N�H�[�^�j�I�����X�V����֐�
	void Object::UpDate_Quartanion(const ML::QT& qt)
	{
		this->map_QT *= qt;
	}
	//------------------------------------------------------------------------------------------------------------
	//�A�����Ă��Ďg���Ȃ��ʂ�T��
	void Object::Check_Unusable_Side()
	{
		//�ŏ��l�A�ő�l�𒴂��Ȃ��悤�ɂ���
		auto Inside_Range = [this](const size_t& ci)
		{return ci >= 0 && ci < this->size; };

		//���������������ǂȂ̂����f����
		auto Judge = [](const Bbox& b1, const Bbox& b2)
		{return b1.What_Type_Is_this_Box() == BoxType::Wall && b2.What_Type_Is_this_Box() == BoxType::Wall; };
		//�e�{�b�N�X�ɘA������{�b�N�X�����邩���m�F
		for (size_t i = 0; i < this->size; i++)
		{			
			//���ʊJ�n
			//��
			if (this->fenceNumber < 8)
			{
				if (Inside_Range(i - 1) && Judge(this->arr[i - 1], this->arr[i]))
				{
					//�����|���S����\�����Ă���
					this->arr[i].Marking_On_Unusable_Poligon(Box_Side::Xminus);
				}
				//�E
				if (Inside_Range(i + 1) && Judge(this->arr[i + 1], this->arr[i]))
				{
					//�����|���S����\�����Ă���
					this->arr[i].Marking_On_Unusable_Poligon(Box_Side::Xplus);
				}
			}
			else
			{
				//��
				if (Inside_Range(i - 1) && Judge(this->arr[i - 1], this->arr[i]))
				{
					//�����|���S����\�����Ă���
					this->arr[i].Marking_On_Unusable_Poligon(Box_Side::Yminus);
				}
				//��
				if (Inside_Range(i + 1) && Judge(this->arr[i + 1], this->arr[i]))
				{
					//�����|���S����\�����Ă���
					this->arr[i].Marking_On_Unusable_Poligon(Box_Side::Yplus);
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------
	//�z��\�[�g�y�у{�[�����X�^�[�g�ʒu�ɒu��
	void Object::Array_Sorting()
	{
		//stl remove_if�ō폜(�����f�[�^�ɏ㏑������)�Ƃ�������炢�Ȃ���
		//�f�[�^��O�ɐς߂�
		auto remove_Point = remove_if(&this->arr[0], &this->arr[this->size], [](const Bbox& b) {return b.What_Type_Is_this_Box() == BoxType::Road; });

		//�����f�[�^�ɏ㏑������
		for (; remove_Point != &this->arr[this->size]; remove_Point++)
		{
			*remove_Point = Bbox();
		}
	}
	//�{�[���^�X�N�̃t���O��ID��g�ݍ��߂�
	void Object::Insert_Id_To_Ball()
	{
		//ID�o�^�̂��߂Ƀ{�[���^�X�N��������Ă���
		auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");

		for (size_t i = 0; i < this->size; i++)
		{			
			auto now_Type = this->arr[i].What_Type_Is_this_Box();
			//���͔z��̌��ɐς߂Ă������̂Ŕ��������炻�̌�͏���������break
			if (now_Type == BoxType::Road || now_Type == BoxType::Clear)
			{
				break;
			}
			//����ȊO�̓t���O�o�^
			ball->Set_Id_And_Flag(this->arr[i].Get_Id());
			
		}
	}

	//---------------------------------------------------------------------------------------------
	//�����_�����O���邩���Ȃ������m�F���邽�߂̃��\�b�h
	bool Object::Is_Need_Render(const unsigned int& index)
	{
		ML::Vec3 d_Cmc = (ge->Map_center - ge->camera[0]->pos);
		const float judge = d_Cmc.Length() + (this->chipSize * this->mapSize / 3.0f);

		//���肷�鋗�����}�b�v��0�ԂƃJ�����Ƃ̋����������Ȃ烌���_�����O���Ȃ�
		ML::Vec3 d_Cf0 = this->arr[index].Get_Pos() - ge->camera[0]->pos;

		//�}���������ŕԂ�
		return d_Cf0.Length() < judge ? true : false;

	}

	//������������������������������������������������������������������������������������
	//�ȉ��͊�{�I�ɕύX�s�v�ȃ��\�b�h
	//������������������������������������������������������������������������������������
	//-------------------------------------------------------------------
	//�^�X�N��������
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, int sideNum, Difficult_Range di)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize(sideNum,di)) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(int sideNum, Difficult_Range di)
	{
		return  this->Initialize(sideNum,di);
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