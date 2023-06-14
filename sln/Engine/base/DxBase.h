#pragma once
//directXcommon
#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include "WinApp.h"
#include <chrono>

#include <DirectXMath.h>

//directX���
class DxBase
{
	DxBase(const DxBase& dxBase) = delete;
	DxBase& operator=(const DxBase& dxBase) = delete;

	DxBase() {};

public://�����o�֐�
	static DxBase* GetInstance();

	void Initialize(WinApp* winApp);

	//�`��O
	void PreDraw();
	//�`��㏈��
	void PostDraw();

	//�f�o�C�X�擾
	ID3D12Device* GetDevice() { return device.Get(); }

	//�R�}���h���X�g�擾
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }

	inline float GetFps()const { return fps; }

private:
	//�f�o�C�X
	//Microsoft::WRL::ComPtr<ID3D12Device>dev;
	//DXGI�t�@�N�g��
	Microsoft::WRL::ComPtr<IDXGIFactory6>dxgiFactory;

	WinApp* winApp = nullptr;

	//direct3D�n�����o�ϐ�
	Microsoft::WRL::ComPtr<ID3D12Device> device;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> cmdAllocator;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> cmdQueue;
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapchain;
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap;
	Microsoft::WRL::ComPtr<ID3D12Fence> fence;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heapforimgui;
	UINT64 fenceVal = 0;
	//------
	//�o�b�N�o�b�t�@
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>>backBuffers;

	//FPS�Œ� ���ԋL�^
	std::chrono::steady_clock::time_point reference_;

	float fps = -1;
private:
	bool InitializeCommand();

	bool InitializeDevice();

	bool InitializeSwapchain();

	bool InitializeRenderTargetView();

	bool DxBase::InitializeDepthBuffer();

	bool DxBase::InitializeFence();

	bool DebugLayer();

	bool SetBreakOnSeverity();
	//----//FPS�Œ�
	void InitFixFps();//������
	void UpdateFixFps();//�X�V
	//-----
};