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

#ifndef _CAMERA_H
#define _CAMERA_H

/********************
*   Game Includes   *
*********************/
#include "MathHelper.h"

/// <summary>
/// Camera Class
/// </summary>
class Camera
{
    private:

        /// <summary>
        /// Position of the Camera
        /// </summary>
        XMFLOAT3 m_Position = XMFLOAT3ZERO;

        /// <summary>
        /// Target of the Camera
        /// </summary>
        XMFLOAT3 m_Target = XMFLOAT3ZERO;

        /// <summary>
        /// Up Vector of the Camera
        /// </summary>
        XMFLOAT3 m_UPVector = XMFLOAT3UP;

        /// <summary>
        /// Projection Matrix of the Camera
        /// </summary>
        XMFLOAT4X4 m_ProjectionMatrix = XMFLOAT4X4IDENTITY;

        /// <summary>
        /// View Matrix of the Camera
        /// </summary>
        XMFLOAT4X4 m_ViewMatrix = XMFLOAT4X4IDENTITY;

        /// <summary>
        /// Dimensions of the Screen
        /// </summary>
        XMUINT2 m_Dimensions = XMUINT2ZERO;

        /// <summary>
        /// Rotation of the Camera
        /// </summary>
        XMFLOAT3 m_Rotation = XMFLOAT3ZERO;

        /// <summary>
        /// Forward Vector of the Camera
        /// </summary>
        XMFLOAT3 m_FwrVector = XMFLOAT3FWRLH;

        /// <summary>
        /// Z Near Point
        /// </summary>
        float m_ZNear = 0.0f;

        /// <summary>
        /// Z Far Point
        /// </summary>
        float m_ZFar = 0.0f;

        /// <summary>
        /// Field of view of the Camera
        /// </summary>
        float m_FoV = 0.0f;

        /// <summary>
        /// Create View Matrix of the Camera
        /// </summary>
        void CreateView();

        /// <summary>
        /// Create Projection Matrix of the Camera
        /// </summary>
        void CreateProjection();

    public:

        /// <summary>
        /// Camera Constructor
        /// </summary>
        Camera();

        /// <summary>
        /// Camera Destructor
        /// </summary>
        ~Camera();

        /// <summary>
        /// Initializes the Camera
        /// </summary>
        /// <param name="position">Camera Position</param>
        /// <param name="target">Camera Target</param>
        /// <param name="UP">Up Vector</param>
        /// <param name="dimensions">Dimension of the Camera</param>
        /// <param name="fov">Field of View</param>
        /// <param name="znear">Z Near Point</param>
        /// <param name="zfar">Z Far Point</param>
        void Initialize(const XMFLOAT3& position, const XMFLOAT3& target, const XMFLOAT3& UP, const XMUINT2& dimensions, float fov, float znear, float zfar);

        /// <summary>
        /// Get Projection Matrix
        /// </summary>
        inline const XMFLOAT4X4&    GetProjectionMatrix() const
        {
            return m_ProjectionMatrix;
        }

        /// <summary>
        /// Get View Matrix
        /// </summary>
        inline const XMFLOAT4X4& GetViewMatrix() const
        {
            return m_ViewMatrix;
        }

        /// <summary>
        /// Get Position
        /// </summary>
        inline const XMFLOAT3& GetPosition() const
        {
            return m_Position;
        }

        /// <summary>
        /// Get Forward Vector
        /// </summary>
        inline const XMFLOAT3& GetFwrVector() const
        {
            return m_FwrVector;
        }

        /// <summary>
        /// Get Dimensions
        /// </summary>
        inline const XMUINT2& GetDimensions() const
        {
            return m_Dimensions;
        }

        /// <summary>
        /// Get Z Near Point
        /// </summary>
        inline float GetZNear() const
        {
            return m_ZNear;
        }

        /// <summary>
        /// Get Z Far Point
        /// </summary>
        inline float GetZFar() const
        {
            return m_ZFar;
        }

        /// <summary>
        /// Get Camera Up Vector
        /// </summary>
        inline const XMFLOAT3& GetUpVector() const
        {
            return m_UPVector;
        }
};

#endif
