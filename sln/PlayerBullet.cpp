#include "PlayerBullet.h"
#include "Input.h"

#include <DirectXMath.h>

//bulletのinitializeにpos入れてその時のプレイヤーposに表示するようにする
void PlayerBullet::Initialize()
{
	//定義とか仮おいておこう

	//もでる読み込み
	mod_playerbullet.reset(Model::LoadFromOBJ("PlayerBul"));
	//作る
	obj.reset(Object3d::Create());
	//セット
	obj->SetModel(mod_playerbullet.get());
	//-----↓任意↓-----//
	//大きさ
	obj->SetScale({ 1.5f, 1.5f, 1.5f });
	//場所

	//obj->SetPosition({ BulletPos });

}

void PlayerBullet::Update()
{
	XMFLOAT3 BulletPos = obj->GetPosition();
	BulletPos.z += velocity.z;
	BulletPos.x += velocity.x;
	BulletPos.y += velocity.y;

	obj->SetPosition(BulletPos);

	//if (TriggerR) {//リセット
	//	obj->SetPosition({ 0,40,-170 });
	//}

	//時間経過消滅
	//if (--vanishTimer_ <= 0) { isVanish_ = TRUE; }

	obj->Update();
}

void PlayerBullet::Draw()
{
	obj->Draw();
}
