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

#ifndef __CODE_CPP
#define __CODE_CPP

/**********************
*   System Includes   *
***********************/
#include <vector>
#include <cstdint>
#include <iostream>

/**********************
*   System Includes   *
***********************/
#include "MasterMindDefs.h"

/*****************
*   Class Decl   *
******************/

class Code final
{
	private:

		std::vector<int32_t> m_Colors;

	public:

		Code(uint32_t size);
		~Code();

		inline size_t GetSize() const
		{
			return m_Colors.size();
		}

		Clue CompareCode(const Code& other) const;

		bool ContainColors(const Code& other) const;

		int32_t& operator[](size_t idx);

		int32_t operator[](size_t idx) const;

		static Code CreateRandomCode(uint32_t size, uint32_t numColors);

		static Code CreateInvalidCode(uint32_t size);
};

std::ostream& operator<<(std::ostream& os, const Code& code);

#endif