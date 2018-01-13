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
#include <exception>

/*********************
*   Local Includes   *
**********************/
#include "Code.h"

/****************
*   Variables   *
*****************/
static std::default_random_engine g_RandomGenerator((std::random_device())());

/*****************
*   Class Defs   *
******************/

Code::Code(uint32_t size)
{
	if (size < 2)
	{
		throw std::exception("Invalid size");
	}

	m_Colors.resize(size, 0);
}

Code::~Code()
{
}

Clue Code::CompareCode(const Code& other) const
{
	Clue clue;

	Code code1 = *this;
	Code code2 = other;

	size_t size = m_Colors.size();
	for (size_t i = 0; i < size; i++)
	{
		if (code1[i] == code2[i])
		{
			clue.m_BlackPegs++;
			code1[i] = -1;
			code2[i] = -2;
		}
	}

	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			if (code1[i] == code2[j])
			{
				clue.m_WhitePegs++;
				code1[i] = -3;
				code2[j] = -4;
				break;
			}
		}
	}

	return clue;
}

bool Code::ContainColors(const Code & other) const
{
	size_t size = m_Colors.size();
	for (size_t i = 0; i < size; i++)
	{
		for (size_t j = 0; j < size; j++)
		{
			if (m_Colors[i] == other[j])
			{
				return true;
			}
		}
	}

	return false;
}

int32_t& Code::operator[](size_t idx)
{
	if (idx > GetSize())
	{
		throw std::out_of_range("Invalid index for Code");
	}

	return m_Colors[idx];
}

int32_t Code::operator[](size_t idx) const
{
	if (idx > GetSize())
	{
		throw std::out_of_range("Invalid index for Code");
	}

	return m_Colors[idx];
}

static int32_t GetRandomInt(int32_t min, int32_t max)
{
	std::uniform_int_distribution<int32_t> disRand(min, max);

	return disRand(g_RandomGenerator);
}

Code Code::CreateRandomCode(uint32_t size, uint32_t numColors)
{
	Code code(size);

	for (uint32_t i = 0; i < size; i++)
	{
		code[i] = GetRandomInt(0, numColors - 1);
	}

	return code;
}

Code Code::CreateInvalidCode(uint32_t size)
{
	Code code(size);

	for (uint32_t i = 0; i < size; i++)
	{
		code[i] = -1;
	}

	return code;
}

std::ostream& operator<<(std::ostream& os, const Code& code)
{
	size_t size = code.GetSize();
	for (size_t i = 0; i < size; i++)
	{
		os << code[i];

		if ((i + 1) != size)
		{
			os << "|";
		}
	}

	return os;
}