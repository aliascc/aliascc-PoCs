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
	m_Camera.Initialize(XMFLOAT3(0.0f, 0.0f, -85.0f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT3UP, XMUINT2(WINDOW_HEIGHT, WINDOW_WIDTH), 45.0f, 0.1f, 1000.0f);
	m_SimGraphics.Initialize(XMUINT2(WINDOW_HEIGHT, WINDOW_WIDTH), 500000, m_Camera);
}

Viewer::~Viewer()
{
}

void Viewer::PaintFrameBuffer(QPainter& painter)
{
	m_Frame++;

	//////////////////////////////////////////////////////////////////////////////////
	// Display Examples
	//////////////////////////////////////////////////////////////////////////////////
	const QImage& image = m_SimGraphics.GetFramebuffer();

	painter.drawImage(QPoint(0, 0), image);

	//////////////////////////////////////////////////////////////////////////////////
	// Display FPS
	//////////////////////////////////////////////////////////////////////////////////
	painter.setPen(Qt::GlobalColor::white);

	float fps = (float)(1000.f / m_FrameTime.GetAverageTime());

	char str[1024];
	sprintf_s(str, "Frame: %d: fps: %.1f, %.2fms", m_Frame, fps, m_FrameTime.GetAverageTime());
	painter.drawText(0, 32, str);

	sprintf_s(str, "Vertex: %.2fms, Pixel: %.2fms", m_VertexTime.GetAverageTime(), m_PixelTime.GetAverageTime());
	painter.drawText(0, 48, str);

	sprintf_s(str, "Threads: %s, ZBuffer: %s, BackFace Culling: %s",
		((m_SimGraphics.GetMaxThreadsOpt() == MaxThreads::HardwareConcurrency) ? "HC" : "ST"),
		(m_SimGraphics.GetZBufferActive() ? "1" : "0"),
		(m_SimGraphics.GetBackFaceCullingActive() ? "1" : "0"));
	painter.drawText(0, 64, str);
}

void Viewer::RenderFrame(QPainter& painter)
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

	PaintFrameBuffer(painter);
}
