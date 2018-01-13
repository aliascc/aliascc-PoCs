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

#pragma once

#ifndef _SIM_GRAPHICS
#define _SIM_GRAPHICS

/**********************
*   System Includes   *
***********************/
#include <mutex>
#include <chrono>
#include <atomic>
#include <qimage.h>
#include <condition_variable>

/********************
*   Game Includes   *
*********************/
#include "ThreadPool.h"
#include "SimGraphicsDefs.h"

/*******************
*   Forward Defs   *
********************/
class Camera;

/// <summary>
/// Simulates a basic render path
/// </summary>
class SimGraphics
{
	private:

		/// <summary>
		/// Variables to share between tasks 
		/// </summary>
		struct TasksVars
		{
			/// <summary>
			/// Max Logical Index Buffer Index
			/// Use to draw multiple meshes 
			/// </summary>
			uint32_t m_MaxLogicIdx = 0;

			/// <summary>
			/// Max Logical Index Buffer Index per iteration
			/// </summary>
			uint32_t m_MaxLogicIdxPerIteration = 0;

			/// <summary>
			/// Number of faces process
			/// </summary>
			uint32_t m_FacesNum = 0;

			/// <summary>
			/// Max Tasks that are going to run
			/// </summary>
			uint32_t m_MaxThreads = 0;

			/// <summary>
			/// Pointer to Vertex Buffer
			/// </summary>
			const std::vector<Vertex>* m_VtxBuffer = nullptr;

			/// <summary>
			/// Pointer to Index Buffer
			/// </summary>
			const std::vector<uint32_t>* m_IdxBuffer = nullptr;

			/// <summary>
			/// Pointer to Mesh Elements
			/// </summary>
			const std::vector<MeshElement>* m_Meshes = nullptr;

			/// <summary>
			/// Pointer to Rotation Matrix
			/// </summary>
			const XMMATRIX* m_xmRotationMat = nullptr;

			/// <summary>
			/// Pointer to View Projection Matrix
			/// </summary>
			const XMMATRIX* m_xmViewProj = nullptr;

			/// <summary>
			/// Current Index Buffer Index to process
			/// </summary>
			std::atomic<uint32_t> m_IdxBufferIdx = ATOMIC_VAR_INIT(0);

			/// <summary>
			/// Current Face Index to process
			/// </summary>
			std::atomic<uint32_t> m_FacesIdx = ATOMIC_VAR_INIT(0);

			/// <summary>
			/// Number of threads that have finish
			/// </summary>
			std::atomic<uint32_t> m_ThreadsFinished = ATOMIC_VAR_INIT(0);

			/// <summary>
			/// Clears the Tasks Variables back to default
			/// </summary>
			inline void Clear()
			{
				m_MaxLogicIdx = 0;
				m_MaxLogicIdxPerIteration = 0;

				m_FacesNum = 0;
				m_MaxThreads = 0;

				m_VtxBuffer = nullptr;
				m_IdxBuffer = nullptr;
				m_Meshes = nullptr;

				m_xmRotationMat = nullptr;
				m_xmViewProj = nullptr;

				m_IdxBufferIdx = ATOMIC_VAR_INIT(0);
				m_FacesIdx = ATOMIC_VAR_INIT(0);
				m_ThreadsFinished = ATOMIC_VAR_INIT(0);
			}
		};

		/// <summary>
		/// Light Information
		/// </summary>
		struct Light
		{
			XMFLOAT3 m_LightDir = XMFLOAT3(0.0f, -0.258819073f, 0.965925813f);;
			XMFLOAT3 m_NegatedLightDir = XMFLOAT3(0.0f, 0.258819073f, -0.965925813f);
		};

		/// <summary>
		/// Task variables
		/// </summary>
		TasksVars m_TasksVars;

		/// <summary>
		/// Thread Pool on where to run tasks
		/// </summary>
		ThreadPool m_ThreadPool;

		/// <summary>
		/// Max number of threads to use
		/// </summary>
		uint32_t m_MaxThreadsToUse = 0;

