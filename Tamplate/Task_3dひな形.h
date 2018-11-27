#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//タイトル画面
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"

namespace Title
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("タイトル");	//グループ名
	const  string  defName("NoName");	//タスク名

	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object*  Create(bool flagGameEnginePushBack_);		
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
		void  Render3D_L0();
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		//メンバー変数・メソッドはここに追加する

	public:		
		
	};
}