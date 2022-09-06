#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "Object3d.h"

#include "Camera.h"

#include "Enemy.h"
#include "Player.h"
#include "SmallEnemy.h"

#include <memory>

class GamePlayScene :public BaseScene
{
public:

	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;
	void DrawUI() override;

	void SmallEnemyAppear();

	void Obj2move();

	std::unique_ptr<Camera> camera; //カメラ

	float time;

	//指定フレームごとに雑魚出現
	static const int SEneAppInterval = 50;

	std::list <std::unique_ptr<SmallEnemy>> smallEnemys_;

	std::list <std::unique_ptr<Enemy>> enemy_;//enemy　ユニークポインタ
	//敵リストを取得
	//const std::list<std::unique_ptr<Enemy>>& GetEnemy() { return enemy_; }

	Player* player_ = nullptr;
	SmallEnemy* sEnemys_ = nullptr;

	//静的メンバ変数取得 当たり判定で弾座標使うために別クラスの数使いたい
	//XMFLOAT3 playerbulposmemory = PlayerBullet::GetPlayerBulPosMemory();
	//静的メンバ変数取得 当たり判定で弾座標使うために別クラスの数使いたい
	//XMFLOAT3 seneposmemory = SmallEnemy::GetSEnePosMemory();

private:

	std::unique_ptr < Sprite> sprite_back = nullptr;
	std::unique_ptr < Sprite> sp_guide = nullptr;

	std::unique_ptr < Model> mod_sword = nullptr;//デバック用キャラ
	std::unique_ptr < Model> model_1 = nullptr;//地面
	std::unique_ptr < Model> mod_worlddome = nullptr;//天球
	std::unique_ptr < Model> mod_kaberight = nullptr;//壁
	std::unique_ptr < Model> mod_kabeleft = nullptr;//壁

	std::unique_ptr < Object3d> obj_sword = nullptr;//デバック用キャラ
	std::unique_ptr < Object3d> object3d_1 = nullptr;
	std::unique_ptr < Object3d> obj_worlddome = nullptr;
	std::unique_ptr < Object3d> obj_kaberight = nullptr;
	std::unique_ptr < Object3d> obj_kabeleft = nullptr;

	//FbxModel* fbxModel_1 = nullptr;
	//FbxObject3d* fbxObject_1=nullptr;

	float frame = 0;

	//雑魚敵出現用カウント
	float SEneAppCount = 0;
};

