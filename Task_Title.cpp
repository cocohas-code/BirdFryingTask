//-------------------------------------------------------------------
//タイトルタスク
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Title.h"
#include  "Task_Game.h"
#include  "Task_Rule.h"
#include  "Task_UI.h"
#include "Task_GameBG.h"


namespace  Title
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
		this->imgBK.reset();
		this->imgBG.reset();
		this->imgLogo.reset();
		this->imgB.reset();
		this->imgUI.reset();
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
		this->fadeAlpha = 1.0f;
		this->fadeAlpha2 = 0;
		this->fadingIN = true;
		this->botan = State::Left;	//選択、最初は左
		//★タスクの生成
		auto BG = GameBG::Object::Create(true);  //背景

		return  true;
	}
	//-------------------------------------------------------------------
	//「終了」タスク消滅時に１回だけ行う処理
	bool  Object::Finalize()
	{
		//★データ＆タスク解放


		if (!ge->QuitFlag() && this->nextTaskCreate) {
			//★引き継ぎタスクの生成
			//左を選択
			if (this->botan == State::Left)
			{
				auto next = Game::Object::Create(true);
			}
			//右を選択
			else
			{
				auto next = Rule::Object::Create(true);
			}
		}

		return  true;
	}
	//-------------------------------------------------------------------
	//「更新」１フレーム毎に行う処理
	void  Object::UpDate()
	{
		auto inp = ge->in1->GetState();
		//HACK:もう少し修正したいです。

		//フェードイン
		if (this->fadingIN)
		{
			this->fadeAlpha -= 0.1f;
			if (fadeAlpha <= 0)
			{
				this->fadeAlpha = 0;
				this->fadingIN = false;
			}
		}
		
		//右…あそびかた
		//左…ゲーム
		if (inp.LStick.BR.on)
		{
			this->botan = State::Right;
		}
		if (inp.LStick.BL.on)
		{
			this->botan = State::Left;
		}
		
		if (this->botan == State::Left)
		{
			
			if (true == inp.SE.down) {
				this->Kill();
			
			}
		}

		if (this->botan == State::Right)
		{
			//ルール説明へ（まだ仮）
			if (true == inp.SE.down) {
				this->Kill();
			}
		}

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{

		////ロゴ
		ML::Box2D draw1(250, 50, 660, 440);
		ML::Box2D src1(0, 0, 660, 440);
		this->res->imgLogo->Draw(draw1, src1);

		//星（選択目印）
		ML::Box2D draw2(60, 500, 200, 160);
		ML::Box2D src2(0, 0, 200, 160);
		//左（STATE）を選んでたら
		if (this->botan == State::Left)
		{
			this->res->imgB->Draw(draw2, src2);
		}
		//右だったら
		else if (this->botan == State::Right)
		{
			draw2.x += 600;
			this->res->imgB->Draw(draw2, src2);
		}

		//[スタート]
		ML::Box2D draw3(200, 500, 230, 176);
		ML::Box2D src3(0, 0, 230, 176);
		this->res->imgUI->Draw(draw3, src3);

		//[ルール]
		ML::Box2D draw4(800, 500, 220, 176);
		ML::Box2D src4(231, 0, 220, 176);
		this->res->imgUI->Draw(draw4, src4);

	    //フェードイン用の黒画像
		ML::Box2D draw5(0, 0, 1270, 720);
		ML::Box2D src5(0, 0, 1270, 720);
		this->res->imgBK->Draw(draw5, src5, ML::Color(fadeAlpha, 1.0f, 1.0f, 1.0f));
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