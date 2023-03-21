#include "SceneChangeDirection.h"

SceneChangeDirection* SceneChangeDirection::GetInstance()
{
	static SceneChangeDirection instance;
	return &instance;
}

void SceneChangeDirection::Initialize()
{
	//sprite読み込み
	SpriteBase::GetInstance()->LoadTexture(5, L"Resources/SceneChange.png");
	// スプライトの生成
	sp_scenechange.reset(Sprite::Create(5, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));

	sp_scenechange->SetPosition({ 1280,0,0 });
}

void SceneChangeDirection::HideTheScreen()
{

	XMFLOAT3 pos = sp_scenechange->GetPosition();

	HideVel = -HideSp;//右から左に隠してく

	pos.x += HideVel;
	sp_scenechange->SetPosition({ pos });

	if (pos.x <= 0) {
		HideTheScreenFlag = false;//隠したから戻す
		//SceneChangeCompFlag = true;
	}
}

void SceneChangeDirection::Update()
{
	if (HideTheScreenFlag) {
		HideTheScreen();//画面隠す条件達成で隠し開始
	}

	sp_scenechange->Update();
}

void SceneChangeDirection::Draw()
{
	sp_scenechange->Draw();
}
