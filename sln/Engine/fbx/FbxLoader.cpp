﻿#include "FbxLoader.h"
#include <cassert>

using namespace DirectX;

//静的メンバ変数の実態
const std::string FbxLoader::baseDirectory = "Resources/";

const std::string FbxLoader::defaultTextureFileName =
"white1x1.png";

FbxLoader* FbxLoader::GetInstance()
{
	static FbxLoader instance;
	return &instance;
}

void FbxLoader::Initialize(ID3D12Device* device)
{
	//再初期化チェック
	assert(fbxManager == nullptr);
	//引数からメンバ変数に代入
	this->device = device;
	//fbxマネージャーの生成
	fbxManager = FbxManager::Create();
	//Fbxマネージャの入出力設定
	FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(ios);
	//fbxインポータの生成
	fbxImporter = FbxImporter::Create(fbxManager, "");
}

FbxModel* FbxLoader::LoadModelFromFile(const string& modelName)
{
	//モデルと同じ名前のフォルダから読み込み
	const string directoryPath = baseDirectory +
		modelName + "/";
	//拡張子.fbx追加
	const string fileName = modelName + ".fbx";
	//連結してフルパスを得る
	const string fullpath = directoryPath + fileName;

	//ファイル名指定、FBXファイル読み込み
	if (!fbxImporter->Initialize(fullpath.c_str(),
		-1, fbxManager->GetIOSettings())) {
		assert(0);
	}

	//シーン生成
	FbxScene* fbxScene =
		FbxScene::Create(fbxManager, "fbxScene");

	//ファイルからロードしたFBXの情報をシーンにインポート
	fbxImporter->Import(fbxScene);

	//モデル生成
	FbxModel* fbxModel = new FbxModel();
	fbxModel->name = modelName;
	//fbxノード数取得
	int nodeCount = fbxScene->GetNodeCount();
	//必要分メモリ確保しといてアドレスズレないようにする
	fbxModel->nodes.reserve(nodeCount);
	//ルートノードから順に解析してモデルに流す
	ParseNodeRecursive(fbxModel, fbxScene->GetRootNode());
	////Fbxシーン解放
	//fbxScene->Destroy();
	fbxModel->fbxScene = fbxScene;
	//バッファ生成
	fbxModel->CreateBuffers(device);

	return fbxModel;
}

void FbxLoader::ParseNodeRecursive(FbxModel* fbxModel, FbxNode* fbxNode, Node* parent)
{
	//ノード名取得
	string name = fbxNode->GetName();
	//モデルにノード追加
	fbxModel->nodes.emplace_back();
	Node& node = fbxModel->nodes.back();
	//fbxノードのローカル移動情報
	FbxDouble3 rotation = fbxNode->LclRotation.Get();
	FbxDouble3 scaling = fbxNode->LclScaling.Get();
	FbxDouble3 translation = fbxNode->LclTranslation.Get();
	//形式変換して代入
	node.rotation = { (float)rotation[0],(float)rotation[1],(float)rotation[2],0.0f };
	node.scaling = { (float)scaling[0],(float)scaling[1],(float)scaling[2],0.0f };
	node.translation = { (float)translation[0],(float)translation[1],(float)translation[2],1.0f };
	//回転角をdegreeからラジアンに
	node.rotation.m128_f32[0] = XMConvertToRadians(node.rotation.m128_f32[0]);
	node.rotation.m128_f32[1] = XMConvertToRadians(node.rotation.m128_f32[1]);
	node.rotation.m128_f32[2] = XMConvertToRadians(node.rotation.m128_f32[2]);
	//スケール回転平行移動計算
	XMMATRIX matScaling, matRotation, matTranslation;
	matScaling = XMMatrixScalingFromVector(node.scaling);
	matRotation = XMMatrixRotationRollPitchYawFromVector(node.rotation);
	matTranslation = XMMatrixTranslationFromVector(node.translation);
	//ローカル変形行列計算
	node.transform = XMMatrixIdentity();
	node.transform *= matScaling;//ワールド行列にスケーリング反映
	node.transform *= matRotation;//ワールド行列に回転反映
	node.transform *= matTranslation;//ワールド行列に平行移動反映
	//グローバル変形行列計算
	node.globalTransform = node.transform;
	if (parent) {
		node.parent = parent;
		//親変形を乗算
		node.globalTransform *= parent->globalTransform;
	}
	//FBXノードメッシュ情報解析
	FbxNodeAttribute* fbxNodeAttribute = fbxNode->GetNodeAttribute();

	if (fbxNodeAttribute) {
		if (fbxNodeAttribute->GetAttributeType() == FbxNodeAttribute::eMesh) {
			fbxModel->meshNode = &node;
			ParseMesh(fbxModel, fbxNode);
		}
	}
	//子ノードに対して再帰呼び出し
	for (int i = 0; i < fbxNode->GetChildCount(); i++) {
		ParseNodeRecursive(fbxModel, fbxNode->GetChild(i));
	}
}

