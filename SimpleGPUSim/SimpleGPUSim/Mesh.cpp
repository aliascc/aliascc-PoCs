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
#include <sstream>
#include <fstream>

/********************
*   Game Includes   *
*********************/
#include "Mesh.h"

/********************
*   Function Defs   *
*********************/
Mesh::Mesh()
{
}

void Mesh::Load(const std::string& meshFile)
{
	std::string line;
	uint32_t count = 0;

	m_VtxBuffer.clear();
	m_IdxBuffer.clear();

	std::ifstream exampleStream(meshFile);

	//Get Size of Vertex Buffer and Index Buffer
	if (std::getline(exampleStream, line))
	{
		std::istringstream iss(line);
		uint32_t vtxSize;
		uint32_t idxSize;

		if (!(iss >> vtxSize >> idxSize))
		{
			return;
		}

		m_VtxBuffer.resize(vtxSize);
		m_IdxBuffer.resize(idxSize);
	}
	else
	{
		return;
	}

	//Load Vertex
	count = 0;
	while (count < m_VtxBuffer.size() && std::getline(exampleStream, line))
	{
		std::istringstream iss(line);
		Vertex& vtx = m_VtxBuffer[count];

		if (!(iss >> vtx.m_Position.x >> vtx.m_Position.y >> vtx.m_Position.z >> vtx.m_Normal.x >> vtx.m_Normal.y >> vtx.m_Normal.z))
		{
			return;
		}

		++count;
	}

	//Load Index
	count = 0;
	while (count < m_IdxBuffer.size() && std::getline(exampleStream, line))
	{
		std::istringstream iss(line);

		if (!(iss >> m_IdxBuffer[count]))
		{
			return;
		}

		++count;
	}
}
