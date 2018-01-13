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

#ifndef _MATH_HELPER_H
#define _MATH_HELPER_H

/**********************
*   System Includes   *
***********************/
#include <vector>
#include <stdint.h>
#include <algorithm>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#undef min
#undef max

/************
*   Using   *
*************/
using namespace DirectX;
using namespace PackedVector;

/****************
*   Constants   *
*****************/

extern const XMUINT2 XMUINT2ZERO;

extern const XMFLOAT3 XMFLOAT3ZERO;
extern const XMFLOAT3 XMFLOAT3ONE;
extern const XMFLOAT3 XMFLOAT3UP;
extern const XMFLOAT3 XMFLOAT3FWRLH;

extern const XMFLOAT4X4 XMFLOAT4X4IDENTITY;

/****************
*   Functions   *
*****************/

/// <summary>
/// Get a Random Float Number
/// </summary>
/// <param name="min">Minimum float range</param>
/// <param name="max">Maximum float range</param>
/// <returns>Random Float Number</returns>
extern float GetRandomFloat(float min, float max);

/// <summary>
/// Get a Random Int Number
/// </summary>
/// <param name="min">Minimum int range</param>
/// <param name="max">Maximum int range</param>
/// <returns>Random int Number</returns>
extern int32_t GetRandomInt(int32_t min, int32_t max);

/// <summary>
/// Get a Random uint Number
/// </summary>
/// <param name="min">Minimum uint range</param>
/// <param name="max">Maximum uint range</param>
/// <returns>Random uint Number</returns>
extern uint32_t GetRandomUInt(uint32_t min, uint32_t max);

/// <summary>
/// Creates a View Matrix for the Camera
/// </summary>
/// <param name="cameraPosition">Camera Position</param>
/// <param name="cameraTarget">Camera Target</param>
/// <param name="cameraUP">Camera Up Vector</param>
/// <param name="lookAt">View Matrix Result</param>
inline void CreateLookAtLH(const XMFLOAT3& cameraPosition, const XMFLOAT3& cameraTarget, const XMFLOAT3& cameraUP, XMFLOAT4X4& lookAt)
{
	XMVECTOR cp = XMLoadFloat3(&cameraPosition);
	XMVECTOR ct = XMLoadFloat3(&cameraTarget);
	XMVECTOR cu = XMLoadFloat3(&cameraUP);

	XMMATRIX xmMatrix = XMMatrixLookAtLH(cp, ct, cu);

	XMStoreFloat4x4(&lookAt, xmMatrix);
}

/// <summary>
/// Creates a Projection Matrix for the Camera
/// </summary>
/// <param name="fov">Field of view</param>
/// <param name="aspectRatio">Aspect Ration of the Camera</param>
/// <param name="nearZ">Near Z Depth</param>
/// <param name="farZ">Far Z Depth</param>
inline void CreatePrespectiveFovLH(float fov, float aspectRatio, float nearZ, float farZ, XMFLOAT4X4& proj)
{
	XMMATRIX xmMatrix = XMMatrixPerspectiveFovLH(fov, aspectRatio, nearZ, farZ);

	XMStoreFloat4x4(&proj, xmMatrix);
}

/// <summary>
/// Get the minimum of 3 numbers
/// </summary>
/// <param name="a">Number A to compare</param>
/// <param name="b">Number B to compare</param>
/// <param name="c">Number C to compare</param>
/// <returns>minimum number</returns>
inline float Minimum(float a, float b, float c)
{
	return std::min(a, std::min(b, c));
}

/// <summary>
/// Get the maximum of 3 numbers
/// </summary>
/// <param name="a">Number A to compare</param>
/// <param name="b">Number B to compare</param>
/// <param name="c">Number C to compare</param>
/// <returns>maximum number</returns>
inline float Maximum(float a, float b, float c)
{
	return std::max(a, std::max(b, c));
}

/// <summary>
/// Calculates on which side the point (C) is of the line (A-B)
/// Also Helpful in calculating Barycentric Coordinates
/// </summary>
/// <param name="a">Point A</param>
/// <param name="b">Point B</param>
/// <param name="c">Point C</param>
/// <returns>maximum number</returns>
inline float EdgeFunction(const XMFLOAT4& v1, const XMFLOAT4& v2, const XMFLOAT4& v3)
{
	//CCW: EAB(P)=(A.x−B.x)∗(P.y−A.y)−(A.y−B.y)∗(P.x−A.x)
	//float edge = (v1.x - v2.x) * (v3.y - v1.y) - (v1.y - v2.y) * (v3.x - v1.x);

	//CW: EAB(P) = (P.x−A.x)∗(B.y−A.y)−(P.y−A.y)∗(B.x−A.x)
	float edge = (v3.x - v1.x) * (v2.y - v1.y) - (v3.y - v1.y) * (v2.x - v1.x);

	return edge;
}

template<typename T>
T clamp(const T &val, const T &min, const T &max)
{
	return std::max(min, std::min(max, val));
}

#endif