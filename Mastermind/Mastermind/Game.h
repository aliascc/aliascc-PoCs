/*
* Copyright (c) 2016 <Carlos Chac�n>
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

#ifndef __GAME_CPP
#define __GAME_CPP

/**********************
*   System Includes   *
***********************/
#include <cstdint>

/*********************
*   Local Includes   *
**********************/
#include "MasterMindDefs.h"

/*******************
*   Forward Decl   *
********************/
class MasterMind;

/*****************
*   Class Decl   *
******************/

class Game final
{
	private:

		uint32_t m_CodeSize = 4;

		uint32_t m_NumColors = 6;

		GameType m_GameType = GameType::Simple;

		MasterMind* m_MasterMind = nullptr;
		
		uint32_t AutoGuess();

	public:

		Game(GameType type, uint32_t codeSize, uint32_t numColors);

		~Game();

		void ChangeGameType(GameType type);

		void AutoRun(uint32_t numRuns);

		void Run();
};

#endif

