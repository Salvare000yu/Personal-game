#include "ClearScene.h"
#include "SceneManager.h"
#include "GameSound.h"
#include "Input.h"
#include "ComplexInput.h"
#include "DebugText.h"
//#include "GamePlayScene.h"
#include "TitleScene.h"
#include "Timer.h"

using namespace DirectX;

void ClearScene::Initialize()
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
	GameSound::GetInstance()->LoadWave("verROOP_tukawanakutemoiiYO.wav");
	GameSound::GetInstance()->LoadWave("personalgame_decision.wav");

	// 音声再生
	GameSound::GetInstance()->PlayWave("verROOP_tukawanakutemoiiYO.wav", 0.2f, XAUDIO2_LOOP_INFINITE);

	// 3Dオブジェクトの数
	//const int OBJECT_NUM = 2;

	//Object3d object3ds[OBJECT_NUM];

	// スプライト共通テクスチャ読み込み
	SpriteBase::GetInstance()->LoadTexture(1, L"Resources/GameClear.png");

	// スプライトの生成
	sprite.reset(Sprite::Create(1, XMFLOAT3(0, 0, 0), { 0,0 }, { 1, 1, 1, 1 }, { 0, 0 }, false, false));
	//for (int i = 0; i < 1; i++)
	//{
	//    int texNumber = 1;
	//    Sprite* sprite = Sprite::Create(spriteBase, texNumber, { 0,0 }, false, false);

	//    // スプライトの座標変更
	//    sprite->SetPosition({ (float)(80),(float)(20),0 });
	//    //sprite->SetRotation((float)(rand() % 360));
	//    sprite->SetSize({ (float)(200), (float)(200) });

	//    sprite->TransferVertexBuffer();

	//    sprites.push_back(sprite);ClearScene
	//}

#pragma endregion 描画初期化処理
}

void ClearScene::Finalize()
{

	//スプライト解放
	//delete sprite;

	//3dオブジェクト解放
	//delete object3d_1;
	//delete object3d_2;
	//delete object3d_3;
	//delete model_1;
	//delete model_2;
}

void ClearScene::Update()
{
	Input* input = Input::GetInstance();
	ComplexInput* cInput = ComplexInput::GetInstance();

	if ((cInput->DecisionByEnter()))     // エンターキーが押されていたら
	{
		GameSound::GetInstance()->PlayWave("personalgame_decision.wav", 0.2f);
		//振動
		input->PadVibration();
		// 音声停止
		GameSound::GetInstance()->SoundStop("verROOP_tukawanakutemoiiYO.wav");
		//シーン切り替え
		BaseScene* scene = new TitleScene();
		sceneManager_->SetNextScene(scene);
	}

	DrawUI();

	//スプライト更新
	sprite->Update();
}

void ClearScene::Draw()
{
	//// スプライト共通コマンド
	SpriteBase::GetInstance()->PreDraw();
	//SpriteCommonBeginDraw(spriteBase, dxBase->GetCmdList());
	//// スプライト描画
	sprite->Draw();

	////3dオブジェ描画前処理
	//Object3d::PreDraw();

	////3dオブジェ描画
	//object3d_1->Draw();

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
	//SpriteBase(spriteBase, dxBase->GetCmdList());
	//// スプライト描画
	//sprite->Draw();
}

void ClearScene::DrawUI()
{
	//DebugText::GetInstance()->Print("[ENTERorPAD_A]:PLAYSCENE", 410, 100, 3.0f);

	//時間
	{
		Timer* timer = Timer::GetInstance();
		char tmp[32]{};
		sprintf_s(tmp, 32, "ClearTime : %2.f", timer->time);
		DebugText::GetInstance()->Print(tmp, 550, 430, 1);
	}

}
