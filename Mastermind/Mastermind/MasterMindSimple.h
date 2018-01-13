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

#ifndef __MASTER_MIND_SIMPLE_CPP
#define __MASTER_MIND_SIMPLE_CPP

/**********************
*   System Includes   *
***********************/
#include <list>

/**********************
*   System Includes   *
***********************/
#include "MasterMind.h"

/*****************
*   Class Decl   *
******************/

class MasterMindSimple : public MasterMind
{
	protected:

		std::list<Code> m_Combinations;

		std::list<Code> m_Posibilities;

		void InitCombinations();

	public:

		MasterMindSimple(uint32_t codeSize, uint32_t numColors);

		virtual ~MasterMindSimple();

		void StartNewGame() override;

		Code GetFirstGuess() override;

		Code GetNextGuess(const Code& currentGuess, const Clue& clue) override;
};

#endif