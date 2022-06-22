/*****************************************************************//**
 * \file   main.cpp
 * \brief  Sculpto sandbox program entry point.
 * 
 * \author Sabitov Kirill
 * \date   18 June 2022
 *********************************************************************/

 #include "sandbox_app.h"
 #include <numeric>

scl::application *scl::CreateApplication()
{
    return new sandbox_app();
}

ID3D12Device5Ptr CreateDevice(IDXGIFactory4Ptr Factory)
{
    IDXGIAdapter1Ptr Adapter;
    for (u32 i = 0; Factory->EnumAdapters1(i, &Adapter) != DXGI_ERROR_NOT_FOUND; i++)
    {
        DXGI_ADAPTER_DESC1 desc;
        Adapter->GetDesc1(&desc);
        if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) continue;

        ID3D12Device5Ptr Device;
        D3D_CALL(D3D12CreateDevice(Adapter, D3D_FEATURE_LEVEL_12_0, IID_PPV_ARGS(&Device)));
        if (Device) return Device;

        // :((((
        // D3D12_FEATURE_DATA_D3D12_OPTIONS5 feature5;
        // auto hr = Device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &feature5,
        //                                       sizeof(D3D12_FEATURE_DATA_D3D12_OPTIONS5));
        // if (SUCCEEDED(hr) && feature5.RaytracingTier != D3D12_RAYTRACING_TIER_NOT_SUPPORTED)
        //     return Device;
    }

    MessageBox(nullptr, "DXR is not supported by your GPU.", "Error!", MB_ICONERROR | MB_OK);
    exit(239);
    return nullptr;
}

ID3D12CommandQueuePtr CreateCommandQueue(ID3D12Device5Ptr Device)
{
    D3D12_COMMAND_QUEUE_DESC com_queue_desc {};
    com_queue_desc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    com_queue_desc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

    ID3D12CommandQueuePtr CommandQueue;
    D3D_CALL(Device->CreateCommandQueue(
        &com_queue_desc,
        IID_PPV_ARGS(&CommandQueue)
    ));
    return CommandQueue;
}

ID3D12DescriptorHeapPtr CreateDescriptorHeap(ID3D12Device5Ptr Device, u32 Count,
                                             D3D12_DESCRIPTOR_HEAP_TYPE Type, bool ShaderVisible)
{
    D3D12_DESCRIPTOR_HEAP_DESC desc_heap_desc {};
    desc_heap_desc.NumDescriptors = Count;
    desc_heap_desc.Type = Type;
    desc_heap_desc.Flags = ShaderVisible ?
        D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE :
        D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    ID3D12DescriptorHeapPtr Heap;
    D3D_CALL(Device->CreateDescriptorHeap(
        &desc_heap_desc,
        IID_PPV_ARGS(&Heap)
    ));
    return Heap;
}

D3D12_CPU_DESCRIPTOR_HANDLE CreateRTV(ID3D12Device5Ptr Device,ID3D12ResourcePtr Resource,
                                      ID3D12DescriptorHeapPtr Heap, u64 &UsedHeapEntries, DXGI_FORMAT Format)
{
    D3D12_RENDER_TARGET_VIEW_DESC rtv_desc = {};
    rtv_desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
    rtv_desc.Format = Format;
    rtv_desc.Texture2D.MipSlice = 0;

    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = Heap->GetCPUDescriptorHandleForHeapStart();
    rtvHandle.ptr += UsedHeapEntries * Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    UsedHeapEntries += 1;
    Device->CreateRenderTargetView(Resource, &rtv_desc, rtvHandle);
    return rtvHandle;
}

IDXGISwapChain3Ptr CreateSwapChain(IDXGIFactory4Ptr Factory, ID3D12CommandQueuePtr CommandQueue,
                                   window_handle WindowHandle, DXGI_FORMAT Format, u32 Width, u32 Height,
                                   int SwapChainBuffersCount)
{
    DXGI_SWAP_CHAIN_DESC1 swap_chain_desc {};
    swap_chain_desc.BufferCount = SwapChainBuffersCount;
    swap_chain_desc.Width = Width;
    swap_chain_desc.Height = Height;
    swap_chain_desc.Format = Format;
    swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swap_chain_desc.SampleDesc.Count = 1;

    MAKE_SMART_COM_PTR(IDXGISwapChain1);
    IDXGISwapChain1Ptr tmp_swap_chain;
    auto hr = Factory->CreateSwapChainForHwnd(
        CommandQueue, WindowHandle, &swap_chain_desc,
        nullptr, nullptr, &tmp_swap_chain
    );
    if (FAILED(hr)) D3D_TRACEHR("Failed to create swap chain", hr);

    IDXGISwapChain3Ptr SwapChain;
    D3D_CALL(tmp_swap_chain->QueryInterface(IID_PPV_ARGS(&SwapChain)));
    return SwapChain;
}

void sandbox_app::InitDirectX()
{
    SwapChainSize = uvec2(GetWindowData().Width, GetWindowData().Height);

#ifdef _DEBUG
    // Initializing debug layer
    ID3D12DebugPtr Debug;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&Debug))))
        Debug->EnableDebugLayer();
