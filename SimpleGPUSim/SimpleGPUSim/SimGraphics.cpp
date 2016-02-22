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
#include <atomic>
#include <algorithm>

/********************
*   Game Includes   *
*********************/
#include "Camera.h"
#include "SimGraphics.h"

/********************
*   Function Defs   *
*********************/
SimGraphics::SimGraphics()
{
	m_MaxThreadsOpt = MaxThreads::HardwareConcurrency;
	m_MaxThreadsToUse = m_ThreadPool.GetMaxThreads();
}

SimGraphics::~SimGraphics()
{
	m_ThreadPool.Stop();
}

void SimGraphics::Initialize(const XMUINT2& renderTargetSize, uint32_t maxFacesPerIteration, const Camera& camera)
{
	if (renderTargetSize.x == 0 || renderTargetSize.y == 0)
	{
		return;
	}

	m_RenderTargetSize = renderTargetSize;

	uint32_t totalSize = m_RenderTargetSize.x * m_RenderTargetSize.y;

	m_CurrentCamera = &camera;
	m_RenderTargetSize = renderTargetSize;

	m_Framebuffer = QImage(m_RenderTargetSize.x, m_RenderTargetSize.y, QImage::Format::Format_RGB888);

	m_ZBuffer.resize(totalSize);

	m_ProcessingFaces.resize(maxFacesPerIteration);

	m_IsReady = true;
}

void SimGraphics::Clear()
{
	if (!m_IsReady)
	{
		return;
	}

	m_Framebuffer.fill(Qt::GlobalColor::black);
	std::fill_n(m_ZBuffer.begin(), m_ZBuffer.size(), m_CurrentCamera->GetZFar());
}

void SimGraphics::Draw(const std::vector<Vertex>& vtxBuffer, const std::vector<uint32_t>& idxBuffer, const XMMATRIX& worldPos, const Color& color)
{
	if (!m_IsReady || idxBuffer.size() == 0 || idxBuffer.size() % 3 != 0)
	{
		return;
	}

	m_TasksVars.Clear();

	////////////////////////
	//Calculate WVP
	XMMATRIX xmView = XMLoadFloat4x4(&m_CurrentCamera->GetViewMatrix());
	XMMATRIX xmProj = XMLoadFloat4x4(&m_CurrentCamera->GetProjectionMatrix());
	XMMATRIX xmWVP = worldPos * xmView * xmProj;

	////////////////////////
	//Compute Faces
	uint32_t iterations = ((idxBuffer.size() / 3) / m_ProcessingFaces.size()) + 1;
	uint32_t currentIndex = 0;

	///////////////////////
	//Set Tasks Vars
	m_TasksVars.m_MaxThreads = m_MaxThreadsToUse;

	for (uint32_t faceIt = 0; faceIt < iterations; ++faceIt)
	{
		m_TasksVars.m_FacesIdx = ATOMIC_VAR_INIT(0);

		m_StartVertex = std::chrono::high_resolution_clock::now();
		for (; currentIndex < idxBuffer.size() && m_TasksVars.m_FacesNum < m_ProcessingFaces.size(); currentIndex += 3, ++m_TasksVars.m_FacesNum)
		{
			const Vertex& v1 = vtxBuffer[idxBuffer[currentIndex + 0]];
			const Vertex& v2 = vtxBuffer[idxBuffer[currentIndex + 1]];
			const Vertex& v3 = vtxBuffer[idxBuffer[currentIndex + 2]];

			m_ProcessingFaces[m_TasksVars.m_FacesNum].m_Color = color;
			ComputeFace(worldPos, xmWVP, v1, v2, v3, m_ProcessingFaces[m_TasksVars.m_FacesNum]);
		}
		m_EndVertex = std::chrono::high_resolution_clock::now();

		m_StartPixel = std::chrono::high_resolution_clock::now();
		for (uint32_t i = 0; i < m_TasksVars.m_MaxThreads; ++i)
		{
			m_ThreadPool.AddTask(std::bind(&SimGraphics::MultiThreadRaster, this));
		}
		while (m_TasksVars.m_ThreadsFinished < m_TasksVars.m_MaxThreads)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(100));
		}
		m_EndPixel = std::chrono::high_resolution_clock::now();
	}
}

