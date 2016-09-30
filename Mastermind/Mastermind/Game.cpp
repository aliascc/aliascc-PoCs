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
#include <iostream>
#include <exception>

/*********************
*   Local Includes   *
**********************/
#include "Game.h"
#include "MasterMindSimple.h"

/*****************
*   Class Defs   *
******************/

Game::Game(GameType type, uint32_t codeSize, uint32_t numColors)
	: m_GameType(type)
	, m_CodeSize(codeSize)
	, m_NumColors(numColors)
{
	ChangeGameType(m_GameType);
}

Game::~Game()
{
}

uint32_t Game::AutoGuess()
{
	uint32_t guessAttempts = 0;
	Code mainCode = Code::CreateRandomCode(m_CodeSize, m_NumColors);

	m_MasterMind->StartNewGame();

	Code guess = m_MasterMind->GetFirstGuess();

	while (true)
	{
		guessAttempts++;

		Clue clue = mainCode.CompareCode(guess);

		if (clue.m_BlackPegs == m_CodeSize)
		{
			break;
		}

		guess = m_MasterMind->GetNextGuess(guess, clue);
	}

	std::cout << "Run Guess attempts: " << guessAttempts << std::endl;

	return guessAttempts;
}

void Game::ChangeGameType(GameType type)
{
	DeleteMem(m_MasterMind);

	m_GameType = type;

	switch (m_GameType)
	{
		case GameType::Simple:
			m_MasterMind = new MasterMindSimple(m_CodeSize, m_NumColors);
			break;

		case GameType::Knuft:
			break;

		case GameType::Genetic:
			break;
	}
}

void Game::AutoRun(uint32_t numRuns)
{
	if (m_MasterMind == nullptr)
	{
		throw std::invalid_argument("MasterMind is null");
	}

	std::cout << "AutoRun Started with number or runs: " << numRuns << std::endl;
	
	float average = 0;

	for (float i = 0; i < numRuns; i++)
	{
		average += AutoGuess();
	}

	average /= numRuns;

	std::cout << "AutoRun Guess Average attempts: " << average << std::endl;
}

void Game::Run()
{
	uint32_t guessAttempts = 0;

	m_MasterMind->StartNewGame();

	Code guess = m_MasterMind->GetFirstGuess();

	while (true)
	{
		guessAttempts++;

		std::cout << std::endl << "Guess #" << guessAttempts << ": " << guess << std::endl;

		Clue clue;

		std::cout << "Number of Black Pegs: ";
		std::cin >> clue.m_BlackPegs;
		clue.m_BlackPegs = (clue.m_BlackPegs <= m_CodeSize) ? clue.m_BlackPegs : m_CodeSize;

		if (clue.m_BlackPegs == m_CodeSize)
		{
			break;
		}

		std::cout << "Number of White Pegs: ";
		std::cin >> clue.m_WhitePegs;
		clue.m_WhitePegs = (clue.m_WhitePegs <= m_CodeSize) ? clue.m_WhitePegs : m_CodeSize;

		guess = m_MasterMind->GetNextGuess(guess, clue);
	}

	std::cout << "Run Guess attempts: " << guessAttempts << std::endl << std::endl;
}