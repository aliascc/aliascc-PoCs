/*
* Copyright (c) 2016 <Carlos Chacón>
* All rights reserved
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
* http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*/

/**********************
*   System Includes   *
***********************/
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>
#include <tchar.h>
#include <Windows.h>

/********************
*   Game Includes   *
*********************/
#include "simplegpusim.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_dx11.h"

/******************
*   Extern Defs   *
*******************/
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

/**************
*   Globals   *
***************/
static SimpleGPUSim* g_SimpleGPUSim = nullptr;

/********************
*   Function Defs   *
*********************/
SimpleGPUSim::SimpleGPUSim()
{
    g_SimpleGPUSim = this;
}

SimpleGPUSim::~SimpleGPUSim()
{
    CleanUp();
}

void SimpleGPUSim::CleanUp()
{
    if (m_IsReady)
    {
        ImGui_ImplDX11_Shutdown();
        ImGui::DestroyContext();
        m_IsReady = false;
    }

    CleanupSimulationRenderTarget();

    CleanupDeviceD3D();

    UnregisterClass(L"Simple GPU Simulation", m_WC.hInstance);
}

void SimpleGPUSim::CleanupDeviceD3D()
{
    CleanupRenderTarget();

    if (m_SwapChain != nullptr)
    {
        m_SwapChain->Release();
        m_SwapChain = nullptr;
    }

    if (m_D3dDeviceContext != nullptr)
    {
        m_D3dDeviceContext->Release();
        m_D3dDeviceContext = nullptr;
    }

    if (m_D3dDevice != nullptr)
    {
        m_D3dDevice->Release();
        m_D3dDevice = nullptr;
    }
}

void SimpleGPUSim::CreateRenderTarget()
{
    ID3D11Texture2D* backBuffer = nullptr;
    m_SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer);
    m_D3dDevice->CreateRenderTargetView(backBuffer, nullptr, &m_MainRenderTargetView);
    backBuffer->Release();
}

void SimpleGPUSim::CleanupRenderTarget()
{
    if (m_MainRenderTargetView == nullptr)
    {
        return;
    }

    m_MainRenderTargetView->Release();
    m_MainRenderTargetView = nullptr;
}

bool SimpleGPUSim::CreateSimulationRenderTarget()
{
    m_SimulationBuffer = new uint32_t[m_SimulationRenderSize.y * m_SimulationRenderSize.x];

    D3D11_TEXTURE2D_DESC desc;
    desc.ArraySize          = 1;
    desc.Height             = m_SimulationRenderSize.y;
    desc.Width              = m_SimulationRenderSize.x;
    desc.MipLevels          = 1;
    desc.BindFlags          = D3D11_BIND_SHADER_RESOURCE;
    desc.CPUAccessFlags     = D3D11_CPU_ACCESS_WRITE;
    desc.Format             = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.MiscFlags          = 0;
    desc.SampleDesc.Count   = 1;
    desc.SampleDesc.Quality = 0;
    desc.Usage              = D3D11_USAGE_DYNAMIC;

    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem            = m_SimulationBuffer;
    data.SysMemPitch        = sizeof(uint32_t) * m_SimulationRenderSize.x;
    data.SysMemSlicePitch   = 0;

    HRESULT res = m_D3dDevice->CreateTexture2D(&desc, &data, &m_SimulationRenderTarget);
    if (res != S_OK)
    {
        return false;
    }

    m_Viewer.Initialize(m_SimulationRenderSize, m_SimulationBuffer);

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format                      = DXGI_FORMAT_R8G8B8A8_UNORM;
    srvDesc.ViewDimension               = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels         = 1;
    srvDesc.Texture2D.MostDetailedMip   = 0;

    res = m_D3dDevice->CreateShaderResourceView(m_SimulationRenderTarget, &srvDesc, &m_SimulationRenderSRV);
    if (res != S_OK)
    {
        return false;
    }

    return true;
}

void SimpleGPUSim::CleanupSimulationRenderTarget()
{
    if (m_SimulationRenderSRV != nullptr)
    {
        m_SimulationRenderSRV->Release();
        m_SimulationRenderSRV = nullptr;
    }

    if (m_SimulationRenderTarget != nullptr)
    {
        m_SimulationRenderTarget->Release();
        m_SimulationRenderTarget = nullptr;
    }

    if (m_SimulationBuffer != nullptr)
    {
        delete m_SimulationBuffer;
        m_SimulationBuffer = nullptr;
    }
}