void SimGraphics::DrawMultiple(const std::vector<Vertex>& vtxBuffer, const std::vector<uint32_t>& idxBuffer, const std::vector<MeshElement>& meshes, const XMMATRIX& xmRotationMatrix)
{
	if (!m_IsReady || idxBuffer.size() == 0 || idxBuffer.size() % 3 != 0)
	{
		return;
	}

	m_TasksVars.Clear();

	////////////////////////
	//Compute Faces
	uint32_t iterations = (((idxBuffer.size() / 3) * meshes.size()) / m_ProcessingFaces.size()) + 1;
	uint32_t currentIndex = 0;

	//Get View Projection
	XMMATRIX xmView = XMLoadFloat4x4(&m_CurrentCamera->GetViewMatrix());
	XMMATRIX xmProj = XMLoadFloat4x4(&m_CurrentCamera->GetProjectionMatrix());
	XMMATRIX xmViewProj = xmView * xmProj;

	///////////////////////
	//Set Tasks Vars
	m_TasksVars.m_VtxBuffer = &vtxBuffer;
	m_TasksVars.m_IdxBuffer = &idxBuffer;
	m_TasksVars.m_Meshes = &meshes;
	m_TasksVars.m_MaxLogicIdx = meshes.size() * idxBuffer.size();
	m_TasksVars.m_MaxLogicIdxPerIteration = m_ProcessingFaces.size() * 3;
	m_TasksVars.m_MaxThreads = m_MaxThreadsToUse;
	m_TasksVars.m_xmRotationMat = &xmRotationMatrix;
	m_TasksVars.m_xmViewProj = &xmViewProj;

	for (uint32_t faceIt = 0; faceIt < iterations; ++faceIt)
	{
		m_StartVertex = std::chrono::high_resolution_clock::now();

		m_TasksVars.m_FacesIdx = ATOMIC_VAR_INIT(0);
		m_TasksVars.m_IdxBufferIdx = ATOMIC_VAR_INIT(faceIt * static_cast<uint32_t>(m_ProcessingFaces.size()));

		for (uint32_t i = 0; i < m_TasksVars.m_MaxThreads; ++i)
		{
			m_ThreadPool.AddTask(std::bind(&SimGraphics::MultiThreadVertex, this));
		}
		while (m_TasksVars.m_ThreadsFinished < m_TasksVars.m_MaxThreads)
		{
			std::this_thread::yield();
		}

		m_EndVertex = std::chrono::high_resolution_clock::now();

		m_StartPixel = std::chrono::high_resolution_clock::now();

		m_TasksVars.m_FacesNum = m_TasksVars.m_FacesIdx;
		m_TasksVars.m_ThreadsFinished = ATOMIC_VAR_INIT(0);
		m_TasksVars.m_FacesIdx = ATOMIC_VAR_INIT(0);

		for (uint32_t i = 0; i < m_TasksVars.m_MaxThreads; ++i)
		{
			m_ThreadPool.AddTask(std::bind(&SimGraphics::MultiThreadRaster, this));
		}
		while (m_TasksVars.m_ThreadsFinished < m_TasksVars.m_MaxThreads)
		{
			std::this_thread::yield();
		}

		m_EndPixel = std::chrono::high_resolution_clock::now();
	}
}

