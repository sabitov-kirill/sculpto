/*****************************************************************//**
 * \file   sandbox_app.h
 * \brief  Sandbox application definitions module.
 * 
 * \author Sabitov Kirill
 * \date   19 June 2022
 *********************************************************************/

#include <dxgi1_6.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <d3d12sdklayers.h>
#include <dxgidebug.h>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")

#include <sculpto.h>
using namespace scl;

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

// Common DX12 definitions
#include <comdef.h>
#define MAKE_SMART_COM_PTR(_a) _COM_SMARTPTR_TYPEDEF(_a, __uuidof(_a))
MAKE_SMART_COM_PTR(ID3D12Device5);
MAKE_SMART_COM_PTR(ID3D12GraphicsCommandList4);
MAKE_SMART_COM_PTR(ID3D12CommandQueue);
MAKE_SMART_COM_PTR(IDXGISwapChain3);
MAKE_SMART_COM_PTR(IDXGIFactory4);
MAKE_SMART_COM_PTR(IDXGIAdapter1);
MAKE_SMART_COM_PTR(ID3D12Fence);
MAKE_SMART_COM_PTR(ID3D12CommandAllocator);
MAKE_SMART_COM_PTR(ID3D12Resource);
MAKE_SMART_COM_PTR(ID3D12DescriptorHeap);
MAKE_SMART_COM_PTR(ID3D12Debug);
MAKE_SMART_COM_PTR(ID3D12StateObject);
MAKE_SMART_COM_PTR(ID3D12RootSignature);
MAKE_SMART_COM_PTR(ID3DBlob);

#define D3D_CALL(a) {HRESULT hr_ = a; if(FAILED(hr_)) { D3D_TRACEHR( #a, hr_); }}
void D3D_TRACEHR(const std::string& msg, HRESULT hr)
{
    char hr_msg[512];
    FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, hr, 0, hr_msg, ARRAYSIZE(hr_msg), nullptr);
    std::string error_msg = msg + ".\nError! " + hr_msg;
    MessageBox(nullptr, error_msg.c_str(), "D3D Failed", MB_ICONERROR | MB_OK);
}

class sandbox_app: public scl::application
{
    struct frame_object
    {
        ID3D12CommandAllocatorPtr CmdAllocator {};
        ID3D12ResourcePtr SwapChainBuffer {};
        D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle {};
    };

    struct heap_data
    {
        ID3D12DescriptorHeapPtr Heap {};
        u64 UsedEntries { 0 };
    };

private:
    void InitDirectX();
    u32 BeginFrame();
    void EndFrame(u32 RTVIndex);

    ID3D12Device5Ptr Device {};
    ID3D12CommandQueuePtr CmdQueue {};
    IDXGISwapChain3Ptr SwapChain {};
    uvec2 SwapChainSize {};
    ID3D12GraphicsCommandList4Ptr CmdList {};
    ID3D12FencePtr Fence {};
    HANDLE FenceEvent {};
    u64 FenceValue { 0 };

    static const int DefaultSwapChainBuffersCount = 3;
    frame_object FrameObjects[DefaultSwapChainBuffersCount];

    static const int RTVHeapSize = 3;
    heap_data RTVHeap;

public:
    void Init() override;
    void Close() override;
    void Resize() override;
    void Timer() override;

    sandbox_app() : application("Sadbox") {}
    ~sandbox_app() override {}
};
