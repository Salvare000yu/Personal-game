﻿#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "BaseObject.h"
#include "SmallEnemyBullet.h"

#include <functional>
#include <forward_list>

#include <memory>

class SmallEnemy :public BaseObject
{
private:
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:

	void Attack();

	//初期化
	void Initialize()override;

	void Shot();//弾発射

	//登場開始
	void StartAppear();

	void Exit();//消える

	void RetireRight();//右
	void RetireLeft();//左

	void BulletUpdate();

	//更新
	void Update()override;

	//描画
	void Draw()override;

	std::unique_ptr<Camera> camera; //カメラ

	std::forward_list <std::unique_ptr<SmallEnemyBullet>> bullets_;//プレイヤーの弾　ユニークポインタ

	//-----------------↓げったーせったー↓------------------//
	//弾リストを取得
	const std::forward_list<std::unique_ptr<SmallEnemyBullet>>& GetBullets() { return bullets_; }

	inline void SetSEBulModel(Model* model) { seBulModel = model; }

	//通常弾威力
	void SetBulPow(int32_t seBulPower) { this->seBulPower = seBulPower; }
	const int32_t& GetBulPow() { return seBulPower; }

	inline void SetShotTag(BaseObject* shotTag) { this->shotTag = shotTag; }
	//-----------------↑げったーせったー↑------------------//

private:

	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	//フレームごとに発射
	static const uint8_t atkInterval = 90;

	//SEneXの乱数入れる変数
	float sEneRandX = 0;

	//攻撃用カウント
	uint8_t atkCount = 0;
	Model* seBulModel = nullptr;

	//通常弾威力
	const int32_t seBulPowerMax = 100;
	int32_t seBulPower = seBulPowerMax;

	BaseObject* shotTag;//弾うつターゲット

	//ここまで来たら止まって捌ける
	const int posZMax = 420;

	//はける速度
	const float retireSp = 3.f;
	const float rotSp = 1.f;//傾け速度
	const float rotMax = 10.f;//どこまで傾けるか

	//左右に捌ける
	static const int32_t retireFrameDef = 120;
	int32_t retireFrame = retireFrameDef;

	//雑魚敵行動パターン
	std::function<void()>smallEnemyActionPattern;
};