void SimGraphics::MultiThreadVertex()
{
	while (true)
	{
		uint32_t logicalIdx = std::atomic_fetch_add(&m_TasksVars.m_IdxBufferIdx, 3);
		if (logicalIdx >= m_TasksVars.m_MaxLogicIdx || logicalIdx >= m_TasksVars.m_MaxLogicIdxPerIteration)
		{
			break;
		}
		uint32_t faceIdx = std::atomic_fetch_add(&m_TasksVars.m_FacesIdx, 1);

		uint32_t meshIdx = (logicalIdx / m_TasksVars.m_IdxBuffer->size());
		uint32_t idx = logicalIdx - (m_TasksVars.m_IdxBuffer->size() * meshIdx);

		const Vertex& v1 = m_TasksVars.m_VtxBuffer->at(m_TasksVars.m_IdxBuffer->at(idx + 0));
		const Vertex& v2 = m_TasksVars.m_VtxBuffer->at(m_TasksVars.m_IdxBuffer->at(idx + 1));
		const Vertex& v3 = m_TasksVars.m_VtxBuffer->at(m_TasksVars.m_IdxBuffer->at(idx + 2));

		////////////////////////
		//Calculate World & WVP
		XMVECTOR xmPos = XMLoadFloat3(&m_TasksVars.m_Meshes->at(meshIdx).m_Position);
		XMMATRIX xmTranslate = XMMatrixTranslationFromVector(xmPos);

		XMMATRIX xmWorld = (xmTranslate * *m_TasksVars.m_xmRotationMat);
		XMMATRIX xmWVP = xmWorld * *m_TasksVars.m_xmViewProj;

		m_ProcessingFaces[faceIdx].m_Color = m_TasksVars.m_Meshes->at(meshIdx).m_Color;
		ComputeFace(xmWorld, xmWVP, v1, v2, v3, m_ProcessingFaces[faceIdx]);
	}

	std::atomic_fetch_add(&m_TasksVars.m_ThreadsFinished, 1);
}

void SimGraphics::MultiThreadRaster()
{
	while (true)
	{
		uint32_t idx = std::atomic_fetch_add(&m_TasksVars.m_FacesIdx, 1);
		if (idx >= m_TasksVars.m_FacesNum)
		{
			break;
		}

		ComputeRaster(m_ProcessingFaces[idx]);

		if (!m_ProcessingFaces[idx].m_Hidden)
		{
			ComputePixels(m_ProcessingFaces[idx]);
		}
	}

	std::atomic_fetch_add(&m_TasksVars.m_ThreadsFinished, 1);
}

void SimGraphics::ComputeFace(const XMMATRIX& world, const XMMATRIX& wvp, const Vertex& v1, const Vertex& v2, const Vertex& v3, Face& face)
{
	ComputeVertex(world, wvp, v1, face.m_Vertex[0]);
	ComputeVertex(world, wvp, v2, face.m_Vertex[1]);
	ComputeVertex(world, wvp, v3, face.m_Vertex[2]);
}

void SimGraphics::ComputeVertex(const XMMATRIX& world, const XMMATRIX& wvp, const Vertex& vertex, OutPutVertex& outputVertex)
{
	XMVECTOR xmPos = XMVectorSetW(XMLoadFloat3(&vertex.m_Position), 1);
	XMVECTOR xmNormal = XMVectorSetW(XMLoadFloat3(&vertex.m_Normal), 0);

	XMStoreFloat4(&outputVertex.m_Position, XMVector4Transform(xmPos, wvp));
	XMStoreFloat3(&outputVertex.m_Normal, XMVector4Transform(xmNormal, wvp));
	XMStoreFloat3(&outputVertex.m_PositionWS, XMVector4Transform(xmPos, world));
}