void FbxLoader::ParseMesh(FbxModel* fbxModel, FbxNode* fbxNode)
{
	//ノードメッシュ取得
	FbxMesh* fbxMesh = fbxNode->GetMesh();

	//頂点座標読み取り
	ParseMeshVertices(fbxModel, fbxMesh);
	//面を構成するデータ読み取り
	ParseMeshFaces(fbxModel, fbxMesh);
	//マテリアル読み取り
	ParseMaterial(fbxModel, fbxNode);
	//スキニング読み取り
	ParseSkin(fbxModel, fbxMesh);
}

void FbxLoader::ParseMeshFaces(FbxModel* fbxModel, FbxMesh* fbxMesh)
{
	auto& vertices = fbxModel->vertices;
	auto& indices = fbxModel->indices;

	//1ファイルに複数メッシュのモデルは非対応
	assert(indices.size() == 0);
	//面の数
	const int polygonCount = fbxMesh->GetPolygonCount();
	//uvデータの数
	const int textureUVCount = fbxMesh->GetTextureUVCount();
	//UV名リスト
	FbxStringList uvNames;
	fbxMesh->GetUVSetNames(uvNames);
	//面ごとの読み取り
	for (int i = 0; i < polygonCount; i++) {
		//面を構成する頂点数取得
		const int polygonSize = fbxMesh->GetPolygonSize(i);
		assert(polygonSize <= 4);

		//1頂点ずつ処理
		for (int j = 0; j < polygonSize; j++) {
			//fbx頂点配列インデックス
			int index = fbxMesh->GetPolygonVertex(i, j);
			assert(index >= 0);

			//頂点法線読み込み
			FbxModel::VertexPosNormalUvSkin& vertex = vertices[index];
			FbxVector4 normal;
			if (fbxMesh->GetPolygonVertexNormal(i, j, normal))
			{
				vertex.normal.x = (float)normal[0];
				vertex.normal.y = (float)normal[1];
				vertex.normal.z = (float)normal[2];
			}
			//テクスチャUV読み込み
			if (textureUVCount > 0) {
				FbxVector2 uvs;
				bool lUnmappedUV;
				//0番決め打ちで読み込み
				if (fbxMesh->GetPolygonVertexUV(i, j,
					uvNames[0], uvs, lUnmappedUV)) {
					vertex.uv.x = (float)uvs[0];
					vertex.uv.y = (float)uvs[1];
				}
			}
			//インデックス配列に頂点インデックス追加
			//3頂点までなら
			if (j < 3) {
				//1点追加し他の2点と三角形を構成
				indices.push_back(index);
			}
			//4頂点目
			else {
				//3頂点追加時四角形0123のうち230で三角形構成
				int index2 = indices[indices.size() - 1];
				int index3 = index;
				int index0 = indices[indices.size() - 3];
				indices.push_back(index2);
				indices.push_back(index3);
				indices.push_back(index0);
			}
		}
	}
}

void FbxLoader::Finalize()
{
	//各種FBXインスタンス破棄
	fbxImporter->Destroy();
	fbxManager->Destroy();
}

void FbxLoader::LoadTexture(FbxModel* fbxModel, const std::string& fullpath)
{
	HRESULT result = S_FALSE;
	//WICテクスチャのロード
	TexMetadata& metadata = fbxModel->metadata;
	ScratchImage& scratchImg = fbxModel->scratchImg;
	//ユニコード文字列変換
	wchar_t wfilepath[128];
	MultiByteToWideChar(CP_ACP, 0, fullpath.c_str(), -1, wfilepath,
		_countof(wfilepath));
	result = LoadFromWICFile(
		wfilepath, WIC_FLAGS_NONE,
		&metadata, scratchImg);
	if (FAILED(result)) {
		assert(0);
	}
}

