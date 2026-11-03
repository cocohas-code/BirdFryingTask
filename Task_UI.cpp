//-------------------------------------------------------------------
//Game：UIの描画(ライフ、スコア、カウントダウン等)
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_UI.h"

namespace  UI
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
		this->imgKey.reset();
		this->imgLife.reset();
		this->imgCnt.reset();
		this->imgSco.reset();
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
		this->pos_Key = ML::Vec2(1200, 650);
		this->pos_Life = ML::Vec2(100, 100);
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
		
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	//-------------------------------------------------------------------
	void  Object::Render2D_AF()
	{
		//移動
		ML::Box2D draw(-100, -67, 200, 134);
		draw.Offset(this->pos_Key);
		ML::Box2D src(0, 0, 400, 267);
		this->res->imgKey->Draw(draw, src);

		Draw_Score();	 //スコア
		Draw_Life();	 //ライフ
	}
	//-------------------------------------------------------------------
	//獲得したスコアを描画する
	void Object::Draw_Score()
	{
		// Score:
		ML::Box2D draws(0, 0, 195, 65);
		ML::Box2D srcs(0, 0, 195, 65);
		this->res->imgSco->Draw(draws, srcs);

		// 数値
		snprintf(dispscore, 5, "%04d", ge->score);
		for (int i = 0; i < 4; i++)
		{
			ML::Box2D draw(195, 0, 50, 75);
			ML::Box2D src((dispscore[i] - '0') * 50, 0, 50, 75);
			draw.Offset(i * 50 + 50, 0);
			this->res->imgCnt->Draw(draw, src);
		}
	}
	//-------------------------------------------------------------------
	//プレイヤーのスコアを描画する
	void Object::Draw_Life()
	{
		for (int i = 0;i < ge->Playerlife;++i)
		{
			//MEMO:座標はXYともに１００
			ML::Box2D draw(-25 + i * 80, -21, 50, 42);
			draw.Offset(this->pos_Life);
			ML::Box2D src(0, 0, 50, 42);
			this->res->imgLife->Draw(draw, src);
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