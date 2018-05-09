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

/********************
*   Game Includes   *
*********************/
#include "Viewer.h"

/********************
*   Function Defs   *
*********************/
Viewer::Viewer()
    : m_MeshData(EXAMPLE_SPHERE_MULTIPLE, EXAMPLE_WOLF, EXAMPLE_SPHERE_SINGLE)
{
}

Viewer::~Viewer()
{
}

void Viewer::Initialize(const XMUINT2& renderSize, uint32_t* frameBuffer)
{
    m_Camera.Initialize(XMFLOAT3(0.0f, 0.0f, -85.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3UP, renderSize, 45.0f, 0.1f, 1000.0f);
    m_SimGraphics.Initialize(renderSize, 500000, m_Camera, frameBuffer);
}

void Viewer::RenderFrame()
{
    m_Timer.Update();
    m_SimGraphics.Clear();

    m_CurrentRotation += XM_2PI * m_RotationSpeed * (float)m_Timer.GetElapsedTime();
    if (m_CurrentRotation >= XM_2PI)
    {
        m_CurrentRotation = 0;
    }

    auto t0 = std::chrono::high_resolution_clock::now();
    m_MeshData.Render(m_SimGraphics, m_CurrentRotation);
    auto t1 = std::chrono::high_resolution_clock::now();
    auto frameTime = std::chrono::duration<double, std::milli>(t1 - t0).count();
    m_FrameTime.AddTime(frameTime);

    auto vertexTime = std::chrono::duration<double, std::milli>(m_SimGraphics.m_EndVertex - m_SimGraphics.m_StartVertex).count();
    auto pixelTime = std::chrono::duration<double, std::milli>(m_SimGraphics.m_EndPixel - m_SimGraphics.m_StartPixel).count();
    m_VertexTime.AddTime(vertexTime);
    m_PixelTime.AddTime(pixelTime);

    m_Frame++;
    m_FPS = (float)(1000.f / m_FrameTime.GetAverageTime());
}
