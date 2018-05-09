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

#ifndef SIMPLEGPUSIM_H
#define SIMPLEGPUSIM_H

/**********************
*   System Includes   *
***********************/
#include <d3d11.h>
#include <Windows.h>

/********************
*   Game Includes   *
*********************/
#include "Viewer.h"

class SimpleGPUSim
{
    private:

        Viewer m_Viewer;
        bool m_IsReady = false;

        HWND m_WindowHandle = nullptr;
        ID3D11Device* m_D3dDevice = nullptr;
        ID3D11DeviceContext* m_D3dDeviceContext = nullptr;
        IDXGISwapChain* m_SwapChain = nullptr;
        ID3D11RenderTargetView* m_MainRenderTargetView = nullptr;
        WNDCLASSEX m_WC;
        ID3D11Texture2D* m_SimulationRenderTarget = nullptr;
        ID3D11ShaderResourceView* m_SimulationRenderSRV = nullptr;
        uint32_t* m_SimulationBuffer = nullptr;
        XMUINT2 m_SimulationRenderSize = {100, 100};

        bool m_UseMaxThreads = true;
        bool m_UseZBuffer = true;
        bool m_UseBackFaceCulling = true;
        int m_MeshSelection = 0;

        void CreateRenderTarget();
        void CleanupRenderTarget();
        bool CreateDeviceD3D(HWND windowHandle);
        void CleanupDeviceD3D();
        void RenderSimulation();

        bool CreateSimulationRenderTarget();
        void CleanupSimulationRenderTarget();

        bool ShowRenderSimWindow();
        void ShowSimOptions();
        void ShowOverlay();

        void CleanUp();

        static LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

    public:

        SimpleGPUSim();
        ~SimpleGPUSim();

        bool Initialize();

        void Run();
};

#endif // SIMPLEGPUSIM_H
