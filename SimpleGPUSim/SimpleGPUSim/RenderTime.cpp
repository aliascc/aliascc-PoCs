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

/********************
*   Game Includes   *
*********************/
#include "RenderTime.h"

/********************
*   Function Defs   *
*********************/
RenderTime::RenderTime()
{
}

void RenderTime::AddTime(double ms)
{
	uint32_t size = m_TimeHistory.size();
	if (size >= NUM_FRAME_SAVED)
	{
		m_TimeHistory.pop_front();
	}

	m_TimeHistory.push_back(ms);

	double sumMS = 0;
	for(double msTime : m_TimeHistory)
	{
		sumMS += msTime;
	}
	sumMS /= (double)size;

	m_AverageTime = sumMS;
}