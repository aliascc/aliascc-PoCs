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

#ifndef __MASTER_MIND_DEFS_CPP
#define __MASTER_MIND_DEFS_CPP

/**********************
*   System Includes   *
***********************/
#include <cstdint>

/**************
*   Defines   *
***************/

#define DeleteMem(x) if(x != nullptr) { delete x; }

/************
*   Enums   *
*************/

enum class GameType : uint32_t
{
	Simple,
	Knuft,
	Genetic
};

/**************
*   Structs   *
***************/

struct Clue 
{
	uint32_t m_BlackPegs = 0;
	uint32_t m_WhitePegs = 0;

	bool Zeros() const
	{
		return (m_BlackPegs == 0 && m_WhitePegs == 0);
	}

	bool operator==(const Clue& other)
	{
		return (this->m_BlackPegs == other.m_BlackPegs &&
				this->m_WhitePegs == other.m_WhitePegs);
	}

	bool operator!=(const Clue& other)
	{
		return !(*this == other);
	}
};

#endif

