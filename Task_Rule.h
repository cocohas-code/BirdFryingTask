#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//ルール説明
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

namespace  Rule
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ルール説明");	//グループ名
	const  string  defName("NoName");	//タスク名
	//-------------------------------------------------------------------
	class  Resource : public BResource
	{
		bool  Initialize()	override;
		bool  Finalize()	override;
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		DG::Image::SP p1 = DG::Image::Create("./data/image/Rules/page1.png"),
			          p2 = DG::Image::Create("./data/image/Rules/page2.png"),
			          p3 = DG::Image::Create("./data/image/Rules/page3.png"),
			          right = DG::Image::Create("./data/image/Rules/right.png"),
			          left = DG::Image::Create("./data/image/Rules/left.png");

	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
		//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
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
		void  UpDate()			override;//「実行」１フレーム毎に行う処理
		void  Render2D_AF()		override;//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		int pageCnt = 1;
		const int pageMax = 3;
		const int pageRow = 1;
		ML::Box2D pageDraw;
		ML::Box2D pageSrc;
		ML::Box2D turnDraw;
		ML::Box2D turnSrc;
		enum class Page{p1, p2, p3,non};
		enum class Turn{Left, Right, Non};
		Page p;
		Turn t;
	};
}