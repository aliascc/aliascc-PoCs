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

#ifndef SIMPLEGPUSIM_H
#define SIMPLEGPUSIM_H

/**********************
*   System Includes   *
***********************/
#include <QtWidgets/QMainWindow>

/********************
*   Game Includes   *
*********************/
#include "Viewer.h"
#include "ui_simplegpusim.h"

class SimpleGPUSim : public QMainWindow
{
	Q_OBJECT

	private:

		Ui::SimpleGPUSimClass ui;

		Viewer* m_Viewer;

		void paintEvent(QPaintEvent* event) override;

	private slots:

		void on_actionWolf_triggered();

		void on_actionSingleSphere_triggered();

		void on_actionMultipleSpheres_triggered();

		void on_actionSingleThreaded_triggered();

		void on_actionHardwareConcurrency_triggered();

		void on_actionEnableDisableZBufferTest_triggered();

		void on_actionEnableDisableBackFaceCulling_triggered();

	public:

		SimpleGPUSim(QWidget *parent = 0);
		~SimpleGPUSim();
};

#endif // SIMPLEGPUSIM_H
