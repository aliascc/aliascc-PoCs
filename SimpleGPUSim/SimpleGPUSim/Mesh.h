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

#ifndef _SHAPES_H
#define _SHAPES_H

/**********************
*   System Includes   *
***********************/
#include <vector>
#include <string>

/********************
*   Game Includes   *
*********************/
#include "Color.h"
#include "SimGraphicsDefs.h"

/// <summary>
/// Mesh Holder
/// </summary>
struct Mesh
{
	/// <summary>
	/// Vertex Buffer of the Mesh
	/// </summary>
	std::vector<Vertex> m_VtxBuffer;

	/// <summary>
	/// Index Buffer of the Mesh
	/// </summary>
	std::vector<uint32_t> m_IdxBuffer;

	/// <summary>
	/// Mesh Constructor
	/// </summary>
	Mesh();

	/// <summary>
	/// Load the Mesh to memory
	/// </summary>
	/// <param name="meshFile">Mesh File</param>
	void Load(const std::string& meshFile);
};

#endif