		/// <summary>
		/// Light Object
		/// </summary>
		Light m_Light;

		/// <summary>
		/// Frame Buffer on which to render
		/// </summary>
		QImage m_Framebuffer;

		/// <summary>
		/// Z Buffer on which to perform the Z Buffer Test
		/// </summary>
		std::vector<float> m_ZBuffer;

		/// <summary>
		/// Width and Height of the render target
		/// </summary>
		XMUINT2 m_RenderTargetSize = XMUINT2ZERO;

		/// <summary>
		/// Current View Camera
		/// </summary>
		const Camera* m_CurrentCamera = nullptr;

		/// <summary>
		/// Has instance been initialize
		/// </summary>
		bool m_IsReady = false;

		/// <summary>
		/// Is ZBuffer Test Active
		/// </summary>
		bool m_ZBufferActive = true;

		/// <summary>
		/// Is Back Face Culling Test Active
		/// </summary>
		bool m_BackFaceCullingActive = true;

		/// <summary>
		/// Thread Option
		/// </summary>
		MaxThreads m_MaxThreadsOpt = MaxThreads::HardwareConcurrency;

		/// <summary>
		/// Vector holding the faces that have been process by the ComputeVertex
		/// </summary>
		std::vector<Face> m_ProcessingFaces;

		/// <summary>
		/// Task Method to be use by Threads to process the Vertices
		/// </summary>
		void MultiThreadVertex();

		/// <summary>
		/// Task Method to be use by Threads to process the Faces and Pixels
		/// </summary>
		void MultiThreadRaster();

		/// <summary>
		/// Computes the Vertex to get it to clip 
		/// </summary>
		/// <param name="world">World Matrix</param>
		/// <param name="wvp">World View Projection Matrix</param>
		/// <param name="vertex">Vertex to process</param>
		/// <param name="outputVertex">Resulting Vertex</param>
		void ComputeVertex(const XMMATRIX& world, const XMMATRIX& wvp, const Vertex& vertex, OutPutVertex& outputVertex);

		/// <summary>
		/// Computes the 3 Vertices that makes up a face
		/// </summary>
		/// <param name="world">World Matrix</param>
		/// <param name="wvp">World View Projection Matrix</param>
		/// <param name="v1">Vertex 1 to process</param>
		/// <param name="v2">Vertex 2 to process</param>
		/// <param name="v3">Vertex 2 to process</param>
		/// <param name="face">Resulting face</param>
		void ComputeFace(const XMMATRIX& world, const XMMATRIX& wvp, const Vertex& v1, const Vertex& v2, const Vertex& v3, Face& face);

		/// <summary>
		/// Translate the information of the face
		/// - Checks if face is inside the screen
		/// - Interpolates between each vertex to render the pixel
		/// </summary>
		/// <param name="face">Face to process</param>
		void ComputeRaster(Face& face);

		/// <summary>
		/// Goes through every pixel of the face, checks the ZBuffer
		/// and renders to framebuffer
		/// </summary>
		/// <param name="face">Face to process</param>
		void ComputePixels(const Face& face);

		/// <summary>
		/// Calculate Light and color of the pixel
		/// </summary>
		/// <param name="xmPosWS">Position in World Space of the Pixel</param>
		/// <param name="xmNormal">Normal of the Pixel</param>
		/// <param name="color">Color of the pixel</param>
		/// <returns>pixel color</returns>
		uint32_t ComputePixel(const XMVECTOR& xmPosWS, const XMVECTOR& xmNormal, const Color& color);

		/// <summary>
		/// Converts from NDC to Screen Coordinates
		/// </summary>
		/// <param name="vec">NDC Coordinates to convert</param>
		inline void ConvertNDCToRaster(XMFLOAT4& vec)
		{
			vec.x = (vec.x + 1.0f) / 2.0f * m_RenderTargetSize.x;
			vec.y = (1.0f - vec.y) / 2.0f * m_RenderTargetSize.y;
		}

	public:

		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartVertex;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_EndVertex;

