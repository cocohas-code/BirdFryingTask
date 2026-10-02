//-----------------------------------------------------------------------------
//キャラクタ汎用スーパークラス
//-----------------------------------------------------------------------------
#include "BChara.h"
#include  "MyPG.h"

//-----------------------------------------------------------------------------
//めり込まない移動処理
void BChara::CheckMove(ML::Vec2& e_)
{
	//マップが存在するか調べてからアクセス

	//横軸に対する移動
	while (e_.x != 0) {
		float  preX = this->pos.x;
		if (e_.x >= 1) { this->pos.x += 1;		e_.x -= 1; }
		else if (e_.x <= -1) { this->pos.x -= 1;		e_.x += 1; }
		else { this->pos.x += e_.x;		e_.x = 0; }
		ML::Box2D  hit = this->hitBase.OffsetCopy(this->pos);
	}
	//縦軸に対する移動
	while (e_.y != 0) {
		float  preY = this->pos.y;
		if (e_.y >= 1) { this->pos.y += 1;		e_.y -= 1; }
		else if (e_.y <= -1) { this->pos.y -= 1;		e_.y += 1; }
		else { this->pos.y += e_.y;		e_.y = 0; }
		ML::Box2D  hit = this->hitBase.OffsetCopy(this->pos);

	}
}
  //接触判定
bool BChara::CheckHit(const ML::Box2D& hit_)
{
	ML::Box2D you = this->hitBase.OffsetCopy(this->pos);
	return you.Hit(hit_);
}

