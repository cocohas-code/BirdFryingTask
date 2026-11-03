#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//UIの描画(ライフ、スコア、カウントダウン等)
//-------------------------------------------------------------------
#include "BChara.h"

namespace  UI
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ユーアイ");	//グループ名
	const  string  defName(	"Noname");		//タスク名
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()		override;
		bool  Finalize()		override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		DG::Image::SP imgKey = DG::Image::Create("./data/image/Key.png"),
			imgLife = DG::Image::Create("./data/image/heart.png"),
			imgCnt = DG::Image::Create("./data/image/count.png"),
			imgSco = DG::Image::Create("./data/image/score.png");
	};
	//-------------------------------------------------------------------
	class  Object : public  BChara
	{
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:

		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate()		override;	//「実行」１フレーム毎に行う処理
		void  Render2D_AF()	override;	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
	public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
		char dispscore[5];
		ML::Vec2 pos_Key;	//移動UI座標
		ML::Vec2 pos_Life;  //ライフ座標
		void Draw_Score();  //スコア描画
		void Draw_Life();   //ライフ描画
	};
}