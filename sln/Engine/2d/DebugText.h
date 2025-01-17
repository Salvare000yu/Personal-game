#pragma once

#include "Sprite.h"
#include<Windows.h>
#include <string>

// デバッグ文字列クラスの定義
class DebugText
{
public: // 定数の宣言
	static const int maxCharCount = 256;    // 最大文字数
	static const int fontWidth = 9;         // フォント画像内1文字分の横幅
	static const int fontHeight = 18;       // フォント画像内1文字分の縦幅
	static const int fontLineCount = 14;    // フォント画像内1行分の文字数

	int formatPrint(const SpriteBase& spriteBase, const float x, const float y, const float scale, DirectX::XMFLOAT4 color, const char* fmt, ...);

public: // メンバ関数
	static DebugText* GetInstance();

	void Initialize(SpriteBase* spriteBase, UINT texnumber);

	void Finalize();

	void Print(const std::string& text, float x, float y, float scale = 1.0f);

	void DrawAll();

private: // メンバ変数
	//スプライト共通　借りてくる
	SpriteBase* spriteBase_ = nullptr;
	// スプライトデータの配列
	Sprite* sprites_[maxCharCount];
	// スプライトデータ配列の添え字番号
	int spriteIndex_ = 0;
};