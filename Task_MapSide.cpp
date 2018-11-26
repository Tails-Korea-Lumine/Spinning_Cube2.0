//-------------------------------------------------------------------
//�L���[�u�O�ǂ̃}�b�v
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_MapSide.h"
#include  "Task_Ball.h"
#include  "Task_Game.h"
#include  "Task_CameraMan.h"


namespace  Map3d
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->seTeleportIn = "SETeleIn";
		DM::Sound_CreateSE(this->seTeleportIn, "./data/sound/TeleportIn.wav");
		this->seTeleportOut = "SETeleOut";
		DM::Sound_CreateSE(this->seTeleportOut, "./data/sound/TeleportOut.wav");
		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{		
		DM::Sound_Erase(this->seTeleportOut);
		DM::Sound_Erase(this->seTeleportIn);
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
		this->render3D_Priority[0] = 0.3f;
		this->gimicCnt = 0;		
		this->sideNumber = sideNum;
		this->mapSize = 8;//��{��8X8
		this->rendering_Judge = _CMATH_::cosf(ML::ToRadian(70));
		//this->render3D_Priority[0] = 1.0f;
		//�f�[�^�̃[���N���A
		//ZeroMemory(this->arr, sizeof(this->arr));
		this->sizeX = 0;
		this->sizeZ = 0;
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
			this->Map_Load("./data/map/Easy/map" + to_string(sideNum) + ".txt");
			break;
		case Difficult_Range::Normal:
			this->Map_Load("./data/map/Normal/map" + to_string(sideNum) + ".txt");
			break;
		case Difficult_Range::Hard:
			this->Map_Load("./data/map/Hard/map" + to_string(sideNum) + ".txt");
			break;
		}
		//�ʂ��Ƃɕʂ̏����l��^����
		switch (sideNumber)
		{
		case 0:
			this->Map_Rotate(ML::QT(0.0f));
			this->Normal_Side = ML::Vec3(0, 1, 0);
			break;
		case 1:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(90)));
			this->Normal_Side = ML::Vec3(0, 0, 1);
			break;
		case 2:
			this->Map_Rotate(ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(90)));
			this->Normal_Side = ML::Vec3(-1, 0, 0);
			break;
		case 3:
			this->Map_Rotate(ML::QT(ML::Vec3(0, 0, 1), ML::ToRadian(-90)));
			this->Normal_Side = ML::Vec3(1, 0, 0);
			break;
		case 4:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-90)));
			this->Normal_Side = ML::Vec3(0, 0, -1);
			break;
		case 5:
			this->Map_Rotate(ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(180)));
			this->Normal_Side = ML::Vec3(0, -1, 0);
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
		this->sizeX = 0;
		this->sizeZ = 0;
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
		//�����_�����O���邩���Ɋm�F
		if (this->Is_Need_Render() == false)
		{
			//�K�v�Ȃ��Ƃ��͂��̂܂܏����I��
			return;
		}
				
		//�s�񐶐�
		ML::Mat4x4 matW;
		size_t z, x;

		for (z = 0; z < this->sizeZ; z++)
		{
			for (x = 0; x < this->sizeX; x++)
			{
				//���͔z��̌��ɐς߂Ă������̂Ŕ��������炻�̌�͏���������break
				BoxType now_Type = this->arr[z][x].What_Type_Is_this_Box();
				if (now_Type == BoxType::Road || now_Type == BoxType::Clear)
				{
					break;
				}				
				else if (this->Is_Need_Render(z, x) == false)
				{
					continue;
				}
				//�A�t�B���ϊ�
				D3DXMatrixAffineTransformation(&matW, this->chipSize , NULL, &this->map_QT, &this->arr[z][x].Get_Pos());				
				//���[���h�s��ɏ㏑��
				DG::EffectState().param.matWorld = matW;
				//�����_�����O
				DG::Mesh_Draw(this->chipName[(int)now_Type]);
			}
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
		//�����_�����O���郁�b�V���̌�
		int chipNum;
		fin >> chipNum;
		//�`�b�v�t�@�C�����ǂݍ��݂ƁA���b�V���̃��[�h
		for (int c = 1; c <= chipNum; c++)
		{
			string chipFileName, chipFilePath;
			fin >> chipFileName;
			chipFilePath = "./data/mesh/" + chipFileName;
			this->chipName[c] = "Map" + to_string(this->sideNumber) + "Chip" + std::to_string(c);
			DG::Mesh_CreateFromSOBFile(this->chipName[c], chipFilePath);
		}
		//�}�b�v�z��T�C�Y�̓ǂݍ���
		fin >> this->sizeX >> this->sizeZ;
		//�}�b�v�z��f�[�^�̓ǂ݂���
		
		for (size_t z = 0; z < this->sizeZ; z++)
		{
			for (size_t x = 0; x < this->sizeX; x++)
			{
				//�`�b�v�ԍ�(�{�b�N�X�^�C�v)�ǂݍ���
				int chip;
				fin >> chip;
				//�}�b�v��������ɂ��������ʒu�Z�o
				ML::Vec3 pos = ML::Vec3(x*this->chipSize + this->chipSize / 2, this->chipSize + this->chipSize / 2, (this->sizeZ - 1 - z)*this->chipSize + this->chipSize / 2);
				pos += ge->Map_center - ML::Vec3((this->mapSize*this->chipSize / 2), -((this->mapSize - 2)*this->chipSize / 2), (this->mapSize*this->chipSize / 2));
				//�����蔻��p��`
				ML::Box3D base = ML::Box3D(-this->chipSize / 2, -this->chipSize / 2, -this->chipSize / 2, this->chipSize, this->chipSize, this->chipSize);				
				
				//�������邱�ƈȊO�ɉ���������������K�v���������
				switch (BoxType(chip))
				{
				//�e���|�[�g
				case BoxType::Teleportaion:		

					ge->TM.Update_Door_Position(this->sideNumber, pos);
					break;
				//�X�C�b�`�͂����蔻��͈͂�������	
				case BoxType::LightSwitch:
					base = ML::Box3D(base.x / 10, base.y / 10, base.z / 10, base.w / 10, base.h / 10, base.d / 10);
					break;
				case BoxType::Start:
					//�{�[�����X�^�[�g�ʒu�ɒu��
					ML::Mat4x4 matR;
					D3DXMatrixAffineTransformation(&matR, 1.0f, &ge->Map_center, &ML::QT(ML::Vec3(1, 0, 0), ML::ToRadian(-90)), NULL);
					pos = matR.TransformCoord(pos);
					ge->GetTask_One_G<Ball::Object>("�{�[��")->Teleportation(pos);
					chip = (int)BoxType::Road;
					break;
				}
				//�{�b�N�X��ID����
				string id = to_string(this->sideNumber) + to_string(z) + to_string(x);				
				//�z��ɓo�^
				this->arr[z][x] = Bbox((BoxType)chip, pos, base, this->map_QT,id);
			}
			
		}
		fin.close();
		return true;
	}
	//-----------------------------------------------------------------------
	bool Object::Map_Check_Hit(std::vector<ML::Vec3>& all_Points, const ML::Vec3& pos, const float& r, const ML::Vec3& speed)
	{
		//�{�[���ɉ�����������Ƃ�������ꍇ�g��
		auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");
		//�{�[�������̖ʂɂ��邩���m�F���邽�߂̃t���O	
		bool ball_on_This_Side = false;
		//�e���|�[�g�A�E�g����ꏊ
		ML::Vec3 exitpos;

		//����X�^�[�g		
		for (size_t z = 0; z < this->sizeZ; z++)
		{
			for (size_t x = 0; x < this->sizeX; x++)
			{
				//�ڐG�O�p�`�𔻒�O�ɃN���A����
				this->col_Poligons.clear();
				//���͔z��̌��ɐς߂Ă������̂Ŕ��������炻�̌�͏���������break
				BoxType now_Type = this->arr[z][x].What_Type_Is_this_Box();
				if (now_Type == BoxType::Road || now_Type == BoxType::Clear)
				{
					break;
				}
				//��苗���ȓ��̂��̂������������
				ML::Vec3 d = this->arr[z][x].Get_Pos() - pos;
				//d�͐�Βl�̋���(distance)				
				//��苗���ȏゾ�����画�肹�����ɍ��ڂ�
				if (d.Length() > this->chipSize)
				{
					continue;
				}
				ball_on_This_Side = true;

				//����J�n
				if (!this->arr[z][x].Get_Collision_Poligon(&this->col_Poligons, all_Points, pos, r, speed))
				{
					//�������ĂȂ��}�X�Ȃ玟�̃}�X�Ɉڍs
					continue;
				}				

				//�M�~�b�N����
				switch (now_Type)
				{
				//�S�[�����̓N���A����
				case BoxType::Goal:										
					ball->Teleportation(pos + (d*0.01f));
					//�Q�[���^�X�N�ɃN���A������������
					ge->game.lock()->Game_Clear();
					break;
				//���̓e���|�[�g
				case BoxType::Teleportaion:					
					if (ge->TM.Find_Exit(this->sideNumber, &exitpos))
					{
						ball->Teleportation(exitpos);
						auto eff = ge->eff_Manager.lock();
						//�e���|�[�g�C���̃G�t�F�N�g
						eff->Add_Effect(pos, this->arr[z][x].Get_Pos(), ML::Vec3(0, 0, 0), BEffect::effType::Teleportin);
						//�e���|�[�g�C���̉���炷
						DM::Sound_Play(this->res->seTeleportIn, false);
						//�e���|�[�g�A�E�g�̃G�t�F�N�g
						eff->Add_Effect(exitpos, this->Normal_Side, BEffect::effType::TeleportOut);
						//�e���|�[�g�A�E�g�̉���炷
						DM::Sound_Play(this->res->seTeleportOut, false);
					}
					break;
				//�X�C�b�`�͌���������
				case BoxType::LightSwitch:
					//�J�����}���Ƀ��C�g��3�b�ԃI�t���閽�߂𑗂�
					ge->GetTask_One_G<CameraMan::Object>("�J�����}��")->Turnoff_the_Light();
					break;
				//�M�~�b�N�łȂ���Q���͂����蔻�茋�ʂ�ۑ�����
				default:
					for (auto& c : this->col_Poligons)
					{
						ge->collision_Result.push_back(c);
					}
					break;
				}
			}
			
		}
		//�߂�l�̓{�[�������̖ʂɂ��邩���m�F�����t���O
		return ball_on_This_Side;
	}

	//------------------------------------------------------------------------
	//��]������
	void Object::Map_Rotate(const ML::QT& qt)
	{
		//�}�b�v�S�̂̉�]�l�X�V
		this->UpDate_Quartanion(qt);

		for (size_t z = 0; z < this->sizeZ; z++)
		{
			for (size_t x = 0; x < this->sizeX; x++)
			{				
				//��]�s�񐶐�
				ML::Mat4x4 matR;
				D3DXMatrixAffineTransformation(&matR, this->chipSize / 100.0f, &ge->Map_center, &qt, NULL);				

				//�{�b�N�X�Ɍʂœn��
				this->arr[z][x].Rotate_Box(&matR, qt);
				//�e���|�[�g���̈ʒu�X�V
				BoxType now_Type = this->arr[z][x].What_Type_Is_this_Box();
				if (now_Type == BoxType::Teleportaion)
				{
					ge->TM.Update_Door_Position(this->sideNumber, this->arr[z][x].Get_Pos());
				}
			}
		}
		
	}

	//-------------------------------------------------------------------------------------------
	//�N�H�[�^�j�I�����X�V����֐�
	void Object::UpDate_Quartanion(const ML::QT& qt)
	{
		this->map_QT *= qt;

		//�@���x�N�g������]�ʂɉ����ē�������]���s��
		ML::Mat4x4 matR;
		D3DXMatrixAffineTransformation(&matR, 1.0f, NULL, &qt, NULL);

		this->Normal_Side = matR.TransformNormal(this->Normal_Side);
	}

	//------------------------------------------------------------------------------------------
	//�����_�����O���邩���m�F����
	bool Object::Is_Need_Render()
	{
		//�J�����Ƃ̓��ϒl��ۑ�����ꏊ
		float c = 0.0f;
		//�����x�N�g��
		ML::Vec3 sv = ge->camera[0]->target - ge->camera[0]->pos;
		MyMath::Vector_Dot(&c, sv, this->Normal_Side);

		//���ϒl��-90 < cos < 90�̊Ԃ̓����_�����O�����Ȃ�
		return c <= this->rendering_Judge;
	}
	//---------------------------------------------------------------------------------------------
	//�����_�����O���邩���Ȃ������m�F���邽�߂̃��\�b�h(�}�X��)
	bool Object::Is_Need_Render(const unsigned int& z ,const unsigned int& x)
	{
		ML::Vec3 d_Cmc = (ge->Map_center - ge->camera[0]->pos);
		const float judge = d_Cmc.Length() + (this->chipSize * this->mapSize / 3.0f);

		//���肷�鋗�����}�X�ƃJ�����Ƃ̋����������Ȃ烌���_�����O���Ȃ�
		ML::Vec3 d_Cf0 = this->arr[z][x].Get_Pos() - ge->camera[0]->pos;

		//�}���������ŕԂ�
		return d_Cf0.Length() < judge;

	}
	//-----------------------------------------------------------------------------------------
	//�}�b�v�̖@���x�N�g����Ԃ����\�b�h
	void Object::Get_Normal_Side(ML::Vec3* result)const
	{
		*result = this->Normal_Side;
	}
	//�A�����Ă��Ďg���Ȃ��ʂ�T��
	void Object::Check_Unusable_Side()
	{
		//�ŏ��l�A�ő�l�𒴂��Ȃ��悤�ɂ���
		auto Inside_Range = [this](const size_t& cx, const size_t& cz)
		{return cx >= 0 && cx < this->sizeX && cz >= 0 && cz < this->sizeZ; };

		//���������������ǂȂ̂����f����
		auto Judge = [](const Bbox& b1, const Bbox& b2)
		{return b1.What_Type_Is_this_Box() == BoxType::Wall && b2.What_Type_Is_this_Box() == BoxType::Wall; };
		//�e�{�b�N�X�ɘA������{�b�N�X�����邩���m�F
		for (size_t z = 0; z < this->sizeZ; z++)
		{
			for (size_t x = 0; x < this->sizeX; x++)
			{
				//���ʊJ�n
				//��
				if (Inside_Range(x, z-1) && Judge(this->arr[z - 1][x], this->arr[z][x]))
				{
					//�����|���S����\�����Ă���
					this->arr[z][x].Marking_On_Unusable_Poligon(Box_Side::Zplus);
				}
				//��O
				if (Inside_Range(x, z + 1) && Judge(this->arr[z + 1][x], this->arr[z][x]))
				{
					//�����|���S����\�����Ă���
					this->arr[z][x].Marking_On_Unusable_Poligon(Box_Side::Zminus);
				}
				//��
				if (Inside_Range(x - 1, z) && Judge(this->arr[z][x - 1], this->arr[z][x]))
				{
					//�����|���S����\�����Ă���
					this->arr[z][x].Marking_On_Unusable_Poligon(Box_Side::Xminus);
				}
				//�E
				if (Inside_Range(x + 1, z) && Judge(this->arr[z][x+1], this->arr[z][x]))
				{
					//�����|���S����\�����Ă���
					this->arr[z][x].Marking_On_Unusable_Poligon(Box_Side::Xplus);
				}
			}
		}
	}

	//-------------------------------------------------------------------------------------
	//�z��\�[�g�y�у{�[�����X�^�[�g�ʒu�ɒu��
	void Object::Array_Sorting()
	{
		////�ꎞ�I�ɃR�s�[����ꏊ
		//Bbox temp;
		////�z��̃{�b�N�X�^�C�v�����Ȃ�Ό��ɒu��
		//for (size_t z = 0; z < this->sizeZ; z++)
		//{
		//	for (size_t x = 0; x < this->sizeX - 1; x++)
		//	{
		//		//���̃{�b�N�X�͌��ɐς߂�
		//		if (this->arr[z][x].What_Type_Is_this_Box() == BoxType::Road)
		//		{					
		//			temp = this->arr[z][x];
		//			//1��̂��̂ɏ㏑������
		//			for (size_t i = x; i < this->sizeX - 1; i++)
		//			{
		//				this->arr[z][i] = this->arr[z][i + 1];
		//			}
		//			this->arr[z][this->sizeX - 1] = temp;					
		//		}				
		//	}
		//	//�����Ɛς�ł��邩���m�F
		//	for (size_t x = 0; x < this->sizeX-1; x++)
		//	{
		//		//���𔭌������̂�1��̂��̂����ȊO��������
		//		if (this->arr[z][x].What_Type_Is_this_Box() == BoxType::Road && this->arr[z][x+1].What_Type_Is_this_Box() != BoxType::Road)
		//		{
		//			//�ς߂鏈���̂�蒼��
		//			z--;
		//			break;
		//		}
		//	}
		//}

		//2�����z��Ȃ̂ŁAsizeZ���\�[�g��������
		for (size_t z = 0; z < this->sizeZ; z++)
		{
			//stl remove_if�ō폜(�����f�[�^�ɏ㏑������)�Ƃ�������炢�Ȃ���
			//�f�[�^��O�ɐς߂�
			auto remove_Point = remove_if(&this->arr[z][0], &this->arr[z][this->sizeX], [](const Bbox& b) {return b.What_Type_Is_this_Box() == BoxType::Road; });

			//�����f�[�^�ɏ㏑������
			for (; remove_Point != &this->arr[z][this->sizeX]; remove_Point++)
			{
				*remove_Point = Bbox();
			}
		}
	}
	//�{�[���^�X�N�̃t���O��ID��g�ݍ��߂�
	void Object::Insert_Id_To_Ball()
	{
		//ID�o�^�̂��߂Ƀ{�[���^�X�N��������Ă���
		auto ball = ge->GetTask_One_G<Ball::Object>("�{�[��");

		for (size_t z = 0; z < this->sizeZ; z++)
		{
			for (size_t x = 0; x < this->sizeX; x++)
			{
				auto now_Type = this->arr[z][x].What_Type_Is_this_Box();
				//���͔z��̌��ɐς�ł������̂Ō���������break
				if (now_Type == BoxType::Road || now_Type == BoxType::Clear)
				{
					break;
				}
				//����ȊO�̓t���O�o�^
				ball->Set_Id_And_Flag(this->arr[z][x].Get_Id());
			}
		}
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