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
#include <math.h>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

/********************
*   Game Includes   *
*********************/
#include "MeshData.h"
#include "MathHelper.h"
#include "SimGraphics.h"

/********************
*   Function Defs   *
*********************/
MeshData::MeshData(const std::string& multipleSphereExample, const std::string& wolfExample, const std::string& singleSphereExample)
{
    m_WolfMesh.Load("WolfMesh.txt");
    m_SphereMesh.Load("SphereMesh.txt");

    LoadExample(multipleSphereExample, m_ExampleSphereMultiple);
    LoadExample(wolfExample, m_ExampleWolf);
    LoadExample(singleSphereExample, m_ExampleSphereSingle);
}

MeshData::~MeshData()
{
}

void MeshData::LoadExample(const std::string& exampleFile, std::vector<MeshElement>& example)
{
    std::ifstream exampleStream(exampleFile);

    std::string line;
    while (std::getline(exampleStream, line))
    {
        MeshElement meshElement;
        std::istringstream iss(line);

        if (!(iss >> meshElement.m_Position.x >> meshElement.m_Position.y >> meshElement.m_Position.z))
        {
            break;
        }

        float r = GetRandomFloat(0.0f, 1.0f);
        float g = GetRandomFloat(0.0f, 1.0f);
        float b = GetRandomFloat(0.0f, 1.0f);

        meshElement.m_Color = Color(r, g, b, 1.0f);

        example.emplace_back(meshElement);
    }
}

void MeshData::Render(SimGraphics& simGraphics, float rotation)
{
    switch (m_ExampleOpt)
    {
        case ExampleOpt::SphereMultiple:
            RenderSphereExample(simGraphics, m_ExampleSphereMultiple, rotation);
            break;

        case ExampleOpt::SphereSingle:
            RenderSphereExample(simGraphics, m_ExampleSphereSingle, rotation);
            break;

        case ExampleOpt::Wolf:
            RenderWolfExample(simGraphics, rotation);
            break;
    }
}

void MeshData::RenderSphereExample(SimGraphics& simGraphics, const std::vector<MeshElement>& sphereExample, float rotation)
{
    if (sphereExample.empty())
    {
        return;
    }

    //XMMATRIX rotationMat = XMMatrixRotationZ(rotation) * XMMatrixRotationX(rotation) * XMMatrixRotationY(rotation);
    XMMATRIX rotationMat = XMMatrixRotationZ(rotation);

    if (sphereExample.size() == 1)
    {
        XMVECTOR xmPos = XMLoadFloat3(&sphereExample[0].m_Position);
        XMMATRIX xmTranslate = XMMatrixTranslationFromVector(xmPos);

        XMMATRIX world = (xmTranslate * rotationMat);

        simGraphics.Draw(m_SphereMesh.m_VtxBuffer, m_SphereMesh.m_IdxBuffer, world, sphereExample[0].m_Color);
    }
    else
    {
        simGraphics.DrawMultiple(m_SphereMesh.m_VtxBuffer, m_SphereMesh.m_IdxBuffer, sphereExample, rotationMat);
    }
}

void MeshData::RenderWolfExample(SimGraphics& simGraphics, float rotation)
{
    if (m_ExampleWolf.empty())
    {
        return;
    }

    XMMATRIX rotationMat = XMMatrixRotationY(rotation * 2.0f);
    XMMATRIX scaleMat = XMMatrixScaling(6.5f, 6.5f, 6.5f);
    XMMATRIX world = rotationMat * scaleMat;

    if (m_ExampleWolf.size() == 1)
    {
        XMVECTOR xmPos = XMLoadFloat3(&m_ExampleWolf[0].m_Position);
        XMMATRIX xmTranslate = XMMatrixTranslationFromVector(xmPos);

        world = xmTranslate * world;

        simGraphics.Draw(m_WolfMesh.m_VtxBuffer, m_WolfMesh.m_IdxBuffer, world, m_ExampleWolf[0].m_Color);
    }
    else
    {
        simGraphics.DrawMultiple(m_WolfMesh.m_VtxBuffer, m_WolfMesh.m_IdxBuffer, m_ExampleWolf, world);
    }
}