bool SimpleGPUSim::CreateDeviceD3D(HWND windowHandle)
{
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));

    sd.BufferCount                          = 2;
    sd.Flags                                = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage                          = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow                         = windowHandle;

    sd.BufferDesc.Width                     = 0;
    sd.BufferDesc.Height                    = 0;
    sd.BufferDesc.Format                    = DXGI_FORMAT_R8G8B8A8_UNORM;

    sd.BufferDesc.RefreshRate.Numerator     = 60;
    sd.BufferDesc.RefreshRate.Denominator   = 1;

    sd.SampleDesc.Count                     = 1;
    sd.SampleDesc.Quality                   = 0;
    sd.Windowed                             = true;
    sd.SwapEffect                           = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
#ifdef _DEBUG
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = {
                                                        D3D_FEATURE_LEVEL_11_0,
                                                        D3D_FEATURE_LEVEL_10_0,
                                                   };

    if (D3D11CreateDeviceAndSwapChain(  nullptr,
                                        D3D_DRIVER_TYPE_HARDWARE,
                                        nullptr,
                                        createDeviceFlags,
                                        featureLevelArray,
                                        2,
                                        D3D11_SDK_VERSION,
                                        &sd,
                                        &m_SwapChain,
                                        &m_D3dDevice,
                                        &featureLevel,
                                        &m_D3dDeviceContext) != S_OK)
    {
        return false;
    }

    CreateRenderTarget();

    return true;
}

bool SimpleGPUSim::Initialize()
{
    if (m_IsReady)
    {
        return true;
    }

    m_WC = { sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"Simple GPU Simulation", nullptr};
    RegisterClassEx(&m_WC);
    m_WindowHandle = CreateWindow(L"Simple GPU Simulation", L"Simple GPU Simulation", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, m_WC.hInstance, nullptr);

    if (m_WindowHandle == nullptr)
    {
        CleanUp();
        return false;
    }

    // Initialize Direct3D
    if (!CreateDeviceD3D(m_WindowHandle))
    {
        CleanUp();
        return false;
    }

    if (!CreateSimulationRenderTarget())
    {
        CleanUp();
        return false;
    }

    // Show the window
    ShowWindow(m_WindowHandle, SW_SHOWDEFAULT);
    UpdateWindow(m_WindowHandle);

    // Setup ImGui binding
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplDX11_Init(m_WindowHandle, m_D3dDevice, m_D3dDeviceContext);
    ImGui::StyleColorsDark();
    io.Fonts->AddFontDefault();

    m_IsReady = true;
    return true;
}

void SimpleGPUSim::RenderSimulation()
{
    m_Viewer.RenderFrame();

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT res = m_D3dDeviceContext->Map(m_SimulationRenderTarget, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (res != S_OK)
    {
        return;
    }

    const XMUINT2& rs = m_Viewer.GetSimGraphics().GetRenderTargetSize();
    uint32_t rowPitch = rs.x * sizeof(uint32_t);

    BYTE* mappedData = reinterpret_cast<BYTE*>(mappedResource.pData);
    BYTE* buffer = reinterpret_cast<BYTE*>(m_SimulationBuffer);
    for (size_t i = 0; i < rs.y; i++)
    {
        memcpy(mappedData, buffer, rowPitch);
        mappedData += mappedResource.RowPitch;
        buffer += rowPitch;
    }

    m_D3dDeviceContext->Unmap(m_SimulationRenderTarget, 0);
}

void SimpleGPUSim::Run()
{
    if (!m_IsReady)
    {
        return;
    }

    ImGuiIO& io = ImGui::GetIO();

    Color clearColor = Color(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    MSG msg;
    ZeroMemory(&msg, sizeof(msg));
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
            continue;
        }
        ImGui_ImplDX11_NewFrame();

        ShowOverlay();
        ShowSimOptions();
        if (!ShowRenderSimWindow())
        {
            break;
        }

        m_D3dDeviceContext->OMSetRenderTargets(1, &m_MainRenderTargetView, nullptr);
        m_D3dDeviceContext->ClearRenderTargetView(m_MainRenderTargetView, (float*)&clearColor);

        RenderSimulation();

        ImGui::Render();
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        m_SwapChain->Present(1, 0);
    }
}

