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
#include <exception>

/*********************
*   Local Includes   *
**********************/
#include "MasterMind.h"

/*****************
*   Class Defs   *
******************/

MasterMind::MasterMind(uint32_t codeSize, uint32_t numColors)
	: m_CodeSize(codeSize)
	, m_NumColors(numColors)
{
	if (m_CodeSize < 2 || m_NumColors < 2)
	{
		throw std::exception("Invalid Number of Code Size and/or Number of Colors");
	}
}

MasterMind::~MasterMind()
{
}
