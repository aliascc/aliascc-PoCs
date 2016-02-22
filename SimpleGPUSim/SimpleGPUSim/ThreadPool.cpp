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
#include "ThreadPool.h"

/********************
*   Function Defs   *
*********************/
ThreadPool::ThreadPool()
{
	m_MaxThreads = std::thread::hardware_concurrency();

	for (uint32_t i = 0; i < m_MaxThreads; i++)
	{
		m_Pool.emplace_back(std::thread(&ThreadPool::Run, this));
	}
}

ThreadPool::~ThreadPool()
{
	Stop();
}

void ThreadPool::Run()
{
	TaskFunction task;
	while (true)
	{
		{
			std::unique_lock<std::mutex> lock(m_PoolMutex);

			m_Conditional.wait(lock, [this]{return !m_TaskQueue.empty() || !m_IsRunning; });

			if (m_TaskQueue.empty() && !m_IsRunning)
			{
				break;
			}

			task = m_TaskQueue.front();

			m_TaskQueue.pop();
		}

		task();
	}
}

void ThreadPool::AddTask(TaskFunction task)
{
	{
		std::unique_lock<std::mutex> lock(m_PoolMutex);
		
		m_TaskQueue.push(task);
	}

	m_Conditional.notify_one();
}

void ThreadPool::Stop()
{
	if (!m_IsRunning)
	{
		return;
	}

	{
		std::unique_lock<std::mutex> lock(m_PoolMutex);

		m_IsRunning = false;
	}

	m_Conditional.notify_all();

	for (std::thread& poolThread : m_Pool)
	{
		poolThread.join();
	}
	m_Pool.clear();
}