void SimGraphics::ComputeRaster(Face& face)
{
	face.m_Hidden = false;

	if (face.m_Vertex[0].m_Position.w == 0.0f || face.m_Vertex[1].m_Position.w == 0 || face.m_Vertex[2].m_Position.w == 0)
	{
		face.m_Hidden = true;

		return;
	}

	XMVECTOR xmPos1 = XMLoadFloat4(&face.m_Vertex[0].m_Position);
	XMVECTOR xmPos2 = XMLoadFloat4(&face.m_Vertex[1].m_Position);
	XMVECTOR xmPos3 = XMLoadFloat4(&face.m_Vertex[2].m_Position);

	//////////////////////////////////////////////////
	//Calculate Face Normal for Back Face Culling
	
	if (m_BackFaceCullingActive)
	{
		XMVECTOR xmFN1 = xmPos2 - xmPos1;
		XMVECTOR xmFN2 = xmPos3 - xmPos1;

		XMVECTOR xmNormal = XMVector3Cross(xmFN1, xmFN2);
		XMVECTOR xmTemp = xmPos1 * -1;

		xmTemp = XMVector3Dot(xmTemp, xmNormal);

		if (XMVectorGetX(xmTemp) >= 0)
		{
			face.m_Hidden = true;

			return;
		}
	}

	/////////////////
	//Calculate NDC
	xmPos1 = xmPos1 / face.m_Vertex[0].m_Position.w;
	xmPos2 = xmPos2 / face.m_Vertex[1].m_Position.w;
	xmPos3 = xmPos3 / face.m_Vertex[2].m_Position.w;

	XMStoreFloat4(&face.m_Vertex[0].m_Position, xmPos1);
	XMStoreFloat4(&face.m_Vertex[1].m_Position, xmPos2);
	XMStoreFloat4(&face.m_Vertex[2].m_Position, xmPos3);

	////////////////
	//Get Windows Relative Coordinates
	ConvertNDCToRaster(face.m_Vertex[0].m_Position);
	ConvertNDCToRaster(face.m_Vertex[1].m_Position);
	ConvertNDCToRaster(face.m_Vertex[2].m_Position);

	//Set Face BB
	float maxX = Maximum(face.m_Vertex[0].m_Position.x, face.m_Vertex[1].m_Position.x, face.m_Vertex[2].m_Position.x);
	float minX = Minimum(face.m_Vertex[0].m_Position.x, face.m_Vertex[1].m_Position.x, face.m_Vertex[2].m_Position.x);

	float maxY = Maximum(face.m_Vertex[0].m_Position.y, face.m_Vertex[1].m_Position.y, face.m_Vertex[2].m_Position.y);
	float minY = Minimum(face.m_Vertex[0].m_Position.y, face.m_Vertex[1].m_Position.y, face.m_Vertex[2].m_Position.y);

	if (maxX < 0.0f || minX >(m_RenderTargetSize.x - 1) ||
			maxY < 0.0f || minY >(m_RenderTargetSize.y - 1))
	{
		face.m_Hidden = true;

		return;
	}

	face.m_BoundingBox.m_MaxX = std::min((int32_t)m_RenderTargetSize.x - 1, (int32_t)(std::floor(maxX)));
	face.m_BoundingBox.m_MinX = std::max(0, (int32_t)(std::floor(minX)));

	face.m_BoundingBox.m_MinY = std::max(0, (int32_t)(std::floor(minY)));
	face.m_BoundingBox.m_MaxY = std::min((int32_t)m_RenderTargetSize.y - 1, (int32_t)(std::floor(maxY)));

	face.m_Area = EdgeFunction(face.m_Vertex[0].m_Position, face.m_Vertex[1].m_Position, face.m_Vertex[2].m_Position);
}

