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

#ifndef _SIM_GRAPHIC_DEFS
#define _SIM_GRAPHIC_DEFS

#include "Color.h"
#include "MathHelper.h"

#define WINDOW_HEIGHT	1024
#define WINDOW_WIDTH	768

/// <summary>
/// Enum that define options on threads to run
/// </summary>
enum class MaxThreads : uint32_t
{
	SingleThreads = 0,
	HardwareConcurrency
};

/// <summary>
/// Element defining the mesh position and color
/// </summary>
struct MeshElement
{
	/// <summary>
	/// Mesh Position
	/// </summary>
	XMFLOAT3 m_Position;

	/// <summary>
	/// Mesh Color
	/// </summary>
	Color m_Color;
};

/// <summary>
/// Vertex Definition
/// </summary>
struct Vertex
{
	/// <summary>
	/// Position of the Vertex
	/// </summary>
	XMFLOAT3 m_Position;

	/// <summary>
	/// Normal of the Vertex
	/// </summary>
	XMFLOAT3 m_Normal;

	/// <summary>
	/// Color of the Vertex
	/// </summary>
	Color m_Color;

	/// <summary>
	/// Vertex Constructor
	/// </summary>
	Vertex()
	{
	}

	/// <summary>
	/// Vertex Constructor
	/// </summary>
	/// <param name="pos">Position of the vertex</param>
	/// <param name="normal">Normal of the vertex</param>
	/// <param name="color">Color of the vertex</param>
	Vertex(const XMFLOAT3& pos, const XMFLOAT3& normal, const Color& color)
		: m_Position(pos)
		, m_Normal(normal)
		, m_Color(color)
	{
	}
};

/// <summary>
/// Output Vertex Definition
/// </summary>
struct OutPutVertex
{
	/// <summary>
	/// Position of the Vertex in clip space
	/// </summary>
	XMFLOAT4 m_Position;

	/// <summary>
	/// Normal of the vertex
	/// </summary>
	XMFLOAT3 m_Normal;

	/// <summary>
	/// Position of the Vertex in World Space
	/// </summary>
	XMFLOAT3 m_PositionWS;

	/// <summary>
	/// Color of the Vertex
	/// </summary>
	Color m_Color;
};

/// <summary>
/// Bounding Box Definition
/// </summary>
struct BoundingBox
{
	/// <summary>
	/// Max X Pixel Position
	/// </summary>
	uint32_t m_MaxX = 0;

	/// <summary>
	/// Min X Pixel Position
	/// </summary>
	uint32_t m_MinX = 0;

	/// <summary>
	/// Max Y Pixel Position
	/// </summary>
	uint32_t m_MaxY = 0;

	/// <summary>
	/// Min Y Pixel Position
	/// </summary>
	uint32_t m_MinY = 0;
};

/// <summary>
/// Face Definition
/// </summary>
struct Face
{
	/// <summary>
	/// Output Vertex
	/// </summary>
	OutPutVertex m_Vertex[3];

	/// <summary>
	/// Color of the Face
	/// </summary>
	Color m_Color;

	/// <summary>
	/// Bounding Box for the Face
	/// </summary>
	BoundingBox m_BoundingBox;

	/// <summary>
	/// Area of the Face
	/// </summary>
	float m_Area = 0.0f;

	/// <summary>
	/// Is the face hidden from the camera
	/// </summary>
	bool m_Hidden = false;
};

#endif
