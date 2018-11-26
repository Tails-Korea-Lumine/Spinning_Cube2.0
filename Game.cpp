#include "MyPG.h"
#include "MyGameMain.h"
enum state
{
	Non,
	Normal,
	Dead
};
enum Angle4
{
	Up = 270,
	Down = 90,
	Left = 180,
	Right = 0,
};

//�Q�[���{��
namespace Game
{
	ML::Mat4x4 matR;
	//�}�b�v�̏��
	struct Map3D
	{
		int arr[30][30][30];
		int sizeZ, sizeY, sizeX;		
		float chipSize;
		ML::Box3D hitBase;
		string chipName[10];
	};

	//�L�����N�^�[�̏��
	struct Chara
	{
		state player_state;
		string meshName;
		ML::Vec3 pos;
		ML::Box3D hitBase;
		ML::Vec3 angle;
		Angle4 playerAngle;
		int tailsize;
		float speed;
		float fallspeed;
		float jumppow;
		bool viewMode; // false = �ʏ� true = �g�b�v�r���[
		float dist; //����
		float ajast_CM;//���_�̍����𒲐�
		float ajast_TG;//�����_�̍����𒲐�
		float ajast_CM_Max;//���_�����̍ő�l
		float ajast_CM_Min;//���_�����̍ŏ��l
		float ajast_CM_Std;//���_�����̊�_
		float fov; //����p
		float fov_min; //����p�̍ŏ��l
		float fov_max; //����p�̍ő�l
		float fov_std; //����p�̊�{�l
	};

	struct object
	{
		state object_state;
		ML::Vec3 pos;
		ML::Box3D hitBase;
		ML::Vec3 angle;
		float fallspeed;//�G�I�u�W�F�N�g���󂩂痎���Ă��鑬�x

	};

	void Map_Initialize(Map3D& m_);
	bool Map_Load(Map3D& m_, string f_);
	void Map_Render(Map3D& m_);
	void Map_Finalize(Map3D& m_);
	bool Map_Check_Hit(Map3D& m_, const ML::Box3D& hit);
	
	void Chara_Initialize(Chara&c_);
	void Chara_CheckMove(Chara& c_, ML::Vec3& est_);
	bool Chara_Check_Foot(Chara& c_);
	void Player_Initialize(Chara&c_, ML::Vec3& p_);
	void Player_Render(Chara&c_);
	void Player_UpDate(Chara&c_);
	void Player_Option_UpDate(std::list<ML::Vec3>& l);
	void Player_Option_Render(std::list<ML::Vec3>& l);

	void Sky_Initialize(Chara& c_, ML::Vec3& p_);
	void Sky_UpDate(Chara& c_);
	void Sky_Render(Chara& c_);

	void Cloud_Initialize(Chara& c_, ML::Vec3& p_);
	void Cloud_Update(Chara& c_);
	void Cloud_Render(Chara& c_);

	void Object_Initialize(object& o_);
	bool Object_Check_Foot(object& c_);
	
	void Item_Initialize(object& o_, ML::Vec3 p_);
	void Item_UpDate(object& o_);
	void Item_Render(object& o_);

	void Enemy_Initialize(object& o_, ML::Vec3 p_);
	void Enemy_UpDate(object& o_);
	void Enemy_Render(object& o_);

	//�Q�[�����
	DI::VGamePad  in1;
	Map3D MapData;
	Chara player;	
	Chara sky;
	Chara cloud;
	object Item[30];
	object Enemy[20*20];

	int g_time;

