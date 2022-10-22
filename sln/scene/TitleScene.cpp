#include "TitleScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "DebugText.h"
#include "GamePlayScene.h"

#include "../PostEffect.h"

//PostEffect* postEffect = nullptr;

using namespace DirectX;

void TitleScene::Initialize()
{
#pragma region 描画初期化処理

	// マウスカーソル非表示
	Input* input = Input::GetInstance();
	input->MouseCursorHiddenFlag(false);

	////---objからモデルデータ読み込み---
	//model_1 = Model::LoadFromOBJ("ground");
	//model_2 = Model::LoadFromOBJ("triangle_mat");
	////Model* model_3 = Model::LoadFromOBJ("chr_sword");
	////---3dオブジェクト生成---
	//object3d_1 = Object3d::Create();
	//object3d_2 = Object3d::Create();
	//object3d_3 = Object3d::Create();
	////---3dオブジェクトに3dモデルを紐づける---
	//object3d_1->SetModel(model_1);
	//object3d_2->SetModel(model_2);
	//object3d_3->SetModel(model_2);

	//object3d_2->SetScale({ 20.0f, 20.0f, 20.0f });
	//object3d_3->SetScale({ 30.0f, 30.0f, 30.0f });

	//object3d_2->SetPosition({ 5,-1,5 });
	//object3d_3->SetPosition({ -5,-1,5 });

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("A_rhythmaze_125.wav");

	// 音声再生
	GameSound::GetInstance()->PlayWave("A_rhythmaze_125.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	// 3Dオブジェクトの数
	//const int OBJECT_NUM = 2;

	//Object3d object3ds[OBJECT_NUM];

	// スプライト共通テクスチャ読み込み
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/title_prac.png");

	// スプライトの生成
	sprite1.reset(Sprite::Create(1, XMFLOAT3(0, 0, 0), { 0,0 }, { 1,1,1,1 }, { 0, 0 }, false, false));
	//for (int i = 0; i < 1; i++)
	//{
	//    int texNumber = 1;
	//    Sprite* sprite = Sprite::Create(spriteBase, texNumber, { 0,0 }, false, false);

	//    // スプライトの座標変更
	//    sprite->SetPosition({ (float)(80),(float)(20),0 });
	//    //sprite->SetRotation((float)(rand() % 360));
	//    sprite->SetSize({ (float)(200), (float)(200) });

	//    sprite->TransferVertexBuffer();

	//    sprites.push_back(sprite);
	//}

	//ポストエフェクト用テクスチャ読み込み
	//SpriteBase::GetInstance()->LoadTexture(100, L"Resources/white1x1.png");
	//ポストエフェクト初期化
//	postEffect = new PostEffect();
////	postEffect->Initialize();//100, { 0.0f, 0.0f }, false, falseいちおう
//	postEffect->SetSize({ 500.0f,500.0f });
//	postEffect->SetPosition({ 10,10,0 });
//	postEffect->TransferVertexBuffer();
#pragma endregion 描画初期化処理

	int counter = 0; // アニメーションの経過時間カウンター
}

void TitleScene::Finalize()
{

	//スプライト解放
	//delete sprite;

	//3dオブジェクト解放
	//delete object3d_1;
	//delete object3d_2;
	//delete object3d_3;
	//delete model_1;
	//delete model_2;

//	delete postEffect;
}

void TitleScene::Update()
{
	Input* input = Input::GetInstance();

	//押した瞬間のみ
	const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
	const bool TriggerESC = input->TriggerKey(DIK_ESCAPE);
	//パッド押した瞬間
	const bool PadTriggerA = input->TriggerButton(static_cast<int>(Button::A));

	if (TriggerSPACE||PadTriggerA)     // スペースキーが押されていたら
	{
		input->PadVibration();
		// 音声停止
		GameSound::GetInstance()->SoundStop("A_rhythmaze_125.wav");
		//シーン切り替え
		BaseScene* scene = new GamePlayScene();
		sceneManager_->SetNextScene(scene);
	}

	DrawUI();

	//DebugText::GetInstance()->Print("nihon kougakuin!", 200, 200, 2.0f);

	////3dobj
	//object3d_1->Update();
	//object3d_2->Update();
	//object3d_3->Update();

	//スプライト更新
	sprite1->Update();
	//postEffect->Update();

	//終了
	if (TriggerESC) {
		WM_DESTROY;//破棄されるウィンドに送信
		PostQuitMessage(0);//WM_DESTROYの応答、終了要求
	}
}

void TitleScene::Draw()
{
	//// スプライト共通コマンド
	SpriteBase::GetInstance()->PreDraw();
	//SpriteCommonBeginDraw(SspriteBase, dxBase->GetCmdList());
	//// スプライト描画
	sprite1->Draw();

	//ポストエフェクトの描画
	//postEffect->Draw();

	////3dオブジェ描画前処理
	//Object3d::PreDraw();

	////3dオブジェ描画
	//object3d_1->Draw();
	//object3d_2->Draw();
	//object3d_3->Draw();

	////3dオブジェ描画後処理
	//Object3d::PostDraw();

	// ４．描画コマンドここから

	//for (int i = 0; i < _countof(object3ds); i++)
	//{
	//    DrawObject3d(&object3ds[i], dxBase->GetCmdList(), basicDescHeap.Get(), vbView, ibView,
	//        CD3DX12_GPU_DESCRIPTOR_HANDLE(basicDescHeap->GetGPUDescriptorHandleForHeapStart(), constantBufferNum, dxBase->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)),
	//        indices, _countof(indices));
	//}

	//// スプライト共通コマンド
	//SpriteBase::GetInstance()->PreDraw();
	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// スプライト描画
	//sprite->Draw();
}

void TitleScene::DrawUI()
{
	DebugText::GetInstance()->Print("[SPACEorGAMEPAD:A] PLAYSCENE", 300, 100, 3.0f);
}
