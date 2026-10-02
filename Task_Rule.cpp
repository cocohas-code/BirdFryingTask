//-------------------------------------------------------------------
//状態遷移-ルール説明
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Rule.h"

namespace  Rule
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->p1.reset();
		this->p2.reset();
		this->p3.reset();
		this->right.reset();
		this->left.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		p = Page::p1;
		t = Turn::Right;

		this->pageDraw = ML::Box2D(150, 20, 900, 630);
		this->pageSrc = ML::Box2D(0, 0, 900, 630);
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();

		if (inp.LStick.BR.down) {
			if (pageCnt < pageMax)
			{
				this->pageCnt++;
			}
		}
		else if (inp.LStick.BL.down) {
			if (pageCnt > pageRow)
			{
				this->pageCnt--;
			}
		
		}
		
	
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{
		//表紙
		switch (pageCnt)
		{
		case 1:
			p = Page::p1;
			this->res->p1->Draw(this->pageDraw, this->pageSrc);
			break;
		case 2:
			p = Page::p2;
			this->res->p2->Draw(this->pageDraw, this->pageSrc);
			break;
		case 3:
			p = Page::p3;
			this->res->p3->Draw(this->pageDraw, this->pageSrc);
			break;
		default:
			p = Page::non;
			break;
		}

		//矢印
		if (pageCnt != pageMax)
		{
			//右矢印
			ML::Box2D drawR(1200, 350, 50, 100);
			ML::Box2D srcR(0, 0, 50, 100);
			this->res->right->Draw(drawR, srcR);
		}
		if (pageCnt != pageRow)
		{
			//左矢印
			ML::Box2D drawL(0, 350, 50, 100);
			ML::Box2D srcL(0, 0, 50, 100);
			this->res->left->Draw(drawL, srcL);
		}
	}

	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//以下は基本的に変更不要なメソッド
	//★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★★
	//-------------------------------------------------------------------
	//タスク生成窓口
	Object::SP  Object::Create(bool  flagGameEnginePushBack_)
	{
		Object::SP  ob = Object::SP(new  Object());
		if (ob) {
			ob->me = ob;
			if (flagGameEnginePushBack_) {
				ge->PushBack(ob);//ゲームエンジンに登録
				
			}
			if (!ob->B_Initialize()) {
				ob->Kill();//イニシャライズに失敗したらKill
			}
			return  ob;
		}
		return nullptr;
	}
	//-------------------------------------------------------------------
	bool  Object::B_Initialize()
	{
		return  this->Initialize();
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
	//リソースクラスの生成
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