	std::list<ML::Vec3> Optionlist;//�����ۂ̃��X�g
	//-----------------------------------------------------------------------------
	//����������
	//�@�\�T�v�F�v���O�����N�����ɂP����s�����i�f�ނȂǂ̏������s���j
	//-----------------------------------------------------------------------------
	void Initialize()
	{
		srand(unsigned int(time));
		ML::Mat4x4 matProj;		
		//�J�����̐ݒ�
		ge->camera[0] = MyPG::Camera::Create(
			ML::Vec3(1000.0f, 0.0f, 1000.0f),				//	�^�[�Q�b�g�ʒu
			ML::Vec3(1000.0f, 1400.0f, -1000.0f),			//	�J�����ʒu
			ML::Vec3(0.0f, 1.0f, 0.0f),					//	�J�����̏�����x�N�g��
			ML::ToRadian(35), 10.0f, 4000.0f,	//	����p�E���싗��
			(float)ge->screenWidth / (float)ge->screenHeight);		//	��ʔ䗦		
		DG::EffectState().param.bgColor = ML::Color(1, 1, 1, 1);

		//���b�V���̓ǂݍ���
		DG::Mesh_CreateFromSOBFile("ArrowMesh", "./data/mesh/arrow.sob");
		DG::Mesh_CreateFromSOBFile("PlayerMesh", "./data/mesh/char_Stand.SOB");
		//DG::Mesh_UpDateMaterial("PlayerMesh",
		//	ML::Color(1,1,0,0), //d
		//	ML::Color(1, 0,1,0), //s
		//	ML::Color(1, 0, 0, 0), //a
		//	ML::Color(1,0,0,0));//e
		DG::Mesh_CreateFromSOBFile("SkyMesh", "./data/mesh/sky_data_blue_xfile.sob");
		DG::Mesh_CreateFromSOBFile("CloudMesh", "./data/mesh/sky_data_cloud_xfile.sob");

		//�t�H���g�̒ǉ�
		DG::Font_Create("FontA", "MS �S�V�b�N", 10, 20);


		//�}�b�v�̐���
		Map_Initialize(MapData);
		Map_Load(MapData, "./data/StageData/Map00.txt");

		Player_Initialize(player, ML::Vec3(150, 100, 150));
		
		Sky_Initialize(sky,ML::Vec3(0,0,0));
		Cloud_Initialize(cloud,ML::Vec3(0,0,0));

		//�I�u�W�F�N�g�S�̃[���N���A
		for (int i = 0; i < _countof(Item); i++)
		{
			Object_Initialize(Item[i]);
		}
		for (int i = 0; i < _countof(Enemy); i++)
		{
			Object_Initialize(Enemy[i]);
		}
		g_time = 0;

		//���C�e�B���O�L����
		DG::EffectState().param.lightsEnable = true;
		//�����̋�����ݒ肷��
		DG::EffectState().param.lightAmbient = ML::Color(1, 0.3f,0.3f,0.3f);
		//���s�����̐ݒ�
		DG::EffectState().param.light[0].enable = true;
		DG::EffectState().param.light[0].kind = DG_::Light::Directional;//�����̎��
		DG::EffectState().param.light[0].direction = ML::Vec3(1, -1, 1).Normalize();//�Ǝ˕���
		DG::EffectState().param.light[0].color = ML::Color(1, 1,1,1);//�F�Ƌ���


		//�r���[�|�[�g����m�F�p���̐ݒ�i��قǍ폜�̗\��j
		D3D10_VIEWPORT vp;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		vp.Width = 480;
		vp.Height = 540;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		DG::EffectState().ViewPort(vp);
	}
	//-----------------------------------------------------------------------------
	//�������
	//�@�\�T�v�F�v���O�����I�����ɂP����s�����i�f�ނȂǂ̉�����s���j
	//-----------------------------------------------------------------------------
	void Finalize()
	{
		DG::Mesh_Erase("ArrowMesh");
		DG::Mesh_Erase("PlayerMesh");
		DG::Mesh_Erase(cloud.meshName);
		DG::Mesh_Erase(sky.meshName);
		Map_Finalize(MapData);
		Optionlist.clear();
	}
	//-----------------------------------------------------------------------------
	//���s����
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ����鏈��
	//-----------------------------------------------------------------------------
	TaskFlag UpDate()
	{
		g_time++;
		in1 = DI::GPad_GetState("P1");
		//CameraMan_Update(CameraMan);

		if (in1.B1.down)
		{
			system("pause");
		}

		Player_UpDate(player);
		Player_Option_UpDate(Optionlist);
		//1�b�̂��тɃA�C�e������
		if (g_time % 120 == 0)
		{
			int ranX, ranZ;
			ranX = (rand() % (MapData.sizeX - 2)) + 1;
			ranZ = (rand() % (MapData.sizeZ - 2)) + 1;
			for (int i = 0; i < _countof(Item); i++)
			{
				if (Item[i].object_state == Normal)
				{
					continue;
				}
				Item_Initialize(Item[i], ML::Vec3(ranX * 100, 100, ranZ * 100));
				break;
			}
		}
		else if (g_time % 300 == 0)
		{
			int ranX, ranZ;
			ranX = (rand() % (MapData.sizeX - 2)) + 1;
			ranZ = (rand() % (MapData.sizeZ - 2)) + 1;
			for (int i = 0; i < _countof(Enemy); i++)
			{
				if (Enemy[i].object_state == Normal)
				{
					continue;
				}
				Enemy_Initialize(Enemy[i], ML::Vec3(ranX * 100, 500, ranZ * 100));
				break;
			}
		}

		//Sky_UpDate(sky);
		//Cloud_Update(cloud);
		
		for(int e =0; e<_countof(Enemy);e++)
		{
			Enemy_UpDate(Enemy[e]);
		}
		//�A�C�e���s��
		for (int d = 0; d < _countof(Item); d++)
		{
			Item_UpDate(Item[d]);
		}
		


		TaskFlag rtv = Task_Game;//��肠�������݂̃^�X�N���w��
		if (true == in1.ST.down) {
			rtv = Task_Title;	//���̃^�X�N���^�C�g����
		}

		return rtv;
	}
	//-----------------------------------------------------------------------------
	//�`�揈��
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ�����\������	2D
	//-----------------------------------------------------------------------------
	void Render2D()
	{
		auto p = &player;		
		char buf[1024];
		sprintf_s(buf,
			"�v���C���@���W(%6.2f,%6.2f,%6.2f) ���� : %6.2f���@\n"
			"������ %d",
			p->pos.x, p->pos.y, p->pos.z, ML::ToDegree(p->angle.y)
			, p->tailsize
			);
		ML::Box2D draw(300, 0, 600, 200);
		DG::Font_Draw("FontA", draw, buf, ML::Color(1, 1, 0, 1));
	}
	//-----------------------------------------------------------------------------
	//�`�揈��
	//�@�\�T�v�F�Q�[���̂P�t���[���ɓ�����\������	3D
	//-----------------------------------------------------------------------------
	void Render3D()
	{
		//���s�ړ��s��𐶐� //���[���h�ϊ���K�p����
		ML::Mat4x4  matT;
		matT.Translation(ML::Vec3(0, 0, 0));	
		Sky_Render(sky);
		Cloud_Render(cloud);
		DG::EffectState().param.matWorld = matT;
		DG::Mesh_Draw("ArrowMesh");
		
		Map_Render(MapData);
		Player_Render(player);
		Player_Option_Render(Optionlist);

		//�A�C�e���\��
		for (int i = 0; i < _countof(Item); i++)
		{
			Item_Render(Item[i]);
		}
		//�G�̕\��
		for (int i = 0; i < _countof(Enemy); i++)
		{
			Enemy_Render(Enemy[i]);
		}
	}
	//-------------------------------------------------------------------------------------------
	//�}�b�v�Ɋւ���֐�
	//�}�b�v�̏�����
	void Map_Initialize(Map3D& m_)
	{
		ZeroMemory(m_.arr, sizeof(m_.arr));
		m_.sizeX = 0;
		m_.sizeY = 0;
		m_.sizeZ = 0;
		m_.hitBase = ML::Box3D(0, 0, 0, 0, 0, 0);
		m_.chipSize = 100.0f;
		for (int i = 0; i < _countof(m_.chipName); i++)
		{
			m_.chipName[i] = "";
		}
	}
	//�}�b�v�̓ǂݍ���
	bool Map_Load(Map3D& m_, string f_)
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
			m_.chipName[c] = "MapChip" + std::to_string(c);
			DG::Mesh_CreateFromSOBFile(m_.chipName[c], chipFilePath);
		}
		//�}�b�v�z��T�C�Y�̓ǂݍ���
		fin >> m_.sizeX >> m_.sizeY >> m_.sizeZ;
		m_.hitBase = ML::Box3D(0, 0, 0,
			m_.sizeX*(int)m_.chipSize,
			m_.sizeY*(int)m_.chipSize,
			m_.sizeZ*(int)m_.chipSize);
		//�}�b�v�z��f�[�^�̓ǂ݂�����
		for (int y = 0; y < m_.sizeY; y++)
		{
			for (int z = m_.sizeZ - 1; z >= 0; z--)
			{
				for (int x = 0; x < m_.sizeX; x++)
				{
					fin >> m_.arr[z][y][x];
				}
			}
		}
		fin.close();
		return true;
	}
	//�}�b�v�̕\��
	void Map_Render(Map3D& m_)
	{
		ML::Mat4x4 matS;
		matS.Scaling(m_.chipSize / 100.0f);
		for (int y = 0; y < m_.sizeY; y++)
		{
			for (int z = 0; z < m_.sizeZ; z++)
			{
				for (int x = 0; x < m_.sizeX; x++)
				{
					int cn = m_.arr[z][y][x];
					if (cn == 0)
					{
						continue;
					}

					ML::Mat4x4 matT;
					matT.Translation(ML::Vec3(x*m_.chipSize + m_.chipSize / 2, y*m_.chipSize + m_.chipSize / 2, z*m_.chipSize + m_.chipSize / 2));
					ML::Mat4x4 matW;
					matW = matS * matT;
					DG::EffectState().param.matWorld = matW;
					DG::Mesh_Draw(m_.chipName[cn]);
				}
			}
		}
	}
	//�}�b�v�̊J��
	void Map_Finalize(Map3D& m_)
	{
		for (int i = 0; i < _countof(m_.chipName); i++)
		{
			if (m_.chipName[i] != "")
			{
				DG::Mesh_Erase(m_.chipName[i]);
			}
		}
		m_.sizeX = 0;
		m_.sizeY = 0;
		m_.sizeZ = 0;
	}
	//�}�b�v�Ƃ̓����蔻��
	bool Map_Check_Hit(Map3D& m_, const ML::Box3D& hit)
	{
		struct Box3D_2point
		{
			int fx, fy, fz;//�l�����������̓_
			int bx, by, bz;//�l���傫�����̓_
		};

		Box3D_2point r =
		{
			hit.x , hit.y , hit.z,
			hit.x + hit.w , hit.y + hit.h , hit.z + hit.d
		};

		Box3D_2point m =
		{
			m_.hitBase.x , m_.hitBase.y, m_.hitBase.z,
			m_.hitBase.x + m_.hitBase.w , m_.hitBase.y + m_.hitBase.h, m_.hitBase.z + m_.hitBase.d
		};

		//�L�����N�^�̋�`���}�b�v�͈͓̔��Ɋۂߍ���
		if (r.fx < m.fx) { r.fx = m.fx; }
		if (r.fy < m.fy) { r.fy = m.fy; }
		if (r.fz < m.fz) { r.fz = m.fz; }
		if (r.bx > m.bx) { r.bx = m.bx; }
		if (r.by > m.by) { r.by = m.by; }
		if (r.bz > m.bz) { r.bz = m.bz; }

		//�L�����N�^���}�b�v�̊O�Ɋ��S�ɏo�Ă����画��I��
		if (r.bx <= r.fx) {	return false;}
		if (r.by <= r.fy) { return false; }
		if (r.bz <= r.fz) { return false; }

		//���[�v�͈͐ݒ�
		int sx, sy, sz, ex, ey, ez;
		sx = r.fx / (int)m_.chipSize;
		sy = r.fy / (int)m_.chipSize;
		sz = r.fz / (int)m_.chipSize;
		ex = (r.bx - 1) / (int)m_.chipSize;
		ey = (r.bx - 1) / (int)m_.chipSize;
		ez = (r.bz - 1) / (int)m_.chipSize;

		//�ڐG����
		for (int z = sz; z <= ez; z++)
		{
			for (int y = sy; y <= ey; y++)
			{
				for (int x = sx; x <= ex; x++)
				{
					if (m_.arr[z][y][x] > 0)
					{
						return true;
					}
				}
			}
		}
		return false;//�ڐG���Ă�����̂��Ȃ�
	}

	//-------------------------------------------------------------------------------------------------
	//�L�����N�^�[����	
	//�L�����N�^�[�̃[���N���A
	void Chara_Initialize(Chara &c_)
	{
		c_.meshName = "";
		c_.hitBase = ML::Box3D(0, 0, 0, 0, 0, 0);
		c_.pos = ML::Vec3(0, 0, 0);
		c_.angle = ML::Vec3(0, 0, 0);
		c_.fallspeed = 0.0f;
		c_.jumppow = 0.0f;
	}
	//�߂荞�܂Ȃ��ړ�
	void Chara_CheckMove(Chara& c_, ML::Vec3& est_)
	{
		float temp, temp2;
		//XZ���ʂɑ΂���ړ�
		while (est_.x != 0.0f)//X�ړ�
		{
			float preX = c_.pos.x;//�ړ��O�̈ʒu���m��
			if (est_.x >= 1.0f)
			{
				c_.pos.x += 1.0f;
				est_.x -= 1.0f;
			}
			else if (est_.x <= -1.0f)
			{
				c_.pos.x -= 1.0f;
				est_.x += 1.0f;
			}
			else
			{
				c_.pos.x += est_.x;
				est_.x = 0.0f;
			}

			//�ڐG����������Ă݂�
			ML::Box3D hit = c_.hitBase;
			hit.Offset((int)c_.pos.x, (int)c_.pos.y, (int)c_.pos.z);
			if (Map_Check_Hit(MapData, hit))
			{
				c_.pos.x = preX;
				break;
			}
		}
		
		while (est_.z != 0.0f)//Z�ړ�
		{
			float preZ = c_.pos.z;

			if (est_.z >= 1.0f)
			{
				c_.pos.z += 1.0f;
				est_.z -= 1.0f;
			}
			else if (est_.z <= -1.0f)
			{
				c_.pos.z -= 1.0f;
				est_.z += 1.0f;
			}
			else
			{
				c_.pos.z += est_.z;
				est_.z = 0.0f;
			}
			//�ڐG����������Ă݂�
			ML::Box3D hit = c_.hitBase;
			hit.Offset((int)c_.pos.x, (int)c_.pos.y, (int)c_.pos.z);
			if (Map_Check_Hit(MapData, hit))
			{
				c_.pos.z = preZ;
				break;
			}
		}

		//�c���ɑ΂���ړ�
		while (est_.y != 0.0f)
		{
			float preY = c_.pos.y;
			if (est_.y >= 1.0f)
			{
				c_.pos.y += 1.0f;
				est_.y -= 1.0f;
			}
			else if (est_.y <= -1.0f)
			{
				c_.pos.y -= 1.0f;
				est_.y += 1.0f;
			}
			else
			{
				c_.pos.y += est_.y;
				est_.y = 0.0f;
			}
			//�ڐG����������Ă݂�
			ML::Box3D hit = c_.hitBase;
			hit.Offset((int)c_.pos.x, (int)c_.pos.y, (int)c_.pos.z);
			if (Map_Check_Hit(MapData, hit))
			{
				c_.pos.y = preY;
				break;
			}
		}
	}
	//�����̔���
	bool Chara_Check_Foot(Chara& c_)
	{
		//����p��BOX�����
		ML::Box3D hit(c_.hitBase.x, c_.hitBase.y - 1, c_.hitBase.z, c_.hitBase.w, 1, c_.hitBase.d);

		hit.Offset(c_.pos);
		return Map_Check_Hit(MapData, hit);
	}
	//----------------------------------------------------------------------------------------------------
	//�v���C���[�̏�����
	void Player_Initialize(Chara&c_, ML::Vec3& p_)
	{
		Chara_Initialize(c_);
		c_.player_state = Normal;
		c_.pos = p_;
		c_.speed = 6;
		c_.meshName = "PlayerMesh";
		c_.hitBase = ML::Box3D(-40, 0, -40, 80, 160, 80);
		c_.jumppow = 20.0f;
		c_.tailsize = 1;
		c_.playerAngle = Right;
	}
	//�v���C���[�̕\��
	void Player_Render(Chara&c_)
	{
		if (player.player_state != Normal)
		{
			return;
		}
		ML::Mat4x4 matT;
		matT.Translation(c_.pos);
		//��]�s��
		ML::Mat4x4 matR;
		matR.RotationY(c_.angle.y);
		//���f���̕\��
		ML::Mat4x4 matW = matR*matT;
		DG::EffectState().param.matWorld = matW;
		DG::Mesh_Draw(c_.meshName);
	}
	//�v���C���[�̍s��
	void Player_UpDate(Chara&c_)
	{
		if (player.player_state != Normal)
		{
			return;
		}
		ML::Vec3 est(0, 0, 0);

		switch (c_.playerAngle)
		{
		case Up:
		case Down:
			if (in1.LStick.L.on)
			{
				c_.playerAngle = Left;
				c_.angle.y = ML::ToRadian((float)Left);
			}
			if (in1.LStick.R.on)
			{
				c_.playerAngle = Right;
				c_.angle.y = ML::ToRadian((float)Right);
			}
			break;
		case Left:
		case Right:
			if (in1.LStick.U.on)
			{
				c_.playerAngle = Up;
				c_.angle.y = ML::ToRadian((float)Up);
			}
			if (in1.LStick.D.on)
			{
				c_.playerAngle = Down;
				c_.angle.y = ML::ToRadian((float)Down);
			}
			break;
		}
		
		

		//if (in1.LStick.volume > 0)
		//{
		//	//�A�i���O���͂̏�(270��)���O���ɂȂ�悤��
		//	c_.angle.y += in1.LStick.axis.x * ML::ToRadian(5);
		//	//c_.angle.y += CameraMan.angle.y;			
		//}
		ML::Mat4x4 matR;
		matR.RotationY(c_.angle.y);
		est.x += c_.speed;// *in1.LStick.volume;
		est = matR.TransformCoord(est);
		//������`���l�ʂƐڐG���Ă���Ƃ��̂݃W�����v�\
		if (Chara_Check_Foot(c_))
		{
			if (in1.B2.down)
			{
				c_.fallspeed = c_.jumppow;
			}
		}
		else
		{
			c_.fallspeed -= ML::Gravity(100) * 4;
		}
		est.y = c_.fallspeed;
		Chara_CheckMove(c_, est);
	}
	//�����ۂ̍s��
	void Player_Option_UpDate(std::list<ML::Vec3>& l)
	{
		if (player.player_state != Normal)
		{
			return;
		}
		//1�t���[�����ɏꏊ��]������
		l.push_back(player.pos);

		if (l.size() > player.tailsize * 10)
		{
			l.pop_front();
		}

		//�v���C���Ƃ̐ڐG����
		int cnt = 0;
		for (auto li = l.end(); li != l.begin(); li--)
		{
			if (cnt % 10 == 0 && cnt != 0 && cnt > 20)
			{
				ML::Box3D me(-25, 0, -25, 50, 150, 50);
				me.Offset((*li));
				ML::Box3D you = player.hitBase.OffsetCopy(player.pos);
				if (you.Hit(me))
				{
					player.player_state = Dead;
					Optionlist.clear();
				}
			}
			cnt++;
		}

	}
	//�����ۂ̕\��
	void Player_Option_Render(std::list<ML::Vec3>& l)
	{
		if (player.player_state != Normal)
		{
			return;
		}
		int cnt = 0;

		for (auto li = l.end(); li != l.begin(); --li)
		{
			if (cnt % 10 == 0 && cnt != 0)
			{
				ML::Mat4x4 matT;
				matT.Translation((*li));
				ML::Mat4x4 matW = matT;

				DG::EffectState().param.matWorld = matW;

				DG::EffectState().param.objectColor = ML::Color(1, 0.2f, 0.1f, 0.8f);
				DG::Mesh_Draw(player.meshName);//���̕\��
				DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
			}
			cnt++;
		}
	}
	//--------------------------------------------------------------------------------------------------
	//��
	//��̏�����

	void Sky_Initialize(Chara& c_, ML::Vec3& p_)
	{
		Chara_Initialize(c_);
		c_.meshName = "SkyMesh";
	}

	//��̍s��
	void Sky_UpDate(Chara& c_)
	{
		c_.angle.y += ML::ToRadian(0.01f);		
	}

	//��̕\��
	void Sky_Render(Chara& c_)
	{
		ML::Mat4x4 matT;
		matT.Translation(c_.pos);
		//��]�s��̐���
		ML::Mat4x4 matR_Sky;
		matR_Sky.RotationY(c_.angle.y);
		//�J�����̐ݒ��ύX
		ge->camera[0]->forePlane = 80000.0f;
		ge->camera[0] -> UpDate();
		//�[�x�o�b�t�@�𖳌���
		DG::EffectState().DSS_Def3D_Dis();
		//���C�e�B���O���� �_�̕��Ō��ɖ߂��\��
		DG::EffectState().param.light[0].enable = false;

		//���f���̕\��
		ML::Mat4x4 matW = matR_Sky * matT;
		DG::EffectState().param.matWorld = matW;
		DG::EffectState().param.objectColor = ML::Color(1, 0.2f, 0.5f, 1);
		DG::Mesh_Draw(c_.meshName);
		DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
	}

	//----------------------------------------------------------------------------------------------------
	//�_
	//�_�̏�����
	void Cloud_Initialize(Chara& c_, ML::Vec3& p_)
	{
		Chara_Initialize(c_);
		c_.meshName = "CloudMesh";
	}
	//�_�̍s��
	void Cloud_Update(Chara& c_)
	{
		c_.angle.y += ML::ToRadian(0.01f);		
	}
	//�_�̕\��
	void Cloud_Render(Chara& c_)
	{
		ML::Mat4x4 matT;
		matT.Translation(c_.pos);
		//��]�s��̐���
		ML::Mat4x4 matR_Cloud;
		matR_Cloud.RotationY(c_.angle.y);		

		//���f���̕\��
		ML::Mat4x4 matW = matR_Cloud * matT;
		DG::EffectState().param.matWorld = matW;
		//�o�̓}�[�W���𔼓����ɐݒ�
		auto bs_bup = DG::EffectState().BS_Get();
		DG::EffectState().BS_Alpha();
		DG::Mesh_Draw(c_.meshName);
		DG::EffectState().BS_Set(bs_bup);
		//�[�x�o�b�t�@�𖳌���
		DG::EffectState().DSS_Def3D();
		//�J�����̐ݒ�
		ge->camera[0]->forePlane = 4000.0f;
		ge->camera[0]->UpDate();

		//���C�e�B���O�L�� ��Ŗ����ɂ������̂����ɖ߂�
		DG::EffectState().param.light[0].enable = true;
		
	}

	//-------------------------------------------------------------------------------------------------------
	//�I�u�W�F�N�g����
	//�I�u�W�F�N�g�̃[���N���A
	void Object_Initialize(object& o_)
	{
		o_.pos = ML::Vec3(0, 0, 0);
		o_.hitBase = ML::Box3D(0, 0, 0, 0, 0, 0);
		o_.angle = ML::Vec3(0, 0, 0);
		o_.fallspeed = 0;
		o_.object_state = Non;
	}
	bool Object_Check_Foot(object& c_)
	{
		//����p��BOX�����
		ML::Box3D hit(c_.hitBase.x, c_.hitBase.y - 1, c_.hitBase.z, c_.hitBase.w, 1, c_.hitBase.d);

		hit.Offset(c_.pos);
		return Map_Check_Hit(MapData, hit);
	}
	//------------------------------------------------------------------------------------------------------
	//�A�C�e��
	//�A�C�e���̏�����
	void Item_Initialize(object& o_, ML::Vec3 p_)
	{
		Object_Initialize(o_);
		o_.object_state = Normal;
		o_.pos = p_;
		o_.hitBase = ML::Box3D(0, 0, 0, 100, 100, 100);		
	}
	void Item_UpDate(object& o_)
	{
		if (o_.object_state != Normal)
		{
			return;
		}
		//�v���C���Ƃ̂����蔻��
		ML::Box3D you = player.hitBase.OffsetCopy(player.pos);
		ML::Box3D me = o_.hitBase.OffsetCopy(o_.pos);

		if (you.Hit(me))
		{
			player.tailsize++;
			o_.object_state = Dead;
			
			player.speed += 0.3f;
			
		}
	}
	void Item_Render(object& o_)
	{
		if (o_.object_state != Normal)
		{
			return;
		}
		ML::Mat4x4 matT;
		matT.Translation(o_.pos);

		ML::Mat4x4 matW = matT;

		DG::EffectState().param.matWorld = matW;
		DG::Mesh_Draw(MapData.chipName[2]);//���̕\��
	}
	//-----------------------------------------------------------------------------------------------------------
	//�G
	//�G�̏�����
	void Enemy_Initialize(object& o_, ML::Vec3 p_)
	{
		Object_Initialize(o_);
		o_.object_state = Normal;
		o_.hitBase = ML::Box3D(-50, -50, -50, 100, 100, 100);
		o_.pos = p_;
		o_.fallspeed = 20.0f;
	}
	//�G�̓���
	void Enemy_UpDate(object& o_)
	{
		if (o_.object_state != Normal)
		{
			return;
		}
		//�n��ɂ��܂ŗ�����
		if (!Object_Check_Foot(o_))
		{
			o_.pos.y -= o_.fallspeed;
		}
		//�v���C���Ƃ̂����蔻��
		ML::Box3D you = player.hitBase.OffsetCopy(player.pos);
		ML::Box3D me = o_.hitBase.OffsetCopy(o_.pos);
		if (you.Hit(me))
		{
			//�Q�[���I�[�o�[����
			player.player_state = Dead;
			Optionlist.clear();
		}
	}
	//�G�̕`��
	void Enemy_Render(object& o_)
	{
		ML::Mat4x4 matT;
		matT.Translation(o_.pos);

		ML::Mat4x4 matW = matT;

		DG::EffectState().param.matWorld = matW;
		DG::EffectState().param.objectColor = ML::Color(1, 1, 0, 0);
		DG::Mesh_Draw(MapData.chipName[1]);
		DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
	}
}