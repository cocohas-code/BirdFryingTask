//-------------------------------------------------------------------
//プレイヤー（移動や当たり判定など）
//-------------------------------------------------------------------
#include  "MyPG.h"
#include  "Task_Player.h"
#include  "Task_Enemy.h"
#include  "Task_Life.h"
#include  "Task_Star.h"
#include  "Task_Stardraw.h"

//-------------------------------------------------------------------
//プレイヤー、接触判定等
//-------------------------------------------------------------------

namespace  Player
{
	Resource::WP  Resource::instance;
	//-------------------------------------------------------------------
	//リソースの初期化
	bool  Resource::Initialize()
	{
		this->img = DG::Image::Create("./data/image/player.png");
		this->font = DG::Font::Create("HGS 教科書体", 10, 50);
		return true;
	}
	//-------------------------------------------------------------------
	//リソースの解放
	bool  Resource::Finalize()
	{
		this->img.reset();
		/*this->font.reset();*/
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
		this->moveCnt = 0;									//移動カウント
		this->hitBase = ML::Box2D(-100, -75, 200, 150);		//接触判定用の矩形
		this->pos = ML::Vec2(300, 200);						//座標
		this->moveVec = ML::Vec2(10,10);					//移動ベクトル
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
	//移動ーーーーーーーーーーーーーーーーーーーーーーーーーーーー
		ML::Vec2 est(0, 0);
		if (inp.LStick.BR.on) {
			est.x += this->moveVec.x;
		}
		if (inp.LStick.BL.on) {
			est.x -= this->moveVec.x;
		}
		if (inp.LStick.BU.on) {
			est.y -= this->moveVec.y;
		}
		if (inp.LStick.BD.on) {
			est.y += this->moveVec.y;
		}
		//斜め移動
		if (inp.LStick.BD.on || inp.LStick.BU.on ||
			inp.LStick.BR.on || inp.LStick.BL.on) {

			float angle = inp.LStick.angleDYP;
			float x = cos(angle) * this->moveVec.x;
			float y = sin(angle) * this->moveVec.y;

			if (inp.LStick.volume >= 10) {
				this->pos.x += x;
				this->pos.y += y;
			}
		}
		this->CheckMove(est);
		
	//接触判定処理ーーーーーーーーーーーーーーーーーーーーーーーーーーーー
		ML::Box2D me = this->hitBase.OffsetCopy(this->pos);
		
		//   -敵-
		auto target = ge->GetTask<Enemy::Object>("敵");
		ML::Box2D you = target->hitBase.OffsetCopy(target->pos);
		//０以外は無敵状態じゃない
		if (this->unHitTime > 0)
		{
			this->unHitTime--;
		}
		if (true == you.Hit(me) &&
			this->unHitTime <= 0)
		{
			//当たったら無敵状態
			//ライフも減る
			this->unHitTime = 60;
			ge->Playerlife--;
		}

		//   -アイテム-
		auto Item = ge->GetTasks<BChara>("アイテム");
		for (auto it = Item->begin();
			it != Item->end();
			++it) {
			//相手に接触の有無を確認
			if ((*it)->CheckHit(me)) {
				(*it)->Kill();
				break;
			}
			
		}

		//   -星-
		auto targets = ge->GetTasks<Star::Object>("星");
		for (auto it = targets->begin();
			it != targets->end();
			++it) {
			if ((*it)->CheckHit(me)) {
				(*it)->pos.x = (*it)->startPos;
				ge->score += 50;
				(*it)->pos.y = rand() % ((720 + 1) - 100);
				break;
			}
		}

		//auto targets = ge->GetTasks<Stars::Object>("星たち");
		//for (auto it = targets->begin();
		//	it != targets->end();
		//	++it) {
		//	for (int y = 0;y < 2;++y) {
		//		for (int x = 0;x < 5;++x) {
		//			ML::Box2D you = (*it)->hit[y][x].OffsetCopy((*it)->pos);
		//			if (true == you.Hit(me)) {
		//				(*it)->Kill();
		//				//消える、スコアが加算される
		//			}
		//		}
		//	}

		//}

	}
	//-------------------------------------------------------------------
	//「２Ｄ描画」１フレーム毎に行う処理
	void  Object::Render2D_AF()
	{

		//プレイヤー描画
		ML::Box2D draw = this->hitBase;
		draw.Offset(this->pos);
		ML::Box2D src(0, 0, 250, 200);
		//無敵状態、点滅
		if (this->unHitTime > 0) {
			if ((this->unHitTime / 4) % 2 == 0) {
				src.w = 0;
				src.h = 0;
			}
		}
		this->res->img->Draw(draw, src);
		//ライフ確認用
		ML::Box2D textBox0(500, 200, 700, 100);
		string text0 = to_string(ge->Playerlife);
		this->res->font->Draw(textBox0, text0, ML::Color(1, 1, 1, 1));
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
