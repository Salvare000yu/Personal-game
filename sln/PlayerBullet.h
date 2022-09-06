#pragma once
#include "Object3d.h"
#include "BaseObject.h"

#include <memory>

class PlayerBullet:public BaseObject
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

	float frame = 0;

	//消滅フラグ
	bool isVanish_ = FALSE;

	XMFLOAT3 velocity{ 0,0,3 };

public:

	//初期化
	void Initialize()override;

	//更新
	void Update()override;

	//描画
	void Draw()override;

	std::unique_ptr<Camera> camera; //カメラ

	//消えるまで
	static const int32_t BulVanishTime = 60 * 5;//寿命

	//消滅の宣告
	int32_t vanishTimer_ = BulVanishTime;

	bool IsVanish()const { return isVanish_; }

	//当たった時呼び出すためのコールバック関数
	void OnCollision();

	void SetVelocity(const XMFLOAT3& velocity) { this->velocity = velocity; }

	const XMFLOAT3& GetVelocity() { return velocity; }
		
	std::unique_ptr < Model> mod_playerbullet = nullptr;

};
