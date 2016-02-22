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

#ifndef _TIMER_H
#define _TIMER_H

/**********************
*   System Includes   *
***********************/
#include <stdint.h>

/// <summary>
/// Timer Class to keep track of time between frames
/// </summary>
class Timer
{

	private:

		/// <summary>
		/// Elapsed Time since Last Frame
		/// </summary>
		double m_ElapsedTime = 0.0;

		/// <summary>
		/// Total running time since the start of the Game App
		/// </summary>
		double m_TotalElapsedTime = 0.0;

		/// <summary>
		/// Keeps previous Time Stamp, to measure time between calls
		/// </summary>
		__int64 m_PrevTime = 0;

		/// <summary>
		/// Stores the Freq of Counts per Second of the system
		/// </summary>
		double m_SecondPerCount = 0.0;

		/// <summary>
		/// Gets the time Since last call. Works like a lap in a chronometer
		/// </summary>
		/// <param name="updatePrevTime">If true, updates Previous time with current time</param>
		/// <returns>Returns time since last call and Previous time updated</returns>
		double GetLapTime(bool updatePrevTime);

	public:

		/// <summary>
		/// Timer Constructor
		/// </summary>
		Timer();

		/// <summary>
		/// Timer Destructor
		/// </summary>
		~Timer();

		/// <summary>
		/// Gets Elapsed Time since Last Frame
		/// </summary>
		inline float GetElapsedTime() const
		{
			return m_ElapsedTime;
		}

		/// <summary>
		/// Gets Total running time since the start of the Game App
		/// </summary>
		inline float GetTotalElapsedTime() const
		{
			return m_TotalElapsedTime;
		}

		/// <summary>
		/// Tells the Timer to update it self and all the properties
		/// </summary>
		void Update();
};

#endif