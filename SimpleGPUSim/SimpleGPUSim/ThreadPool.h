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

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

/**********************
*   System Includes   *
***********************/
#include <queue>
#include <mutex>
#include <thread>
#include <vector>
#include <stdint.h>
#include <functional>
#include <condition_variable>

/// <summary>
/// Typedef Task Function
/// </summary>
typedef std::function<void()> TaskFunction;

/// <summary>
/// Thread Pool Class
/// </summary>
class ThreadPool
{
	private:

		/// <summary>
		/// Thread Pool vector
		/// </summary>
		std::vector<std::thread> m_Pool;

		/// <summary>
		/// Task Queue
		/// </summary>
		std::queue<TaskFunction> m_TaskQueue;

		/// <summary>
		/// Pool Mutex
		/// </summary>
		std::mutex m_PoolMutex;

		/// <summary>
		/// Max Threads available to run
		/// </summary>
		uint32_t m_MaxThreads = 0;

		/// <summary>
		/// Is the thread pool running
		/// </summary>
		bool m_IsRunning = true;

		/// <summary>
		/// Conditional variable, waiting for a notify to run a task
		/// </summary>
		std::condition_variable m_Conditional;

		/// <summary>
		/// Thread Run Method to wait for tasks to run
		/// </summary>
		void Run();

	public:

		/// <summary>
		/// Thread Pool Constructor
		/// </summary>
		ThreadPool();

		/// <summary>
		/// Thread Pool Destructor
		/// </summary>
		~ThreadPool();

		/// <summary>
		/// Get Max Threads the Pool can run simultaneously
		/// </summary>
		inline uint32_t GetMaxThreads() const
		{
			return m_MaxThreads;
		}

		/// <summary>
		/// Add a Task for the thread pool to execute
		/// </summary>
		/// <param name="task">Task to add to the thread pool</param>
		void AddTask(TaskFunction task);


		/// <summary>
		/// Stop the thread pool from running
		/// </summary>
		void Stop();
};

#endif
