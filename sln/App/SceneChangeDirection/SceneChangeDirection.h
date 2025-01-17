﻿/// <<<<<<<<
/// @brief シーン遷移時演出のクラス
/// >>>>>>>>

#pragma once
#include "Sprite.h"
#include "BaseObject.h"
#include "Object3d.h"
#include "SpriteBase.h"
#include "DxBase.h"

class SceneChangeDirection
{
public:
	static SceneChangeDirection* GetInstance();
	void Initialize();

	void HideTheScreen();

	void OpenTheScreen();

	void Update();

	void Draw();

	//シーン遷移フラグ　false:してない
	bool sceneChangeDirectionFlag = false;
	//シーン遷移完了　false:完了前
	bool sceneChangeCompFlag = false;
	//画面隠す　false:隠し始める前
	bool hideTheScreenFlag = false;
	//playsceneが始まり演出始める　false:まだ
	bool gameReadyStartFlag = false;
	//開き切った　false:開く前
	bool openTheScreenFlag = false;

private:

	std::unique_ptr < Sprite> sp_scenechange = nullptr;

	const float hideSp = 80;//画面隠す速度 画像サイズで割れる大きさ

	float hideVel = 0;//実際に座標に足す値
};