﻿//きゃら固有値
#include "CharParameters.h"
#include "Object3d.h"
#include "Input.h"
#include "GameSound.h"
#include "DebugText.h"
#include <DirectXMath.h>

#ifdef max
#undef max
#endif // max

#ifdef min
#undef min
#endif // min

using namespace DirectX;

CharParameters* CharParameters::GetInstance()
{
	static CharParameters instance;
	return &instance;
}
void CharParameters::Initialize()
{
	// -----------------スプライト共通テクスチャ読み込み
	SpriteBase::GetInstance()->LoadTexture(3, L"Resources/HPbar.png");
	SpriteBase::GetInstance()->LoadTexture(4, L"Resources/HPbar_waku.png");
	SpriteBase::GetInstance()->LoadTexture(5, L"Resources/playerHPbar.png");
	SpriteBase::GetInstance()->LoadTexture(6, L"Resources/playerHPbar_waku.png");

	//スプライト生成
	sp_enemyhpbar.reset(Sprite::Create(3, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_enemyhpbarwaku.reset(Sprite::Create(4, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_playerhpbar.reset(Sprite::Create(5, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	sp_playerhpbarwaku.reset(Sprite::Create(6, XMFLOAT3(1, 1, 1), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	//スプライトポジション
	sp_enemyhpbar->SetPosition({ 140,-80,0 });
	sp_enemyhpbarwaku->SetPosition({ 140,-80,0 });
	sp_playerhpbar->SetPosition({ 80,520,0 });
	sp_playerhpbarwaku->SetPosition({ 30,520,0 });
	//ｰｰ色
	///自機HPバー最初の色
	sp_playerhpbar->SetColor({ 0,1,0,1 });

	//パラメータ関連初期化
	BossDefense = BossDefenseDef;
	NowBossHP = BossMaxHP;//
	NowPlayerHP = PlayerMaxHP;//
	pNextPlaceGoFlag = true;
}

void CharParameters::pHpSizeChange()
{
	sp_playerhpbar->size_.x = sp_playerhpbar->texSize_.x * (float)NowPlayerHP / PlayerMaxHP;
	sp_playerhpbar->TransferVertexBuffer();
}
void CharParameters::boHpSizeChange()
{
	//サイズ変更
	sp_enemyhpbar->size_.x = sp_enemyhpbar->texSize_.x * (float)NowBossHP / BossMaxHP;
	sp_enemyhpbar->TransferVertexBuffer();
}

void CharParameters::Update()
{
}

void CharParameters::PlayerHpBarColorChange() 
{
	constexpr float ColorWDec = 0.012f;//透明にしていく速度
	constexpr float Transparency = 0.5f;//最終的な透明度がどこまで行くか。ここまでいったらデフォ値に戻す

	XMFLOAT4 color = sp_playerhpbar->GetColor();
	color.x = 1.f;

	if (pHpBarFrame <= 0) {//指定時間たったら
		color.w -= ColorWDec;
	}

	if (color.w <= Transparency) {
		pHpBarFrame = pHpBarFrameDef;//またこの時間分まつ
		color.w = 1.f;//一番明るい状態
	}

	pHpBarFrame = std::max(--pHpBarFrame, 0);//ToStartFrameの最小値は0

	sp_playerhpbar->SetColor(color);
	sp_playerhpbar->TransferVertexBuffer();
}

void CharParameters::pHpUpdate()
{
	pHpSizeChange();

	//自機HPが一定以上削られたら
	if (NowPlayerHP <= PlayerMaxHP / 3.f) {
		PlayerHpBarColorChange();
	}

	sp_playerhpbar->Update();
	sp_playerhpbarwaku->Update();
}

void CharParameters::boHpUpdate()
{
	boHpSizeChange();
	//更新
	sp_enemyhpbar->Update();
	sp_enemyhpbarwaku->Update();
}

void CharParameters::Draw()
{
}
void CharParameters::pHpDraw()
{
	sp_playerhpbar->Draw();
	sp_playerhpbarwaku->Draw();
}
void CharParameters::boHpDraw()
{
	sp_enemyhpbar->Draw();
	sp_enemyhpbarwaku->Draw();
}

void CharParameters::DrawUI()
{

}