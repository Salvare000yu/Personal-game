#include "PlayerFireLine.h"

using namespace DirectX;

void PlayerFireLine::Initialize()
{
	//定義とか仮おいておこう
	//作る
	obj.reset(Object3d::Create());
	//-----↓任意↓-----//
	//大きさ
	obj->SetScale({ 3.f, 3.f, 3.f });
	//場所

	//obj->SetPosition({ BulletPos });
}

void PlayerFireLine::Update()
{
	XMFLOAT3 FireLinePos = obj->GetPosition();

	obj->SetPosition(FireLinePos);

	obj->Update();
}

void PlayerFireLine::Draw()
{
	obj->Draw();
}