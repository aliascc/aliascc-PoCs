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

#ifndef _VIEWER_H
#define _VIEWER_H

/**********************
*   System Includes   *
***********************/
#include <windows.h>

/********************
*   Game Includes   *
*********************/
#include "Timer.h"
#include "Camera.h"
#include "MeshData.h"
#include "RenderTime.h"
#include "SimGraphics.h"

/**************
*   Defines   *
***************/
#define EXAMPLE_WOLF                "example_wolf.txt"
#define EXAMPLE_SPHERE_MULTIPLE     "example_sphere_multiple.txt"
#define EXAMPLE_SPHERE_SINGLE       "example_sphere_single.txt"

/// <summary>
/// Renders the a given example to a frame buffer
/// </summary>
class Viewer
{
    private:

        /// <summary>
        /// Number of frames per second
        /// </summary>
        float m_FPS = 0;

        /// <summary>
        /// Number of frames rendered
        /// </summary>
        int m_Frame = 0;

        /// <summary>
        /// Current rotation of the objects
        /// </summary>
        float m_CurrentRotation = 0.0f;

        /// <summary>
        /// Rotation speed of the objects
        /// </summary>
        float m_RotationSpeed = 0.10f;

        /// <summary>
        /// Timer to keep track of the elapsed time
        /// </summary>
        Timer m_Timer;

        /// <summary>
        /// Class to Simulated Graphics Render
        /// </summary>
        SimGraphics m_SimGraphics;

        /// <summary>
        /// Camera Instance
        /// </summary>
        Camera m_Camera;

        /// <summary>
        /// Keeps the average time for the frame to render
        /// </summary>
        RenderTime m_FrameTime;

        /// <summary>
        /// Keeps the average time for the pixels to be processed
        /// </summary>
        RenderTime m_PixelTime;

        /// <summary>
        /// Keeps the average time for the vertices to be processed
        /// </summary>
        RenderTime m_VertexTime;

        /// <summary>
        /// Mesh Data Examples to be render
        /// </summary>
        MeshData m_MeshData;

    public:

        /// <summary>
        /// Viewer Constructor
        /// </summary>
        Viewer();

        /// <summary>
        /// Viewer Destructor
        /// </summary>
        ~Viewer();

        /// <summary>
        /// Renders the Examples
        /// </summary>
        void RenderFrame();

        /// <summary>
        /// Initializes the Viewer
        /// </summary>
        void Initialize(const XMUINT2& renderSize, uint32_t* frameBuffer);

        /// <summary>
        /// Gets the reference to the Simulate Graphics Class
        /// </summary>
        inline SimGraphics& GetSimGraphics()
        {
            return m_SimGraphics;
        }

        /// <summary>
        /// Gets the reference to the Mesh Data Class
        /// </summary>
        inline MeshData& GetMeshData()
        {
            return m_MeshData;
        }

        /// <summary>
        /// Get FPS
        /// </summary>
        inline float GetFPS() const
        {
            return m_FPS;
        }

        /// <summary>
        /// Get Frame Average Time
        /// </summary>
        inline float GetFrameAverageTime() const
        {
            return m_FrameTime.GetAverageTime();
        }

        /// <summary>
        /// Get Vertex Average Time
        /// </summary>
        inline float GetVertexAverageTime() const
        {
            return m_VertexTime.GetAverageTime();
        }

        /// <summary>
        /// Get Pixel Average Time
        /// </summary>
        inline float GetPixelAverageTime() const
        {
            return m_PixelTime.GetAverageTime();
        }
};

#endif
