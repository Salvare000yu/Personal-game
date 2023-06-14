#include "SceneManager.h"

#include "PostEffect.h"
#include "DxBase.h"
#include "DebugText.h"

SceneManager::~SceneManager()
{
	//�Ō�̃V�[���I���Ɖ��
	scene_->Finalize();
	delete scene_;
}

void SceneManager::Update()
{
	//�V�[���؂�ւ���������
	if (nextScene_) {
		if (scene_) {
			//���V�[���I��
			scene_->Finalize();
			delete scene_;
		}

		scene_ = nextScene_;
		nextScene_ = nullptr;

		//�V�[���}�l�[�W���Z�b�g
		scene_->SetSceneManager(this);

		//�V�V�[���̏�����
		scene_->Initialize();
	}
	scene_->Update();
}

void SceneManager::Draw()
{
	PostEffect::GetInstance()->PreDrawScene(DxBase::GetInstance()->GetCmdList());
	scene_->Draw();
	PostEffect::GetInstance()->PostDrawScene(DxBase::GetInstance()->GetCmdList());
}