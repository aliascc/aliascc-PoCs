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

/*********************
*   Local Includes   *
**********************/
#include "MasterMindSimple.h"

/*****************
*   Class Defs   *
******************/

MasterMindSimple::MasterMindSimple(uint32_t codeSize, uint32_t numColors)
	: MasterMind(codeSize, numColors)
{
	InitCombinations();
}

MasterMindSimple::~MasterMindSimple()
{
}

void MasterMindSimple::InitCombinations()
{
	Code codeInit(m_CodeSize);

	uint32_t gcs = (uint32_t)std::pow(m_NumColors, m_CodeSize);

	m_Combinations.push_back(codeInit);

	Code prevCode = codeInit;
	Code code = codeInit;

	for (uint32_t i = 1; i < gcs; i++)
	{
		int32_t shiftpos = m_CodeSize - 1;
		bool add = true;

		prevCode = code;

		do
		{
			int32_t res = prevCode[shiftpos] + 1;

			if (res >= (int32_t)m_NumColors)
			{
				code[shiftpos] = 0;
				shiftpos--;
			}
			else
			{
				code[shiftpos] = res;
				add = false;
			}
		} while (add);
		m_Combinations.push_back(code);
	}
}

void MasterMindSimple::StartNewGame()
{
	m_Posibilities = m_Combinations;
}

Code MasterMindSimple::GetFirstGuess()
{
	Code code(m_CodeSize);

	uint32_t first_half = m_CodeSize / 2;

	for (uint32_t i = 0; i < first_half; i++)
	{
		code[i] = 0;
	}

	for (uint32_t i = first_half; i < m_CodeSize; i++)
	{
		code[i] = 1;
	}

	return code;
}

Code MasterMindSimple::GetNextGuess(const Code& currentGuess, const Clue& clue)
{
	if (m_Posibilities.empty())
	{
		return Code::CreateInvalidCode(m_CodeSize);
	}

	auto it = m_Posibilities.begin();

	while (it != m_Posibilities.end())
	{
		if (clue.Zeros())
		{
			if (currentGuess.ContainColors(*it))
			{
				auto temp_it = it;
				++it;

				m_Posibilities.erase(temp_it);
			}
			else
			{
				++it;
			}
		}
		else
		{
			Clue cmp_clue = currentGuess.CompareCode(*it);

			if (cmp_clue != clue)
			{
				auto temp_it = it;
				++it;

				m_Posibilities.erase(temp_it);
			}
			else
			{
				++it;
			}
		}
	}

	if (m_Posibilities.empty())
	{
		return Code::CreateInvalidCode(m_CodeSize);
	}

	if (m_Posibilities.size() > 3)
	{
		size_t midpoint = m_Posibilities.size() / 2;
		auto it = m_Posibilities.begin();


		for (size_t i = 0; i < midpoint; i++, it++);

		return *it;
	}

	return m_Posibilities.front();
}
