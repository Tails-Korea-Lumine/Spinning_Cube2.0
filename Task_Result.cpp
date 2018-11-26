//-------------------------------------------------------------------
//�^�C�g�����
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Result.h"
#include  "Task_UI.h"
#include  "easing.h"

namespace  Result
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//���\�[�X�̏�����
	bool  Resource::Initialize()
	{
		this->thisTime = "thisTime";
		this->yourScore = "yourScore";
		this->result = "result";
		this->Number_Image = "resultNumber";
		this->presS = "pressS";

		this->bgmName = "ResultBgmName";

		DG::Image_Create(this->thisTime, "./data/image/thisTime.png");
		DG::Image_Create(this->yourScore, "./data/image/yourScore.png");
		DG::Image_Create(this->result, "./data/image/result.png");
		DG::Image_Create(this->Number_Image, "./data/image/Number.png");
		DG::Image_Create(this->presS, "./data/image/pressS.png");

		DM::Sound_CreateStream(this->bgmName, "./data/sound/ending.wav");

		return true;
	}
	//-------------------------------------------------------------------
	//���\�[�X�̉��
	bool  Resource::Finalize()
	{
		DG::Image_Erase(this->thisTime);
		DG::Image_Erase(this->yourScore);
		DG::Image_Erase(this->result);
		DG::Image_Erase(this->Number_Image);
		DG::Image_Erase(this->presS);

		DM::Sound_Erase(this->bgmName);
		return true;
	}
	//-------------------------------------------------------------------
	//�u�������v�^�X�N�������ɂP�񂾂��s������
	bool  Object::Initialize(int playTime, Difficult_Range di)
	{
		//�X�[�p�[�N���X������
		__super::Initialize(defGroupName, defName, true);
		//���\�[�X�N���X����or���\�[�X���L
		this->res = Resource::Create();

		//���f�[�^������
		this->countdown = 0;
		this->countdownFlag = false;		
		//bgm�Đ�
		DM::Sound_Play(this->res->bgmName, true);

		//�_���v�Z�� : ��Փx�ʂ̊�_��-�v���[����(�b�P��)
		switch (di)
		{
		case Difficult_Range::Easy:
			this->score = 300 - (playTime / 60);
			break;
		case Difficult_Range::Normal:
			this->score = 600 - (playTime / 60);
			break;
		case Difficult_Range::Hard:
			this->score = 1000 - (playTime / 60);
			break;
		}
		this->timeCnt = 0;
		//�����\�̏�񏉊���
		for (int i = 0; i < 10; i++)
		{
			this->src_Number[i].x = (140 * i);
			this->src_Number[i].y = 0;
			this->src_Number[i].w = 140;
			this->src_Number[i].h = 140;
		}

		this->vol = 1000;
		
		DG::EffectState().param.bgColor = ML::Color(1, 0, 0, 0);
		//���^�X�N�̐���
		//Wipe In&Out�̂��߂�UI�^�X�N�𐶐�
		UI::Object::Create(true);

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�I���v�^�X�N���Ŏ��ɂP�񂾂��s������
	bool  Object::Finalize()
	{
		//���f�[�^���^�X�N���
		ge->KillAll_G("UI");

		DM::Sound_Stop(this->res->bgmName);

		if (!ge->QuitFlag() && this->nextTaskCreate)
		{
			//�������p���^�X�N�̐���
			auto NextTask = Title::Object::Create(true);
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//�u�X�V�v�P�t���[�����ɍs������
	void  Object::UpDate()
	{
		auto in = DI::GPad_GetState("P1");
		easing::UpDate();
		//�J�E���g�_�E���t���O�𗧂Ă�
		if (in.ST.down)
		{
			this->countdownFlag = true;
			ge->GetTask_One_G<UI::Object>("UI")->Start_WipeIn();
			//�����I�ɉ�ʏ�񂪑S���\�����鎞�Ԃɂ���
			this->timeCnt = 300;
		}
		//�t���O�������Ă���ԂɃJ�E���g�_�E������
		if (this->Is_Count_Down())
		{
			this->countdown++;
			this->BGM_Fade_Out();
		}
		//1�b��Ƀ^�X�N����
		if (this->Is_Count_Down_Over())
		{
			this->Kill();
		}
		

		this->timeCnt++;
	}
	//-------------------------------------------------------------------
	//�u�Q�c�`��v�P�t���[�����ɍs������
	void  Object::Render2D_AF()
	{
		//result
		ML::Box2D draw_Result(30, 30, 400, 100);
		//�摜�S�̃T�C�Y
		POINT size_Result = DG::Image_Size(this->res->result);
		ML::Box2D src_Result(0, 0, size_Result.x, size_Result.y);
		//�`��
		DG::Image_Draw(this->res->result, draw_Result, src_Result);
		//this time
		if (this->Is_Over_Argument_Seconds(1))
		{
			ML::Box2D draw_ThisTime((ge->screenWidth/2), ge->screenHeight/4, 700, 140);
			//�摜�S�̃T�C�Y
			POINT size_ThisTime = DG::Image_Size(this->res->thisTime);
			ML::Box2D src_ThisTime(0, 0, size_ThisTime.x, size_ThisTime.y);
			//�`��
			DG::Image_Draw(this->res->thisTime, draw_ThisTime, src_ThisTime);
		}
		//your score
		if (this->Is_Over_Argument_Seconds(2))
		{
			ML::Box2D draw_YourScore((ge->screenWidth / 2), ge->screenHeight / 2, 770, 140);
			//�摜�S�̃T�C�Y
			POINT size_YourScore = DG::Image_Size(this->res->yourScore);
			ML::Box2D src_YourScore(0, 0, size_YourScore.x, size_YourScore.y);
			//�`��
			DG::Image_Draw(this->res->yourScore, draw_YourScore, src_YourScore);
		}
		//�X�R�A�[�\��
		if (this->Is_Over_Argument_Seconds(3))
		{
			this->Draw_Score();
		}
		//press S to return title
		if (this->Is_Over_Argument_Seconds(4))
		{
			ML::Box2D draw_pressS((ge->screenWidth / 3), ge->screenHeight-100, 700, 70);
			//�摜�S�̃T�C�Y
			POINT size_PressS = DG::Image_Size(this->res->presS);
			ML::Box2D src_pressS(0, 0, size_PressS.x, size_PressS.y);
			//�`��
			DG::Image_Draw(this->res->presS, draw_pressS, src_pressS);
		}

	}

	void  Object::Render3D_L0()
	{
		
	}

	//------------------------------------------------------------------------------------
	//�ǉ����\�b�h
	void Object::Draw_Score() const
	{
		//������̃T�C�Y
		int numSize = 140;

		//�_���̈ʒu�ʂɏ�����
		int score1000 = 0;
		int score100 = 0;
		int score10 = 0;
		int score1 = 0;
		//�_�����}�C�i�X�Ȃ�O�_�ŕ\������
		if (this->score > 0)
		{
			//0�ȏ�Ȃ�_���̗v�f�𕪉����đ��
			score1000 = this->score / 1000;
			score100 = (this->score / 100) % 10;
			score10 = (this->score / 10) % 10;
			score1 = this->score % 10;
		}

		//�\���͈�
		ML::Box2D draw1000((ge->screenWidth/3), (ge->screenHeight/3)*2, numSize, numSize);
		ML::Box2D draw100((ge->screenWidth / 3) + (numSize * 1), (ge->screenHeight / 3) * 2, numSize, numSize);
		ML::Box2D draw10((ge->screenWidth / 3) + (numSize * 2), (ge->screenHeight / 3) * 2, numSize, numSize);
		ML::Box2D draw1((ge->screenWidth / 3) + (numSize * 3), (ge->screenHeight / 3) * 2, numSize, numSize);

		//�`��
		DG::Image_Draw(this->res->Number_Image, draw1000, this->src_Number[score1000]);
		DG::Image_Draw(this->res->Number_Image, draw100, this->src_Number[score100]);
		DG::Image_Draw(this->res->Number_Image, draw10, this->src_Number[score10]);
		DG::Image_Draw(this->res->Number_Image, draw1, this->src_Number[score1]);
	}

	//----------------------------------------------------------------------------------
	//�J�E���^�_�E���t���O��Ԃ��֐�
	bool Object::Is_Count_Down() const
	{
		return this->countdownFlag;
	}
	//----------------------------------------------------------------
	//�J�E���g�_�E�����I������̂����m�F
	bool Object::Is_Count_Down_Over() const
	{
		//�J�E���g�_�E����1�b
		return (this->countdown > 60);
	}

	//-----------------------------------------------------------------------
	//�������炢�̎��Ԃ��������̂����m�F����
	bool Object::Is_Over_Argument_Seconds(const int sec) const
	{
		return (this->timeCnt > (60 * sec));
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
	Object::SP  Object::Create(bool  flagGameEnginePushBack_, int playTime, Difficult_Range di)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//�Q�[���G���W���ɓo�^
			}
			if (!ob->B_Initialize(playTime,di)) {
				ob->Kill();//�C�j�V�����C�Y�Ɏ��s������Kill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize(int playTime, Difficult_Range di)
	{
		return  this->Initialize(playTime,di);
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