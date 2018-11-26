//---------------------------------------------------------------------------------
//�G�t�F�N�g�N���X
//---------------------------------------------------------------------------------

#include "Effect.h"
#include "easing.h"
#include "GameEngine_Ver3_7.h"

void Effect::Load_Eff(const ML::Vec3& pos, const ML::Vec3& angle)
{
	this->Eff_Initialize(pos, angle);	
}

void Effect::Load_Eff(const ML::Vec3& pos, const ML::Vec3& target, const ML::Vec3& angle)
{
	this->Eff_Initialize(pos, target, angle);
}

void Effect::Playing_Effect()
{
	//�����ɍ����֐����Ă�

	switch (this->playing_EffectHandle)
	{
	case effType::CreateCharactor:
		this->Playing_EF0();
		break;
	case effType::TeleportOut:
		this->Playing_EF1();
		break;
	case effType::Teleportin:
		this->Playing_EF2();
		break;
	case effType::EnemyLanding:
		this->Playing_EF3();
		break;
	case effType::Hit_to_Enemy:
		this->Playing_EF4();
		break;
	case effType::Hit_to_Wall:
		this->Playing_EF5();
		break;
	case effType::CreateTail:
		this->Playing_EF6();
		break;
	case effType::DestroyTail:
		this->Playing_EF7();
		break;
	case effType::Hit_to_Tail:
		this->Playing_EF8();
		break;
	case effType::Game_Clear:
		this->Playing_EF9();
		break;

	default:
		return;
	}
}


void Effect::UpDate_Effect()
{
	
	//�����ɍ����֐����Ă�

	switch (this->playing_EffectHandle)
	{
	case effType::CreateCharactor:
		this->UpDate_EF0();
		break;
	case effType::TeleportOut:
		this->UpDate_EF1();
		break;
	case effType::Teleportin:
		this->UpDate_EF2();
		break;
	case effType::EnemyLanding:
		this->UpDate_EF3();
		break;
	case effType::Hit_to_Enemy:
		this->UpDate_EF4();
		break;
	case effType::Hit_to_Wall:
		this->UpDate_EF5();
		break;
	case effType::CreateTail:
		this->UpDate_EF6();
		break;
	case effType::DestroyTail:
		this->UpDate_EF7();
		break;
	case effType::Hit_to_Tail:
		this->UpDate_EF8();
		break;
	case effType::Game_Clear:
		this->UpDate_EF9();
		break;

	default:
		return;
	}
}

