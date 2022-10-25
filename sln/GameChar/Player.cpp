#include "Player.h"
//#include "Object3d.h"
#include "Input.h"
#include "GameSound.h"

#include <DirectXMath.h>

using namespace DirectX;

void Player::Attack()
{
	//キー入力使う
	Input* input = Input::GetInstance();

	//triggerkey
	const bool TriggerSPACE = input->TriggerKey(DIK_SPACE);
	//パッド押した瞬間のみ
	const bool PadTriggerRB = input->TriggerButton(static_cast<int>(Button::RB));

	const bool TriggerMouseLEFT = input->TriggerMouse(0);

	//弾発射
	if ((TriggerSPACE || PadTriggerRB || TriggerMouseLEFT) && AttackIntervalFlag == false) {

		XMFLOAT3 PlayerPos = obj->GetPosition();
		//弾生成
		std::unique_ptr<PlayerBullet> madeBullet = std::make_unique<PlayerBullet>();
		//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
		madeBullet->Initialize();
		madeBullet->SetModel(pBulModel);
		madeBullet->SetPosition(PlayerPos);

		// velocityを算出 弾発射速度z
		DirectX::XMVECTOR vecvelocity = XMVectorSet(0, 0, 15, 0);
		XMFLOAT3 xmfloat3velocity;
		XMStoreFloat3(&xmfloat3velocity, XMVector3Transform(vecvelocity, obj->GetMatRot()));

		madeBullet->SetVelocity(xmfloat3velocity);

		// 音声再生 鳴らしたいとき
		GameSound::GetInstance()->PlayWave("shot.wav", 0.1f);

		//弾登録
		bullets_.push_back(std::move(madeBullet));

		//input->PadVibrationDef();

		AttackIntervalFlag = true;

	}
	if (AttackIntervalFlag == true)
	{

		if (--AtkInterval_ >= 0) {//クールタイム 0まで減らす	

			//XMFLOAT3 PlayerPos = obj->GetPosition();
			////弾生成
			//std::unique_ptr<PlayerBullet> madeBullet = std::make_unique<PlayerBullet>();
			////bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
			//madeBullet->Initialize();
			//madeBullet->SetModel(pBulModel);
			//madeBullet->SetPosition(PlayerPos);

			//// velocityを算出
			//XMVECTOR vecvelocity = XMVectorSet(0, 0, 3, 0);
			//XMFLOAT3 xmfloat3velocity;
			//XMStoreFloat3(&xmfloat3velocity, XMVector3Transform(vecvelocity, obj->GetMatRot()));

			//madeBullet->SetVelocity(xmfloat3velocity);

			//// 音声再生 鳴らしたいとき
			//GameSound::GetInstance()->PlayWave("shot.wav", 0.3f);

			////弾登録
			//bullets_.push_back(std::move(madeBullet));

			if (AtkInterval_ <= 0) {
				AttackIntervalFlag = false;
			}//0なったらくらい状態解除
		}
		else { AtkInterval_ = AtkInterval; }
	}

}

void Player::FiringLine()
{

	//いろいろ生成
	firingline_.reset(new PlayerFireLine());
	//いろいろキャラ初期化
	firingline_ = std::make_unique<PlayerFireLine>();
	firingline_->Initialize();
	firingline_->SetModel(pFiringLine);

	XMFLOAT3 PlayerPos = obj->GetPosition();
	firingline_->SetPosition(PlayerPos);

	XMFLOAT3 PlayerRot = obj->GetRotation();
	firingline_->SetRotation(PlayerRot);

	firingline_->SetScale({ 0.5f,0.5f,10.f });
}

void Player::Initialize()
{
	//定義とか仮おいておこう

	//作る
	obj.reset(Object3d::Create());
	//-----↓任意↓-----//
	//大きさ
	obj->SetScale({ 3.0f, 3.0f, 3.0f });
	//場所
	obj->SetPosition({ 0,40,-170 });

	// 音声読み込み
	GameSound::GetInstance()->LoadWave("shot.wav");
}

void Player::Update()
{
	Input* input = Input::GetInstance();
	const bool inputW = input->PushKey(DIK_W);
	const bool inputS = input->PushKey(DIK_S);
	const bool inputA = input->PushKey(DIK_A);
	const bool inputD = input->PushKey(DIK_D);
	//const bool inputE = input->PushKey(DIK_E);
	const bool inputQ = input->PushKey(DIK_Q);
	const bool inputZ = input->PushKey(DIK_Z);
	//const bool inputC = input->PushKey(DIK_C);
	const bool inputI = input->PushKey(DIK_I);
	const bool inputJ = input->PushKey(DIK_J);
	const bool inputK = input->PushKey(DIK_K);
	const bool inputL = input->PushKey(DIK_L);
	const bool inputUp = input->PushKey(DIK_UP);
	const bool inputDown = input->PushKey(DIK_DOWN);
	const bool inputRight = input->PushKey(DIK_RIGHT);
	const bool inputLeft = input->PushKey(DIK_LEFT);
	//パッド押している間
	const bool PadInputUP = input->PushButton(static_cast<int>(Button::UP));
	const bool PadInputDOWN = input->PushButton(static_cast<int>(Button::DOWN));
	const bool PadInputLEFT = input->PushButton(static_cast<int>(Button::LEFT));
	const bool PadInputRIGHT = input->PushButton(static_cast<int>(Button::RIGHT));

	const bool TriggerR = input->TriggerKey(DIK_R);

	//消滅フラグ立ったらその弾は死して拝せよ
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return !bullet->GetAlive();
		});

	XMFLOAT3 rotation = obj->GetRotation();
	//XMFLOAT3 position = obj->GetPosition();

	//if (TriggerR) {//リセット
	//	obj->SetPosition({ 0,40,-170 });
	//	obj->SetRotation({ 0,0,0 });
	//}

	//if (inputUp || inputDown || inputRight || inputLeft)
	//{
	//	XMFLOAT3 Rot = obj->GetRotation();
	//	if (inputUp) {
	//		Rot.x -= 1;
	//	}
	//	if (inputDown) {
	//		Rot.x += 1;
	//	}
	//	if (inputRight) {
	//		Rot.y += 1;
	//	}
	//	if (inputLeft) {
	//		Rot.y -= 1;
	//	}
	//	obj->SetRotation(Rot);
	//}

	//------------------↑プレイヤー移動＆姿勢

	//自分回転
	if (inputL)
	{
		//XMFLOAT3 rotation = obj->GetRotation();
		rotation.y++;
		obj->SetRotation(rotation);
	}
	if (inputJ)
	{
		//XMFLOAT3 rotation = obj->GetRotation();
		rotation.y--;
		obj->SetRotation(rotation);
	}
	if (inputI)
	{
		//XMFLOAT3 rotation = obj->GetRotation();
		rotation.x--;
		obj->SetRotation(rotation);
	}
	if (inputK)
	{
		//XMFLOAT3 rotation = obj->GetRotation();
		rotation.x++;
		obj->SetRotation(rotation);
	}

	//発射処理
	if (alive) { Attack(); }
	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	FiringLine();

	firingline_->Update();

	obj->Update();

}

void Player::Draw()
{
	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw();
	}

	if (alive)
	{
		obj->Draw();
	}

	firingline_->Draw();
}
