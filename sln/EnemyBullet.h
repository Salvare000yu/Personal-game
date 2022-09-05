#pragma once
#include "BaseScene.h"
#include "Object3d.h"
#include "Camera.h"
#include "BaseObject.h"

#include <memory>

class EnemyBullet:public BaseObject
{

private:
	////-----------------model
	//std::unique_ptr < Model> mod_classplayer = nullptr;//自機

	////-----------------obj
	//std::unique_ptr < Object3d> obj_classplayer = nullptr;//自機

	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

	std::unique_ptr < Model> mod_enemybullet = nullptr;

	float frame = 0;
public:
	//初期化
	void Initialize()override;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	static EnemyBullet* GetInstance();

	std::unique_ptr<Camera> camera; //カメラ

	float time;

	//消えるまで
	static const int32_t BulVanishTime = 60 * 5;//寿命

	//消滅フラグ
	bool isVanish_ = FALSE;

	//消滅の宣告
	int32_t vanishTimer_ = BulVanishTime;

	bool IsVanish()const { return isVanish_; }

	//当たった時呼び出すためのコールバック関数
	void OnCollision();
	//雑魚敵座標判定で使うための
	static XMFLOAT3 EnemyBullet::GetEnemyBulPosMemory();
	//静的メンバ変数　当たり判定で使う為の雑魚敵座標
	static XMFLOAT3 EnemyBulPosMemory;

};
