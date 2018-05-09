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
#include <Windows.h>

/********************
*   Game Includes   *
*********************/
#include "Timer.h"

/********************
*   Function Defs   *
*********************/
Timer::Timer()
{
    //Get seconds per count
    __int64 cntsPerSec = 0;
    QueryPerformanceFrequency((LARGE_INTEGER*)&cntsPerSec);

    m_SecondPerCount = 1.0f / (float)cntsPerSec;
}

Timer::~Timer()
{
}

double Timer::GetLapTime(bool updatePrevTime)
{
    __int64 currentTimeStamp = 0;

    QueryPerformanceCounter((LARGE_INTEGER*)&currentTimeStamp);

    double elapsedTime = ((currentTimeStamp - m_PrevTime) * m_SecondPerCount);

#if defined(_DEBUG)

    //If we are debugging and we are inspecting a break point 
    //it is very likely that elapsed time will be more than a sec
    //we do not want our engine to go faster than that, so do not
    //let it pass a sec
    if (elapsedTime > 1.0)
    {
        elapsedTime = 1.0;
    }

#endif

    if (updatePrevTime)
    {
        m_PrevTime = currentTimeStamp;
    }

    return elapsedTime;
}

void Timer::Update()
{
    m_ElapsedTime = GetLapTime(true);

    m_TotalElapsedTime += m_ElapsedTime;
}
