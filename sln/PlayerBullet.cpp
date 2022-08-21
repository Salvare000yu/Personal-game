#include "PlayerBullet.h"
#include "Player.h"
#include "Object3d.h"
#include "Input.h"

#include <DirectXMath.h>

PlayerBullet* PlayerBullet::GetInstance()
{
	static PlayerBullet instance;

	return &instance;
}

//bullet��initialize��pos����Ă��̎��̃v���C���[pos�ɕ\������悤�ɂ���
void PlayerBullet::Initialize(DirectX::XMFLOAT3 position)
{
	//��`�Ƃ��������Ă�����

	//���ł�ǂݍ���
	mod_playerbullet.reset(Model::LoadFromOBJ("PlayerBul"));
	//���
	obj_playerbullet.reset(Object3d::Create());
	//�Z�b�g
	obj_playerbullet->SetModel(mod_playerbullet.get());
	//-----���C�Ӂ�-----//
	//�傫��
	obj_playerbullet->SetScale({ 1.5f, 1.5f, 1.5f });
	//�ꏊ
	obj_playerbullet->SetPosition({ position });

}

void PlayerBullet::Update()
{

	Input* input = Input::GetInstance();

	const bool inputW = input->PushKey(DIK_W);
	const bool inputS = input->PushKey(DIK_S);
	const bool inputA = input->PushKey(DIK_A);
	const bool inputD = input->PushKey(DIK_D);
	const bool inputE = input->PushKey(DIK_E);
	const bool inputQ = input->PushKey(DIK_Q);
	const bool inputZ = input->PushKey(DIK_Z);

	const bool TriggerR = input->TriggerKey(DIK_R);

	XMFLOAT3 position = obj_playerbullet->GetPosition();
	position.z = position.z + 3;
	position.y = position.y + 0.3f;
	obj_playerbullet->SetPosition(position);

	//if (TriggerR) {//���Z�b�g
	//	obj_playerbullet->SetPosition({ 0,40,-170 });
	//}

	//���Ԍo�ߏ���
	if (--vanishTimer_ <= 0) { isVanish_ = TRUE; }

	obj_playerbullet->Update();
}

void PlayerBullet::Draw()
{
	obj_playerbullet->Draw();
}