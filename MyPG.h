#pragma once
#include "GameEngine_Ver3_7.h"
#include <fstream>
#include <sstream>
#include "Collision.h"
#include "Dm2008_Ver1_3.h"
#include "Teleportation_Manager.h"
#include "Task_Effect_Manager.h"
#include "Task_Game.h"


//�Q�[�����S��Ŏg�p����\���̂Ȃǂ��`����
//-----------------------------------------------------------------------------------------------



//-----------------------------------------------------------------------------------------------
namespace  MyPG
{
	//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	//----------------------------------------------
	//�J������{�`
	class Camera
	{
		Camera( ){ }
		Camera(
				const ML::Vec3&		tg_,	//	��ʑ̂̈ʒu
				const ML::Vec3&		pos_,	//	�J�����̈ʒu
				const ML::Vec3&		up_,	//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
				float				fov_,	//	����p
				float				np_,	//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
				float				fp_,	//	��N���b�v���ʁi��������͉f��Ȃ��j
				float				asp_);	//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j			
	public:
	//	�r���[���i�����֘A�j
		ML::Vec3 target;			//	��ʑ̂̈ʒu
		ML::Vec3 pos;			//	�J�����̈ʒu
		ML::Vec3 up;				//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
	//	�ˉe���i����͈͊֘A�j
		float fov;					//	����p
		float nearPlane;			//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
		float forePlane;			//	��N���b�v���ʁi��������͉f��Ȃ��j
		float aspect;				//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j
	//	�s����
		ML::Mat4x4  matView, matProj;
		~Camera( );
		typedef shared_ptr<Camera> SP;
	//	�J�����𐶐�����
		static SP Create(
				const ML::Vec3&		tg_,	//	��ʑ̂̈ʒu
				const ML::Vec3&		pos_,	//	�J�����̈ʒu
				const ML::Vec3&		up_,	//	�J�����̏�����������x�N�g���i��̂x�{�Œ�j
				float				fov_,	//	����p
				float				np_,	//	�O�N���b�v���ʁi������O�͉f��Ȃ��j
				float				fp_,	//	��N���b�v���ʁi��������͉f��Ȃ��j
				float				asp_);	//	�A�X�y�N�g��i��ʂ̔䗦�ɍ��킹��@�����c�j	
	//	�J�����̐ݒ�
		void UpDate( );
	};
	//----------------------------------------------
	class MyGameEngine : public GameEngine
	{
	public:
		MyGameEngine( );
	//�Q�[���G���W���ɒǉ��������̂̏������ƊJ��
		bool Initialize(HWND wnd_, IDXGIAdapter* adp);
		~MyGameEngine( );
	//�Q�[���G���W���ɒǉ��������̂̃X�e�b�v����
		void UpDate( );

	//3DPG1�Ή��ɂ��ǉ�
		//2D�`����̃f�t�H���g�p�����[�^�ݒ�
		void Set2DRenderState(DWORD l_);
		//3D�`����̃f�t�H���g�p�����[�^�ݒ�
		void Set3DRenderState(DWORD l_);



	//�Q�[���G���W���ɒǉ����������͉̂��ɉ�����
	//----------------------------------------------
		MyPG::Camera::SP		camera[4];		//	�J����
		D3D10_VIEWPORT			vp3D_L0;
		D3D10_VIEWPORT			vp3D_L1;
		D3D10_VIEWPORT			vp3D_L2;
		D3D10_VIEWPORT			vp3D_L3;
		D3D10_VIEWPORT			vp3D_BF;
		D3D10_VIEWPORT			vp3D_AF;		
		
		bool gameClearFlag;
		bool getReadyFlag;

		//���[���h��]��
		//ML::QT World_Rotation; 
		//�}�b�v�S�̂̒��S
		ML::Vec3 Map_center;
		//�����蔻��̌���
		std::list<After_Collision> collision_Result;
		//�e���|�[�e�[�V�����}�l�[�W���[
		Teleportation_Manager TM;
		//�G�t�F�N�g�}�l�[�W���[
		weak_ptr <EffectManager::Object> eff_Manager;
		//�{�҂��풓������
		weak_ptr<Game::Object> game;
	//----------------------------------------------
	};
}
extern MyPG::MyGameEngine* ge;


