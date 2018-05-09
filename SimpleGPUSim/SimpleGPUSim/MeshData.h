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

/**********************
*   System Includes   *
***********************/
#include <vector>

/********************
*   Game Includes   *
*********************/
#include "Mesh.h"
#include "SimGraphicsDefs.h"

/*******************
*   Forward Decl   *
********************/
class SimGraphics;

/// <summary>
/// Example Render Option is to be render
/// </summary>
enum class ExampleOpt : uint32_t
{
    SphereMultiple = 0,
    SphereSingle,
    Wolf
};

/// <summary>
/// Mesh Data Holder
/// </summary>
class MeshData
{
    private:

        /// <summary>
        /// Sphere Mesh
        /// </summary>
        Mesh m_SphereMesh;

        /// <summary>
        /// Wolf Mesh
        /// </summary>
        Mesh m_WolfMesh;

        /// <summary>
        /// Example Render Option
        /// </summary>
        ExampleOpt m_ExampleOpt = ExampleOpt::Wolf;

        /// <summary>
        /// Example vector containing the Multiple Spheres
        /// </summary>
        std::vector<MeshElement> m_ExampleSphereMultiple;

        /// <summary>
        /// Example vector containing a Single Sphere
        /// </summary>
        std::vector<MeshElement> m_ExampleSphereSingle;

        /// <summary>
        /// Example vector containing a Wolf
        /// </summary>
        std::vector<MeshElement> m_ExampleWolf;

        /// <summary>
        /// Loads the Example to Memory
        /// </summary>
        /// <param name="exampleFile">Filepath</param>
        /// <param name="example">Example on where to load</param>
        void LoadExample(const std::string& exampleFile, std::vector<MeshElement>& example);

        /// <summary>
        /// Renders the Sphere Example
        /// </summary>
        /// <param name="simGraphics">SimGraphics use to render</param>
        /// <param name="sphereExample">Sphere Example</param>
        /// <param name="rotation">Rotation to render</param>
        void RenderSphereExample(SimGraphics& simGraphics, const std::vector<MeshElement>& sphereExample, float rotation);

        /// <summary>
        /// Renders the Wolf Example
        /// </summary>
        /// <param name="simGraphics">SimGraphics use to render</param>
        /// <param name="rotation">Rotation to render</param>
        void RenderWolfExample(SimGraphics& simGraphics, float rotation);

    public:

        /// <summary>
        /// MeshData Constructor
        /// </summary>
        MeshData(const std::string& multipleSphereExample, const std::string& wolfExample, const std::string& singleSphereExample);

        /// <summary>
        /// MeshData Destructor
        /// </summary>
        ~MeshData();

        /// <summary>
        /// Set which Example Render Option is to be render
        /// </summary>
        inline void SetExampleOpt(ExampleOpt exampleOpt)
        {
            m_ExampleOpt = exampleOpt;
        }

        /// <summary>
        /// Renders the Example
        /// </summary>
        /// <param name="simGraphics">SimGraphics use to render</param>
        /// <param name="rotation">Rotation of the render</param>
        void Render(SimGraphics &simGraphics, float rotation);
};
