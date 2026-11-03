#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//新しい星の処理（配列で流せるか試みる）
//-------------------------------------------------------------------
#include "BChara.h"

namespace  Stars
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("星たち");	//グループ名
	const  string  defName("Noname");		//タスク名
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
		//共有する変数はここに追加する
		DG::Image::SP img = DG::Image::Create("./data/image/Star.png");
		DG::Font::SP f;	//後で消す
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
		//追加したい変数・メソッドはここに追加する
		//BCharaに含まれないモノのみここに追加する
		int arr[3][2][5];	//配置配列
		int scoreAdd;		//スコア加算用
		int side;			//乱数用
		static const int pattern = 3;	//パターン
		static const int col = 2;		//縦
		static const int row = 5;       //横
		ML::Box2D chip[2];				//画像チップ
		ML::Box2D hit[2][5];
		
		int patterns[pattern][col][row] =
		{
			{ 
				{1, 0, 0, 0, 1},
				{0, 0, 1, 0, 0}
			},
			{ 
				{0, 1, 0, 1, 0},
				{1, 0, 0, 0, 1}
			},
			{ 
				{0, 0, 1, 0, 0},
				{1, 0, 0, 0, 1}
			}
		};

		
		
	};
}