		std::chrono::time_point<std::chrono::high_resolution_clock> m_StartPixel;
		std::chrono::time_point<std::chrono::high_resolution_clock> m_EndPixel;

		/// <summary>
		/// Constructor of SimGraphics
		/// </summary>
		SimGraphics();

		/// <summary>
		/// Destructor of SimGraphics
		/// </summary>
		~SimGraphics();

		/// <summary>
		/// Gets the Framebuffer of the Simulate Graphics
		/// </summary>
		inline const QImage& GetFramebuffer() const
		{
			return m_Framebuffer;
		}

		/// <summary>
		/// Set how many threads SimGraphics can use
		/// </summary>
		/// <param name="opt">Max Threads to use</param>
		inline void SetMaxThreadsToUse(MaxThreads opt)
		{
			m_MaxThreadsOpt = opt;
			switch (m_MaxThreadsOpt)
			{
				case MaxThreads::SingleThreads:
					m_MaxThreadsToUse = 1;
					break;

				case MaxThreads::HardwareConcurrency:
					m_MaxThreadsToUse = m_ThreadPool.GetMaxThreads();
					break;
			}
		}

		/// <summary>
		/// Get how many threads SimGraphics can use
		/// </summary>
		inline MaxThreads GetMaxThreadsOpt() const
		{
			return m_MaxThreadsOpt;
		}

		/// <summary>
		/// Gets of the ZBuffer Test is Active
		/// </summary>
		inline bool GetZBufferActive() const
		{
			return m_ZBufferActive;
		}

		/// <summary>
		/// Gets if Back Face Culling Test is Active
		/// </summary>
		inline bool GetBackFaceCullingActive() const
		{
			return m_BackFaceCullingActive;
		}

		/// <summary>
		///Toggles the ZBuffer Test
		/// </summary>
		inline void ToggleZBufferActive()
		{
			m_ZBufferActive = !m_ZBufferActive;
		}

		/// <summary>
		///Toggles the Back-Face Culling Test
		/// </summary>
		inline void ToggleBackFaceCullingActive()
		{
			m_BackFaceCullingActive = !m_BackFaceCullingActive;
		}

		/// <summary>
		/// Gets the Width and Height of the render target
		/// </summary>
		inline const XMUINT2& GetRenderTargetSize()
		{
			return m_RenderTargetSize;
		}

		/// <summary>
		/// Initializes the SimGraphic Object
		/// </summary>
		/// <param name="renderTargetSize">Width and Height of the render target</param>
		/// <param name="maxFacesPerIteration">Max Faces to Render per iteration</param>
		/// <param name="camera">Camera currently been use</param>
		void Initialize(const XMUINT2& renderTargetSize, uint32_t maxFacesPerIteration, const Camera& camera);

		/// <summary>
		/// Clears the Frame Buffer and ZBuffer
		/// </summary>
		void Clear();

		/// <summary>
		/// Draws the Mesh to the Framebuffer
		/// </summary>
		/// <param name="vtxBuffer">Vertex Buffer of the Mesh</param>
		/// <param name="idxBuffer">Index Buffer of the Mesh</param>
		/// <param name="worldPos">World Matrix of the Mesh</param>
		/// <param name="color">Color of the Mesh</param>
		void Draw(const std::vector<Vertex>& vtxBuffer, const std::vector<uint32_t>& idxBuffer, const XMMATRIX& worldPos, const Color& color);

		/// <summary>
		/// Draws multiple Meshes to the Framebuffer
		/// </summary>
		/// <param name="vtxBuffer">Vertex Buffer of the Mesh</param>
		/// <param name="idxBuffer">Index Buffer of the Mesh</param>
		/// <param name="meshes">Mesh Elements to render</param>
		/// <param name="xmRotationMatrix">Rotation Matrix of the Meshes</param>
		/// <param name="color">Color of the Meshes</param>
		void DrawMultiple(const std::vector<Vertex>& vtxBuffer, const std::vector<uint32_t>& idxBuffer, const std::vector<MeshElement>& meshes, const XMMATRIX& xmRotationMatrix);
};

#endif
