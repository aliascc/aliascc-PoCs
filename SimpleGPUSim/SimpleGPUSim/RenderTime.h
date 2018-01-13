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

#ifndef _PROJECT_DEFS_H
#define _PROJECT_DEFS_H

/**********************
*   System Includes   *
***********************/
#include <list>
#include <cstring>
#include <stdint.h>

/**************
*   Defines   *
***************/
#define NUM_FRAME_SAVED 60

/// <summary>
/// Render Time Average Handler
/// </summary>
class RenderTime
{
	private:

		/// <summary>
		/// History of Render Times
		/// </summary>
		std::list<double> m_TimeHistory;

		/// <summary>
		/// Average Frame Time
		/// </summary>
		double m_AverageTime = 0.0;

	public:

		/// <summary>
		/// Render Time Constructor
		/// </summary>
		RenderTime();

		/// <summary>
		/// Add a Render Time 
		/// </summary>
		/// <param name="ms">Render Time to add in ms</param>
		void AddTime(double ms);

		/// <summary>
		/// Get the Average Render Time
		/// </summary>
		inline double GetAverageTime() const
		{
			return m_AverageTime;
		}
};

#endif