void FbxLoader::ParseMaterial(FbxModel* fbxModel, FbxNode* fbxNode)
{
	const int materialCount = fbxNode->GetMaterialCount();
	if (materialCount > 0)
	{
		// 先頭のマテリアルを取得
		FbxSurfaceMaterial* material = fbxNode->GetMaterial(0);
		// テクスチャを読み込んだかどうかを表すフラグ
		bool textureLoaded = false;
		if (material)
		{
			//FbxSurfaceLambertクラスかどうかを調べる
			if (material->GetClassId().Is(FbxSurfaceLambert::ClassId))
			{
				FbxSurfaceLambert* lambert = static_cast<FbxSurfaceLambert*>(material);

				// 環境光係数
				FbxPropertyT<FbxDouble3> ambient = lambert->Ambient;
				fbxModel->ambient.x = (float)ambient.Get()[0];
				fbxModel->ambient.y = (float)ambient.Get()[1];
				fbxModel->ambient.z = (float)ambient.Get()[2];

				// 拡散反射光係数
				FbxPropertyT<FbxDouble3> diffuse = lambert->Diffuse;
				fbxModel->diffuse.x = (float)diffuse.Get()[0];
				fbxModel->diffuse.y = (float)diffuse.Get()[1];
				fbxModel->diffuse.z = (float)diffuse.Get()[2];

				// ディフューズテクスチャを取り出す
				const FbxProperty diffuseProperty =
					material->FindProperty(FbxSurfaceMaterial::sDiffuse);

				if (diffuseProperty.IsValid())
				{
					const FbxFileTexture* texture = diffuseProperty.GetSrcObject<FbxFileTexture>();
					if (texture)
					{
						const char* filepath = texture->GetFileName();
						// ファイルパスからファイル名を抽出
						string path_str(filepath);
						string name = ExtractFileName(path_str);
						// テクスチャ読み込み
						LoadTexture(fbxModel, baseDirectory + fbxModel->name + "/" + name);
						textureLoaded = true;
					}
				}
			}
		}
		// テクスチャが無い場合は白テクスチャを貼る
		if (!textureLoaded)
		{
			LoadTexture(fbxModel, baseDirectory + defaultTextureFileName);
		}
	}
}

void FbxLoader::ParseMeshVertices(FbxModel* fbxModel, FbxMesh* fbxMesh)
{
	auto& vertices = fbxModel->vertices;

	//頂点座標データ数
	const int controlPointsCount =
		fbxMesh->GetControlPointsCount();
	//必要数だけ頂点データ配列確保
	FbxModel::VertexPosNormalUvSkin
		vert{};
	fbxModel->vertices.resize(controlPointsCount,
		vert);

	//FBXメッシュ頂点座標配列取得
	FbxVector4* pCoord =
		fbxMesh->GetControlPoints();

	//fbxメッシュの全頂点座標をモデル内の配列にコピー
	for (int i = 0; i < controlPointsCount; i++) {
		FbxModel::VertexPosNormalUvSkin& vertex =
			vertices[i];
		//座標コピー
		vertex.pos.x = (float)pCoord[i][0];
		vertex.pos.y = (float)pCoord[i][1];
		vertex.pos.z = (float)pCoord[i][2];
	}
}

std::string FbxLoader::ExtractFileName(const std::string& path)
{
	size_t pos1;
	//区切り文字　'\\' 最後の部分検索
	pos1 = path.rfind('\\');
	if (pos1 != string::npos) {
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}
	//区切り文字'/'最後の部分検索
	pos1 = path.rfind('/');
	if (pos1 != string::npos) {
		return path.substr(pos1 + 1, path.size() - pos1 - 1);
	}
	return path;
}

void FbxLoader::ConvertMatrixFromFbx(DirectX::XMMATRIX* dst, const FbxAMatrix& src)
{
	//行
	for (int i = 0; i < 4; i++)
	{
		//列
		for (int j = 0; j < 4; j++)
		{
			//1要素コピー
			dst->r[i].m128_f32[j] = (float)src.Get(i, j);
		}
	}
}

