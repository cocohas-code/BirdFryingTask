#pragma once
#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//キャラクタスーパータスク
//-------------------------------------------------------------------
#include "GameEngine_Ver3_83.h"

class BChara_Star : public BTask
{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
public:
	typedef  shared_ptr<BChara_Star>		SP;
	typedef  weak_ptr<BChara_Star>		WP;
public:
	//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	//キャラクタ共通メンバ
	ML::Vec2	pos;
	ML::Box2D	hitBase;
	ML::Vec2	moveVec;
	int			moveCnt;
	int			arr[2][5];
	
	//左右の向き(2D横視点ゲーム専用)
	//メンバ変数に最低限の初期化を行う
	BChara_Star()
		:pos(0, 0)
		, hitBase(0, 0, 0, 0)
		, moveVec(0, 0)
		, moveCnt(0)
		
	{

	}
	virtual ~BChara_Star() {}

	//キャラクタ共通メソッド
	//ーーーーーーーーーーーーーーーーーーーーーーーーーーー
	//接触判定
	
	
	
};
