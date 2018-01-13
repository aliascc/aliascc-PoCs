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
#include <random>

/***************************
*   Game Engine Includes   *
****************************/
#include "MathHelper.h"

/****************
*   Constants   *
*****************/
const XMUINT2 XMUINT2ZERO(0, 0);

const XMFLOAT3 XMFLOAT3ZERO(0.0f, 0.0f, 0.0f);
const XMFLOAT3 XMFLOAT3ONE(1.0f, 1.0f, 1.0f);
const XMFLOAT3 XMFLOAT3UP(0.0f, 1.0f, 0.0f);
const XMFLOAT3 XMFLOAT3FWRLH(0.0f, 0.0f, 1.0f);


const XMFLOAT4X4 XMFLOAT4X4IDENTITY(1.0f, 0.0f, 0.0f, 0.0f,
																		0.0f, 1.0f, 0.0f, 0.0f,
																		0.0f, 0.0f, 1.0f, 0.0f,
																		0.0f, 0.0f, 0.0f, 1.0f);

/*********************
*   Variables Defs   *
**********************/
std::default_random_engine gRandomGenerator((std::random_device())());

/********************
*   Function Defs   *
*********************/
float GetRandomFloat(float min, float max)
{
	std::uniform_real_distribution<float> disRand(min, max);

	return disRand(gRandomGenerator);
}

int32_t GetRandomInt(int32_t min, int32_t max)
{
	std::uniform_int_distribution<int32_t> disRand(min, max);

	return disRand(gRandomGenerator);
}

uint32_t GetRandomUInt(uint32_t min, uint32_t max)
{
	std::uniform_int_distribution<uint32_t> disRand(min, max);

	return disRand(gRandomGenerator);
}