#endif /* !_DEBUG */

    // Creat DXGI Factory
    IDXGIFactory4Ptr Factory;
    D3D_CALL(CreateDXGIFactory1(IID_PPV_ARGS(&Factory)));
    Device = CreateDevice(Factory);
    CmdQueue = CreateCommandQueue(Device);
    SwapChain = CreateSwapChain(Factory, CmdQueue, GetWindowHandle(),
                                DXGI_FORMAT_R8G8B8A8_UNORM,
                                GetWindowData().Width, GetWindowData().Height,
                                DefaultSwapChainBuffersCount);

    // Create render target view descriptor heap
    RTVHeap.Heap = CreateDescriptorHeap(Device, RTVHeapSize,
                                        D3D12_DESCRIPTOR_HEAP_TYPE_RTV,false);

    // Create per frame objects
    for (u32 i = 0; i < DefaultSwapChainBuffersCount; ++i)
    {
        D3D_CALL(Device->CreateCommandAllocator(
            D3D12_COMMAND_LIST_TYPE_DIRECT,
            IID_PPV_ARGS(&FrameObjects[i].CmdAllocator)
        ));
        D3D_CALL(SwapChain->GetBuffer(
            i, IID_PPV_ARGS(&FrameObjects[i].SwapChainBuffer)
        ));
        FrameObjects[i].RTVHandle =
            CreateRTV(Device, FrameObjects[i].SwapChainBuffer,
                      RTVHeap.Heap, RTVHeap.UsedEntries,
                      DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
    }

    // Create command list
    D3D_CALL(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, FrameObjects[0].CmdAllocator,
                                       nullptr, IID_PPV_ARGS(&CmdList)));

    // Create fence, per-frame sync event
    D3D_CALL(Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence)));
    FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
}

void TransitionBarier(ID3D12GraphicsCommandList4Ptr CmdList, ID3D12ResourcePtr Resource,
                      D3D12_RESOURCE_STATES StateBefore, D3D12_RESOURCE_STATES StateAfter)
{
    D3D12_RESOURCE_BARRIER barriere {};
    barriere.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barriere.Transition.pResource = Resource;
    barriere.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    barriere.Transition.StateBefore = StateBefore;
    barriere.Transition.StateAfter = StateAfter;

    CmdList->ResourceBarrier(1, &barriere);
}

void SubmitCommandList(ID3D12GraphicsCommandList4Ptr CmdList, ID3D12CommandQueuePtr CmdQueue,
                      ID3D12FencePtr Fence, u64 &FenceValue)
{
    CmdList->Close();

    ID3D12CommandList* GraphicsList = CmdList.GetInterfacePtr();
    CmdQueue->ExecuteCommandLists(1, &GraphicsList);

    FenceValue += 1;
    CmdQueue->Signal(Fence, FenceValue);
}

u32 sandbox_app::BeginFrame()
{
    return SwapChain->GetCurrentBackBufferIndex();
}

void sandbox_app::EndFrame(u32 RTVIndex)
{
    // Present current render target
    TransitionBarier(CmdList, this->FrameObjects[RTVIndex].SwapChainBuffer,
                     D3D12_RESOURCE_STATE_RENDER_TARGET,
                     D3D12_RESOURCE_STATE_PRESENT);
    SubmitCommandList(CmdList, CmdQueue, Fence, FenceValue);
    SwapChain->Present(0, 0);

    // Buffers synchronisation
    if (FenceValue > DefaultSwapChainBuffersCount)
    {
        Fence->SetEventOnCompletion(FenceValue - DefaultSwapChainBuffersCount + 1,
                                    FenceEvent);
        WaitForSingleObject(FenceEvent, INFINITE);
    }

    // Prepare command list for next frame
    u32 buffer_index = SwapChain->GetCurrentBackBufferIndex();
    FrameObjects[buffer_index].CmdAllocator->Reset();
    CmdList->Reset(FrameObjects[buffer_index].CmdAllocator, nullptr);
}

void sandbox_app::Init()
{
    InitDirectX();
    std::cout << "Hello!";
}

void sandbox_app::Close()
{
    FenceValue += 1;
    CmdQueue->Signal(Fence, FenceValue);
    Fence->SetEventOnCompletion(FenceValue, FenceEvent);
    WaitForSingleObject(FenceEvent, INFINITE);
}

void sandbox_app::Timer()
{
    u32 RTVIndex = BeginFrame();

    const float clear_color[4] = { 0.102f, 0.30f, 0.47f, 1.0f };
    TransitionBarier(CmdList, FrameObjects[RTVIndex].SwapChainBuffer,
                     D3D12_RESOURCE_STATE_PRESENT,
                     D3D12_RESOURCE_STATE_RENDER_TARGET);
    CmdList->ClearRenderTargetView(FrameObjects[RTVIndex].RTVHandle, clear_color,
                                   0, nullptr);

    EndFrame(RTVIndex);
}

void sandbox_app::Resize()
{

}
