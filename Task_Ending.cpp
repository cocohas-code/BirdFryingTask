//-------------------------------------------------------------------
//リザルト画面
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Ending.h"
#include  "Task_Game.h"

namespace  Ending
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	//-------------------------------------------------------------------
	bool  Resource::Initialize()
	{
		this->imgBG = DG::Image::Create("./data/image/Endings/Curtain.png"); //カーテン
		this->imgB = DG::Image::Create("./data/image/Endings/Board.png");	//ボード
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	//-------------------------------------------------------------------
	bool  Resource::Finalize()
	{
		this->imgBG.reset();
		return true;
	}
	//-------------------------------------------------------------------
	//「初期化」タスク生成時に１回だけ行う処理
	//-------------------------------------------------------------------
	bool  Object::Initialize()
	{
		//スーパークラス初期化
		__super::Initialize(defGroupName, defName, true);
		//リソースクラス生成orリソース共有
		this->res = Resource::Create();

		//★データ初期化
		this->BGpos = ML::Vec2(0, -1200);
		this->Bpos = ML::Vec2(700, -600);
		this->BGmoveCnt = 20;
		this->BmoveCnt = 20;
		//★タスクの生成

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	//-------------------------------------------------------------------
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
	//-------------------------------------------------------------------
	void  Object::UpDate()
	{
		if (this->BGmoveCnt != 0)
		{
			BG_Move();
		}
		else
		{
			Board_Move();
		}
		
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	//-------------------------------------------------------------------
	void  Object::Render2D_AF()
	{
		//カーテンの描画
		ML::Box2D draw0(0, 0, 1280, 720);
		draw0.Offset(this->BGpos);
		ML::Box2D src0(0, 0, 1200, 720);
		this->res->imgBG->Draw(draw0, src0);

		//ボードの描画
		ML::Box2D draw1(0, 0, 469, 572);
		draw1.Offset(this->Bpos);
		ML::Box2D src1(0, 0, 469, 572);
		this->res->imgB->Draw(draw1, src1);
	}
	//-------------------------------------------------------------------
	//背景が上から下へ動く
	void Object::BG_Move()
	{
		auto inp = ge->in1->GetState();
		this->BGpos.y += this->BGmoveCnt;
		if (this->BGpos.y >= 0)
		{
			this->BGmoveCnt = 0;
		}
		//Aキー押したら飛ばせる
		if (inp.SE.down)
		{
			this->BGmoveCnt = 0;
			this->BGpos.y = 0;
		}
	}
	//-------------------------------------------------------------------
	//ボードが上から下へ動く
	void Object::Board_Move()
	{
		auto inp = ge->in1->GetState();
		this->Bpos.y += this->BmoveCnt;
		if (this->Bpos.y >= 0)
		{
			this->BmoveCnt = 0;
		}
		//Aキー押したら早く実行可能
		if (inp.SE.down)
		{
			this->BmoveCnt = 0;
			this->Bpos.y = 0;
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