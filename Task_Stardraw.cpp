//-------------------------------------------------------------------
//星（描画処理、当たり判定などの情報） 
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Stardraw.h"
#include  "Task_Player.h"

namespace  Stars
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/Stars.png");
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
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
		this->hitBase = ML::Box2D(-50,-40,100,80);	//接触矩形
		this->scoreAdd = 50;						//スコア加算
		this->pos.x = 500;
		
		
		for (int y = 0;y < 2;++y) {
			for (int x = 0;x < 5;++x) {
				if (this->patterns[this->side][y][x] != 0) {
					this->hit[y][x] = this->hitBase;
					this->hit[y][x].Offset(ML::Vec2(x * 100, y * 80));
				}
			}
		}


		for (int c = 0; c < 1; ++c)
		{
			int x = (c % 1);
			int y = (c / 1);
			this->chip[c] = ML::Box2D(100, 80, 100, 80);
		}
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
		this->moveCnt++;
		if (this->moveCnt > 300)
		{
			this->moveCnt = 0;
			/*this->pos.x = 500;*/
			/*this->moveVec.x = 15;
			this->side = rand() % 3;*/

			//ランダムは三パターン
			//変わるもの：位置、x座標。

			for (int y = 0;y < 2;y++) {
				for (int x = 0;x < 5;x++) {
					this->arr[this->side][y][x] = this->patterns[this->side][y][x];
				}
			}

		}

		this->pos.x -= this->moveVec.x;
	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{

		for (int y = 0;y < 2;++y) {
			for (int x = 0;x < 5;++x) {
				ML::Box2D draw(x * 100, y * 80, 100, 80);
				draw.Offset(this->pos);
				this->res->img->Draw(draw, this->chip[this->arr[this->side][y][x]]);
			}
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
	Object::Object() {}
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