void FbxLoader::ParseSkin(FbxModel* fbxModel, FbxMesh* fbxMesh)
{
	// スキニング情報
	FbxSkin* fbxSkin =
		static_cast<FbxSkin*>(fbxMesh->GetDeformer(0, FbxDeformer::eSkin));

	// スキニング情報(情報がなければ終了)
	if (fbxSkin == nullptr)
	{
		// 各頂点について処理
		for (int i = 0; i < fbxModel->vertices.size(); i++)
		{
			// 最初のボーン(単位行列)の影響を100%にする
			fbxModel->vertices[i].boneIndex[0] = 0;
			fbxModel->vertices[i].boneWeight[0] = 1.0f;
		}
		return;
	}

	// ボーン配列の参照
	std::vector<FbxModel::Bone>& bones = fbxModel->bones;

	// ボーンの数
	int clusterCount = fbxSkin->GetClusterCount();
	bones.reserve(clusterCount);

	// 全てのボーンについて
	for (int i = 0; i < clusterCount; i++)
	{
		// FBXボーン情報
		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);
		// ボーン自体のノードの名前を取得
		const char* boneName = fbxCluster->GetLink()->GetName();

		// 新しくボーンを追加し、追加したボーンの情報を得る
		bones.emplace_back(FbxModel::Bone(boneName));
		FbxModel::Bone& bone = bones.back();
		// 自作ボーンとFBXボーンを紐づける
		bone.fbxCluster = fbxCluster;

		// FBXから初期姿勢行列を取得する
		FbxAMatrix fbxmat;
		fbxCluster->GetTransformLinkMatrix(fbxmat);

		// XMMatrix型に変換する
		XMMATRIX initialPose;
		ConvertMatrixFromFbx(&initialPose, fbxmat);

		// 初期姿勢行列の逆行列を得る
		bone.invInitialPose = XMMatrixInverse(nullptr, initialPose);
	}

	// ボーン番号とスキンウェイトのペア
	struct WeightSet
	{
		UINT index;
		float weight;
	};

	// 二次元配列(ジャグ配列)
	// list:頂点が影響を受けるボーンの全リスト
	// vector:それを全頂点分
	std::vector<std::list<WeightSet>>
		weightLists(fbxModel->vertices.size());

	// 全てのボーンについて
	for (int i = 0; i < clusterCount; i++)
	{
		// FBXボーン情報
		FbxCluster* fbxCluster = fbxSkin->GetCluster(i);
		// このボーンに影響を受ける頂点の数
		int controlPointIndicesCount = fbxCluster->GetControlPointIndicesCount();
		// このボーンに影響を受ける頂点の配列
		int* controlPointIndices = fbxCluster->GetControlPointIndices();
		double* controlPointWeights = fbxCluster->GetControlPointWeights();

		// 影響を受ける全頂点について
		for (int j = 0; j < controlPointIndicesCount; j++)
		{
			// 頂点番号
			int vertIndex = controlPointIndices[j];
			// スキンウェイト
			float weight = (float)controlPointWeights[j];
			// その頂点の影響を受けるボーンリストに、ボーンとウェイトのペアを追加
			weightLists[vertIndex].emplace_back(WeightSet{ (UINT)i, weight });
		}
	}
	// 頂点配列書き換え用の参照
	auto& vertices = fbxModel->vertices;
	// 各頂点について処理
	for (int i = 0; i < vertices.size(); i++)
	{
		// 頂点のウェイトから最も大きい4つを選択
		auto& weightList = weightLists[i];
		// 大小比較用のラムダ式を指定して降順にソート
		weightList.sort(
			[](auto const& lhs, auto const& rhs) {
				// 左の要素数の方が大きければtrue それでなければfalseを返す
				return lhs.weight > rhs.weight;
			});

		int weightArrayIndex = 0;
		// 降順ソート済みのウェイトリストから
		for (auto& weightSet : weightList)
		{
			// 頂点データに書き込み
			vertices[i].boneIndex[weightArrayIndex] = weightSet.index;
			vertices[i].boneWeight[weightArrayIndex] = weightSet.weight;
			// 4つに達したら終了
			if (++weightArrayIndex >= FbxModel::MAX_BONE_INDICES) {
				float weight = 0.0f;
				// 2番目以降のウェイトを計算
				for (int j = 1; j < FbxModel::MAX_BONE_INDICES; j++)
				{
					weight += vertices[i].boneWeight[j];
				}

				// 合計で1.0f(100%)になるように調整
				vertices[i].boneWeight[0] = 1.0f - weight;
				break;
			}
		}
	}
}