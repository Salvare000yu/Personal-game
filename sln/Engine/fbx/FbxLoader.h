﻿#pragma once

#include "fbxsdk.h"

#include <d3d12.h>
#include <d3dx12.h>

#include <string>

#include "FbxModel.h"

class FbxLoader
{
private:
	//std省略
	using string = std::string;
	using Node = FBX::Node;

public:
	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static FbxLoader* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device"></param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// 後始末
	/// </summary>
	void Finalize();

	/// <summary>
	/// ファイルからFBXモデル読み込み
	/// </summary>
	//void LoadModelFromFile(const string& modelName);
	FbxModel* LoadModelFromFile(const string& modelName);

	/// <summary>
	/// 再帰的にノード構成を解析
	/// </summary>　
	void ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode, Node* parent = nullptr);

	/// <summary>
	/// メッシュ読み取り
	/// </summary>
	void ParseMesh(FbxModel* fbxModel, FbxNode* fbxNode);

	//頂点座標読み取り
	void ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh);
	//面積情報読み取り
	void ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh);
	//マテリアル読み取り
	void ParseMaterial(FbxModel* fbxModel, FbxNode* fbxNode);
	//テクスチャ読み取り
	void LoadTexture(FbxModel* fbxModel, const std::string& fullpath);

	//ディレクトリを含んだファイルパスからファイル名を抽出
	std::string ExtractFileName(const std::string& path);

	/// <summary>
	/// FBXの行列をXMMatrixに変換
	/// </summary>
	/// <param name="dst">書き込み先</param>
	/// <param name="src">元となるFBX行列</param>
	static void ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src);

	/// <summary>
	/// スキニング情報読み取り
	/// </summary>
	/// <param name="fbxModel">モデル</param>
	/// <param name="fbxMesh">メッシュ</param>
	void ParseSkin(FbxModel* fbxModel, FbxMesh* fbxMesh);

private:
	// privateなコンストラクタ（シングルトンパターン）
	FbxLoader() = default;
	// privateなデストラクタ（シングルトンパターン）
	~FbxLoader() = default;
	// コピーコンストラクタを禁止（シングルトンパターン）
	FbxLoader(const FbxLoader& obj) = delete;
	// コピー代入演算子を禁止（シングルトンパターン）
	void operator=(const FbxLoader& obj) = delete;

	//d3d12デバイス
	ID3D12Device* device = nullptr;
	//fbxマネージャー
	FbxManager* fbxManager = nullptr;
	//fbxインポータ
	FbxImporter* fbxImporter = nullptr;

public:

	//モデル格納ルートパス
	static const string baseDirectory;
	//テクスチャがない場合のテクスチャファイル名
	static const string defaultTextureFileName;
};