void SimGraphics::ComputePixels(const Face& face)
{
	for (uint32_t x = face.m_BoundingBox.m_MinX; x <= face.m_BoundingBox.m_MaxX; ++x)
	{
		for (uint32_t y = face.m_BoundingBox.m_MinY; y <= face.m_BoundingBox.m_MaxY; ++y)
		{
			XMFLOAT4 pixelPoint = XMFLOAT4(x + 0.5f, y + 0.5f, 0.0f, 0.0f);

			float p0 = 0;
			float p1 = 0;
			float p2 = 0;

			if((p0 = EdgeFunction(face.m_Vertex[1].m_Position, face.m_Vertex[2].m_Position, pixelPoint)) >= 0 &&
			   (p1 = EdgeFunction(face.m_Vertex[2].m_Position, face.m_Vertex[0].m_Position, pixelPoint)) >= 0 &&
			   (p2 = EdgeFunction(face.m_Vertex[0].m_Position, face.m_Vertex[1].m_Position, pixelPoint)) >= 0)
			{
				p0 /= face.m_Area;
				p1 /= face.m_Area;
				p2 /= face.m_Area;

				float depth = (face.m_Vertex[0].m_Position.z * p0) + (face.m_Vertex[1].m_Position.z * p1) + (face.m_Vertex[2].m_Position.z * p2);
				uint32_t arrPos = (y * m_RenderTargetSize.y) + x;

				bool isVisible = false;

				if (m_ZBufferActive)
				{
					if (depth < m_ZBuffer[arrPos])
					{
						m_ZBuffer[arrPos] = depth;
						isVisible = true;
					}
				}
				else
				{
					isVisible = true;
				}

				if (isVisible)
				{
					XMVECTOR xmNor1 = XMLoadFloat3(&face.m_Vertex[0].m_Normal) * p0;
					XMVECTOR xmNor2 = XMLoadFloat3(&face.m_Vertex[1].m_Normal) * p1;
					XMVECTOR xmNor3 = XMLoadFloat3(&face.m_Vertex[2].m_Normal) * p2;
					xmNor1 = XMVector3Normalize( (xmNor1 + xmNor2 + xmNor3) );

					XMVECTOR xmPos1 = XMLoadFloat3(&face.m_Vertex[0].m_PositionWS) * p0;
					XMVECTOR xmPos2 = XMLoadFloat3(&face.m_Vertex[1].m_PositionWS) * p1;
					XMVECTOR xmPos3 = XMLoadFloat3(&face.m_Vertex[2].m_PositionWS) * p2;
					xmPos1 = xmPos1 + xmPos2 + xmPos3;

					//Color Vertex to Test Plane
					//Color color = (face.m_Vertex[0].m_Color * p0) + (face.m_Vertex[1].m_Color * p1) + (face.m_Vertex[2].m_Color * p2);

					uint32_t pixelColor = ComputePixel(xmPos1, xmNor1, face.m_Color);
					m_Framebuffer.setPixel(x, y, pixelColor);
				}
			}
		}
	}
}

uint32_t SimGraphics::ComputePixel(const XMVECTOR& xmPosWS, const XMVECTOR& xmNormal, const Color& color)
{
	float specularFactor = 8.0f;
	Color specularColor(1.0f, 1.0f, 1.0f, 1.0f);
	Color processedColor = color;

	XMVECTOR xmLightDir = XMLoadFloat3(&m_Light.m_LightDir);
	XMVECTOR xmNegatedLightDir = XMLoadFloat3(&m_Light.m_NegatedLightDir);

	XMVECTOR xmDot;
	XMVECTOR xmCameraPos = XMLoadFloat3(&m_CurrentCamera->GetPosition());

	//Diffuse Color
	xmDot = XMVector3Dot(xmNegatedLightDir, xmNormal);
	float ldn = XMVectorGetX(xmDot);
	ldn = std::max(0.0f, ldn);

	//Specular
	XMVECTOR xmReflect = XMVector3Reflect(xmLightDir, xmNormal);

	XMVECTOR xmDirToPos = xmPosWS - xmCameraPos;
	xmDirToPos = XMVector3Normalize(xmDirToPos);
	xmDirToPos = xmDirToPos * -1;

	xmDot = XMVector3Dot(xmReflect, xmDirToPos);
	float rdp = XMVectorGetX(xmDot);
	rdp = clamp(rdp, 0.0f, 1.0f);
	float specularShine = pow(rdp, specularFactor);

	specularColor = specularColor * specularShine;

	//Set Color
	(processedColor *= ldn) += specularColor;
	processedColor.m_Color.w = 1.0f;

	//Return
	return processedColor;
}