void SimpleGPUSim::ShowOverlay()
{
    ImVec2 window_pos = ImVec2(10.f, 10.f);
    ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always);
    ImGui::SetNextWindowBgAlpha(0.3f); // Transparent background

    if (!ImGui::Begin("Benchmark", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
    {
        ImGui::End();
        return;
    }

    ImGui::Text("FPS: %.2f", m_Viewer.GetFPS());
    ImGui::Separator();
    ImGui::Text("Frame Average Time: %.2fms", m_Viewer.GetFrameAverageTime());
    ImGui::Separator();
    ImGui::Text("Vertex Processing Average Time: %.2fms", m_Viewer.GetVertexAverageTime());
    ImGui::Separator();
    ImGui::Text("Pixel Processing Average Time: %.2fms", m_Viewer.GetPixelAverageTime());

    ImGui::End();
}

void SimpleGPUSim::ShowSimOptions()
{
    ImGui::Begin("Simulation Options");

    if (ImGui::Checkbox("Hardware Concurrency", &m_UseMaxThreads))
    {
        if (m_UseMaxThreads)
        {
            m_Viewer.GetSimGraphics().SetMaxThreadsToUse(MaxThreads::HardwareConcurrency);
        }
        else
        {
            m_Viewer.GetSimGraphics().SetMaxThreadsToUse(MaxThreads::SingleThreads);
        }
    }

    ImGui::Separator();

    if (ImGui::Checkbox("Z Buffer", &m_UseZBuffer))
    {
        m_Viewer.GetSimGraphics().ToggleZBufferActive();
    }

    ImGui::Separator();

    if (ImGui::Checkbox("Back Face Culling", &m_UseBackFaceCulling))
    {
        m_Viewer.GetSimGraphics().ToggleBackFaceCullingActive();
    }

    ImGui::Separator();

    if (ImGui::RadioButton("Wolf", &m_MeshSelection, 0))
    {
        m_Viewer.GetMeshData().SetExampleOpt(ExampleOpt::Wolf);
    }
    if(ImGui::RadioButton("Single Sphere", &m_MeshSelection, 1))
    {
        m_Viewer.GetMeshData().SetExampleOpt(ExampleOpt::SphereSingle);
    }
    if(ImGui::RadioButton("Multiple Spheres", &m_MeshSelection, 2))
    {
        m_Viewer.GetMeshData().SetExampleOpt(ExampleOpt::SphereMultiple);
    }

    ImGui::End();
}

bool SimpleGPUSim::ShowRenderSimWindow()
{
    ImGui::Begin("Simulation Render", nullptr);

    const ImVec2 pos    = ImGui::GetCursorScreenPos();
    const ImVec2 pos2   = ImGui::GetContentRegionAvail();

    const ImVec2 size = ImVec2(pos.x + pos2.x, pos.y + pos2.y);

    const ImVec2 windowSize = ImGui::GetWindowSize();
    if (m_SimulationRenderSize.x != windowSize.x || m_SimulationRenderSize.y != windowSize.y)
    {
        m_SimulationRenderSize.x = windowSize.x;
        m_SimulationRenderSize.y = windowSize.y;

        CleanupSimulationRenderTarget();
        if (!CreateSimulationRenderTarget())
        {
            ImGui::End();
            return false;
        }
    }

    ImTextureID user_texture_id = (void*)m_SimulationRenderSRV;
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddImage(user_texture_id, pos, size);

    ImGui::End();

    return true;
}

LRESULT WINAPI SimpleGPUSim::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
    {
        return true;
    }

    switch (msg)
    {
        case WM_SIZE:
            if (g_SimpleGPUSim->m_D3dDevice != NULL && wParam != SIZE_MINIMIZED)
            {
                ImGui_ImplDX11_InvalidateDeviceObjects();
                g_SimpleGPUSim->CleanupRenderTarget();
                g_SimpleGPUSim->m_SwapChain->ResizeBuffers(0, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam), DXGI_FORMAT_UNKNOWN, 0);
                g_SimpleGPUSim->CreateRenderTarget();
                ImGui_ImplDX11_CreateDeviceObjects();
            }
            return 0;

        case WM_SYSCOMMAND:
            if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
                return 0;
            break;

        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
    }

    return DefWindowProc(hWnd, msg, wParam, lParam);
}