//------------------------------------------------------------------------------------------------------
//�`��֐�
//0�ԁF�L�����N�^����
void Effect::Playing_EF0()
{
	//if (this->effect_Life < 0)
	//{
	//	return;
	//}
	//ML::Mat4x4 matS, matR, matT;
	//matS.Scaling(this->scale);
	//matR.RotationY(this->angle.y);
	//matT.Translation(this->pos);

	//
	////�����x�ݒ�(���Ȃ̂ŉ��Z����)
	//auto bsBuf = DG::EffectState().BS_Get();
	//DG::EffectState().BS_Alpha();
	//DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	//DG::EffectState().param.matWorld = matS * matR * matT;
	//DG::Mesh_Draw(this->meshName);

	////�����x�����ɖ߂�	
	//DG::EffectState().BS_Set(bsBuf);
	//DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}
//1�ԁF�e���|�[�g�A�E�g
void Effect::Playing_EF1()
{
	if (this->effect_Life <= 0)
	{
		return;
	}
	ML::Mat4x4 matS;	
	matS.Scaling(this->scale);
	ML::Mat4x4 matT;
	matT.Translation(this->pos);

	//�����x��ݒ�
	auto bsBuf = DG::EffectState().BS_Get();
	DG::EffectState().BS_Alpha();
	//�G�t�F�N�g���������x�ŕ`��
	DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	DG::EffectState().param.matWorld = matS * matT;
	DG::Mesh_Draw(this->meshName);

	//�����x�����ɖ߂�
	DG::EffectState().BS_Set(bsBuf);
	DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}
//2�ԁF�e���|�[�g�C��
void Effect::Playing_EF2()
{
	if (this->effect_Life <= 0)
	{
		return;
	}
	ML::Mat4x4 matT,matS;
	matT.Translation(this->pos);
	matS.Scaling(this->scale);

	//�����x�ݒ�(���Ȃ̂ŉ��Z����)
	auto bsBuf = DG::EffectState().BS_Get();
	DG::EffectState().BS_AlphaAdd();
	DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	DG::EffectState().param.matWorld = matS * matT;
	DG::Mesh_Draw(this->meshName);

	//�����x�����ɖ߂�
	DG::EffectState().BS_Set(bsBuf);
	DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}
//3�ԁF�G�l�~�[���n
void Effect::Playing_EF3()
{
	//if (this->effect_Life < 0)
	//{
	//	return;
	//}
	//
	//
	//ML::Mat4x4 matT;
	//matT.Translation(this->pos);
	//ML::Mat4x4 matS;
	//matS.Scaling(this->scale);
	//

	////�����x��ݒ�
	//auto bsBuf = DG::EffectState().BS_Get();
	//DG::EffectState().BS_Alpha();
	////�G�t�F�N�g���������x�ŕ`��
	//DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	//DG::EffectState().param.matWorld = matS * matT;
	//DG::Mesh_Draw(this->meshName);

	////�����x�����ɖ߂�
	//DG::EffectState().BS_Set(bsBuf);
	//DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}
//4�ԁF�G�l�~�[�ƏՓ�
void Effect::Playing_EF4()
{
	//if (this->effect_Life < 0)
	//{
	//	return;
	//}

	//ML::Mat4x4 matT;
	//matT.Translation(this->pos);
	//ML::Mat4x4 matR;
	//matR.RotationY(-this->angle.y);
	//ML::Mat4x4 matS;
	//matS.Scaling(this->scale);

	////���C�e�B���O�𖳌��ɂ���
	//DG::EffectState().param.lightsEnable = false;
	////�����x��ݒ�
	//auto bsBuf = DG::EffectState().BS_Get();
	//DG::EffectState().BS_AlphaAdd();
	////�G�t�F�N�g���������x�ŕ`��
	//DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	//DG::EffectState().param.matWorld = matS * matR * matT;
	//DG::Mesh_Draw(this->meshName);

	////�����x,���C�e�B���O�����ɖ߂�
	//DG::EffectState().param.lightsEnable = true;
	//DG::EffectState().BS_Set(bsBuf);
	//DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}
//5�ԁF�ǂƏՓ�
void Effect::Playing_EF5()
{
	////�G�l�~�[�ƏՓ˂ɑ傫�����ω�����
	//if (this->effect_Life < 0)
	//{
	//	return;
	//}

	//ML::Mat4x4 matT;
	//matT.Translation(this->pos);
	//ML::Mat4x4 matR;
	//matR.RotationY(-this->angle.y);
	//ML::Mat4x4 matS;
	//matS.Scaling(this->scale);

	////���C�e�B���O�𖳌��ɂ���
	//DG::EffectState().param.lightsEnable = false;
	////�����x��ݒ�
	//auto bsBuf = DG::EffectState().BS_Get();
	//DG::EffectState().BS_Alpha();
	////�G�t�F�N�g���������x�ŕ`��
	//DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	//DG::EffectState().param.matWorld = matS * matR * matT;
	//DG::Mesh_Draw(this->meshName);

	////�����x,���C�e�B���O�����ɖ߂�
	//DG::EffectState().param.lightsEnable = true;
	//DG::EffectState().BS_Set(bsBuf);
	//DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
	//
}
//6�ԁF�����ې���
void Effect::Playing_EF6()
{
	//if (this->effect_Life <= 0)
	//{
	//	return;
	//}
	//ML::Mat4x4 matS;	
	//matS.Scaling(this->scale);
	//ML::Mat4x4 matT;
	//matT.Translation(this->pos);

	////�����x�ݒ�
	//auto bsBuf = DG::EffectState().BS_Get();
	//DG::EffectState().BS_AlphaAdd();
	//DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	//DG::EffectState().param.matWorld = matS*matT;
	//DG::Mesh_Draw(this->meshName);

	////�����x�����ɖ߂�
	//DG::EffectState().BS_Set(bsBuf);
	//DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}
//7�ԁF�����ۏ���
void Effect::Playing_EF7()
{
	//if (this->effect_Life < 0)
	//{
	//	return;
	//}

	//ML::Mat4x4 matS;
	//matS.Scaling(this->scale);
	//ML::Mat4x4 matT;
	//matT.Translation(this->pos);
	//ML::Mat4x4 matR;
	//matR.RotationY(this->angle.y);


	////�����x��ݒ�
	//auto bsBuf = DG::EffectState().BS_Get();
	//DG::EffectState().BS_AlphaAdd();
	////�G�t�F�N�g���������x�ŕ`��
	//DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	//DG::EffectState().param.matWorld = matS * matR * matT;
	//DG::Mesh_Draw(this->meshName);

	////�����x�����ɖ߂�
	//DG::EffectState().BS_Set(bsBuf);
	//DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);
}
//8�ԁF�����ۂƏՓ�
void Effect::Playing_EF8()
{
	//4�ԁF�G�l�~�[�ƏՓ˂ƑS������
	//this->Playing_EF4();
}
//9�ԁF�N���A�G�t�F�N�g
void Effect::Playing_EF9()
{
	if (this->effect_Life < 0)
	{
		return;
	}
	ML::Mat4x4 matS,matR, matT;
	matS.Scaling(this->scale);
	matR.RotationY(this->angle.y);
	matT.Translation(this->pos);

	//���C�e�B���O�𖳌��ɂ���
	DG::EffectState().param.lightsEnable = false;
	//�����x�ݒ�
	auto bsBuf = DG::EffectState().BS_Get();
	DG::EffectState().BS_Alpha();
	DG::EffectState().param.objectColor = ML::Color(this->alpha, 1, 1, 1);
	DG::EffectState().param.matWorld = matS * matR * matT;
	DG::Mesh_Draw(this->meshName);

	//�����x,���C�e�B���O�����ɖ߂�
	DG::EffectState().param.lightsEnable = true;
	DG::EffectState().BS_Set(bsBuf);
	DG::EffectState().param.objectColor = ML::Color(1, 1, 1, 1);	
}


//--------------------------------------------------------------------------------------------------
//�A�b�v�f�[�g�֐�
//0�ԁF�L�����N�^����
void Effect::UpDate_EF0()
{
	////easing::UpDate();

	//if (this->effect_Life == 140)
	//{
	//	easing::Start("scale_CreateCharactor");
	//	easing::Start("alpha_CreateCharactor");
	//}
	//else if(this->effect_Life < 140 && this->effect_Life >30)
	//{
	//	//easing�ŁA�傫���Ɠ����x��ω�����
	//	this->scale.x = easing::GetPos("scale_CreateCharactor");
	//	this->scale.y = easing::GetPos("scale_CreateCharactor");
	//	this->scale.z = easing::GetPos("scale_CreateCharactor");
	//	this->alpha = easing::GetPos("alpha_CreateCharactor");
	//}
	////30�t���[���������c������j�􂷂�悤�ɓ���
	//else if (this->effect_Life == 30)
	//{
	//	easing::Set("scale_CreateCharactor", easing::ELASTICOUT, 100, 300, 30);
	//	easing::Start("scale_CreateCharactor");
	//}
	//else if (this->effect_Life < 30)
	//{
	//	this->scale.x = easing::GetPos("scale_CreateCharactor");
	//	this->scale.y = easing::GetPos("scale_CreateCharactor");
	//	this->scale.z = easing::GetPos("scale_CreateCharactor");
	//	this->alpha -= 0.05f;
	//}

	////easing�Ɋւ�炸��������]���Ă���
	//this->angle.y += ML::ToRadian(2);
}
//1�ԁF�e���|�[�g�A�E�g
void Effect::UpDate_EF1()
{	
	//�t���[���������A�傫���Ȃ�
	this->alpha -= 0.008f;

	this->scale.y += 100.0f;
	
	//�����x��0�ȉ��ɗ����Ȃ��悤�ɂ���
	if (this->alpha < 0.0)
	{
		this->alpha = 0.0;
	}
}
//2�ԁF�e���|�[�g�C��
void Effect::UpDate_EF2()
{
	this->moveVec = this->target - this->pos;//�i�s���������炤
	//�ړ�
	this->pos += (this->moveVec * 0.25f);
	
	this->alpha -= 0.01f;
	//�����x��0�ȉ��ɗ����Ȃ��悤�ɂ���
	if (this->alpha < 0.0)
	{
		this->alpha = 0.0;
	}	
}
//3�ԁF�G�l�~�[���n
void Effect::UpDate_EF3()
{
	////easing start
	//if (this->effect_Life == 30)
	//{
	//	easing::Start("alpha_EnemyLanding");
	//	easing::Start("scaleXZ_EnemyLanding");
	//}
	////easing::UpDate();

	////XZ���ʂɍL����
	//this->scale.x = easing::GetPos("scaleXZ_EnemyLanding");
	//this->scale.z = easing::GetPos("scaleXZ_EnemyLanding");

	//this->alpha = easing::GetPos("alpha_EnemyLanding");
	//
}
//4�ԁF�G�l�~�[�ƏՓ�
void Effect::UpDate_EF4()
{
	////�J�����ɑ΂��Ċp�x�ύX
	//this->angle = ge->camera[0]->pos - this->pos;

	//this->angle.y = atan2(this->angle.z, this->angle.x);

	////�o��͑����A���ł܂ł͒x��(easing function�ɑ���\������ 2018/02/01)
	//if (this->effect_Life > 50)
	//{
	//	this->alpha += 0.1f;
	//}
	//else
	//{
	//	this->alpha -= 0.02f;
	//}

	////�����x��0~1�̊Ԃɂ���
	//if (this->alpha > 1.0f)
	//{
	//	this->alpha = 1.0f;
	//}
	//else if (this->alpha < 0.0f)
	//{
	//	this->alpha = 0.0f;
	//}

}
//5�ԁF�ǂƏՓ�
void Effect::UpDate_EF5()
{
	////�L�����ԈȊO�̓G�l�~�[�ƏՓ˂Ɠ���
	////�J�����ɑ΂��Ċp�x�ύX
	//this->angle = ge->camera[0]->pos - this->pos;

	//this->angle.y = atan2(this->angle.z, this->angle.x);

	//this->scale += ML::Vec3(10.0f, 10.0f, 10.0f);

	////�o��͑����A���ł܂ł͒x��(easing function�ɑ���\������ 2018/02/01)
	//if (this->effect_Life > 8)
	//{
	//	this->alpha += 0.42f;
	//}
	//else
	//{
	//	this->alpha -= 0.04f;
	//}

	////�����x��0~1�̊Ԃɂ���
	//if (this->alpha > 1.0f)
	//{
	//	this->alpha = 1.0f;
	//}
	//else if (this->alpha < 0.0f)
	//{
	//	this->alpha = 0.0f;
	//}

}
//6�ԁF�����ې���
void Effect::UpDate_EF6()
{
	//if (this->effect_Life > 60)
	//{
	//	return;
	//}
	////�����ۂɂ��Ă���
	//auto opm = ge->GetTask_One_G<OptionManager::Object>("�I�v�V����");
	//if (opm == nullptr)
	//{		
	//	return;
	//}

	//auto target = opm->tailList;
	//auto t = target.begin();

	//this->pos = (*t);

	////�c�������C�t��30�ȏ�Ȃ�傫���ȉ��Ȃ珬����
	//ML::Vec3 scailing(100.0f, 200.0f, 100.0f);
	//this->scale = scailing * sin(ML::ToRadian(float(60 - this->effect_Life) * 3));
	//
	////�������Ȃ�u�Ԃ��甖���Ȃ�
	//if (this->effect_Life <= 30)
	//{
	//	this->alpha -= 0.04f;
	//}

	////�����x��0�ȉ��ɗ����Ȃ��悤�ɂ���
	//if (this->alpha < 0.0)
	//{
	//	this->alpha = 0.0;
	//}
}
//7�ԁF�����ۏ���
void Effect::UpDate_EF7()
{
	//if(this->effect_Life == 300)
	//{
	//	easing::Start("posY_DestroyTail");
	//	easing::Start("scaleY_DestroyTail");
	//}
	////�J�����ɑ΂��Ċp�x�ύX
	//this->angle = ge->camera[0]->pos - this->pos;

	//this->angle.y = atan2(this->angle.z, this->angle.x);
	////easing function
	//
	////easing::UpDate();
	//this->pos.y = easing::GetPos("posY_DestroyTail");
	//this->scale.y = easing::GetPos("scaleY_DestroyTail");
}
//8�ԁF�����ۂƏՓ�
void Effect::UpDate_EF8()
{
	//4�ԁF�G�l�~�[�ƏՓ˂ƑS������
	//this->UpDate_EF4();
}
//9�ԁF�N���A�G�t�F�N�g
void Effect::UpDate_EF9()
{
	if (this->effect_Life > 200)
	{
		return;
	}
	else if (this->effect_Life > 10)
	{
		this->scale += ML::Vec3(8.0f, 8.0f, 8.0f);
		this->angle.y += ML::ToRadian(0.3f);
		this->alpha += 0.8f;		
	}
	else if (this->effect_Life == 10)
	{
		easing::Reset("scale_EF9");
		easing::Start("scale_EF9");
	}
	else if (this->effect_Life < 10)
	{
		//easing::UpDate();
		this->scale.x = easing::GetPos("scale_EF9");
		this->scale.y = easing::GetPos("scale_EF9");
		this->scale.z = easing::GetPos("scale_EF9");
	}
}



void Effect::Dec_Eff()
{
	this->effect_Life--;
}

bool Effect::Is_Alive() const
{
	return this->effect_Life > 0 ? true : false;
}

BEffect::effType Effect::Get_Type() const
{
	return this->playing_EffectHandle;
}

bool Effect::Eff_Judge() const
{
	return this->effect_Life == 0;
}