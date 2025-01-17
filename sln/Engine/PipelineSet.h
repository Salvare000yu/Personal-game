#pragma once
#include <wrl.h>
#include <d3d12.h>

/// <summary>
/// パイプラインセット
/// </summary>

// パイプラインセット
struct PipelineSet
{
	// パイプラインステートオブジェクト
	Microsoft::WRL::ComPtr<ID3D12PipelineState> pipelinestate;
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootsignature;
};
