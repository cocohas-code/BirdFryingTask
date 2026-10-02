#pragma once
#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//キャラクタスーパータスク
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

class BChara : public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef  shared_ptr<BChara>		SP;
	typedef  weak_ptr<BChara>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	//キャラクタ共通メンバ
	ML::Vec2	pos;
	ML::Box2D	hitBase;
	ML::Vec2	moveVec;
	int			moveCnt;
	int			unHitTime;

	//左右の向き(2D横視点ゲーム専用)
	//メンバ変数に最低限の初期化を行う
	BChara()
		:pos(0, 0)				//座標
		,hitBase(0,0,0,0)       //接触矩形
		,moveVec(0, 0)          //移動ベクトル
		,moveCnt(0)             //移動カウント
		,unHitTime(0)           //接触無カウント
	{

	}
	virtual ~BChara() {}

	//キャラクタ共通メソッド
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//めり込まない移動処理
	void CheckMove(ML::Vec2& e_);
	//接触判定
	virtual bool CheckHit(const ML::Box2D& hit_);
};
