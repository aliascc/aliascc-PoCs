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
#include "qimage.h"
#include "qpainter.h"
#include "simplegpusim.h"

/********************
*   Function Defs   *
*********************/
SimpleGPUSim::SimpleGPUSim(QWidget *parent)
	: QMainWindow(parent)
{
	m_Viewer = new Viewer();

	ui.setupUi(this);

	this->setFixedSize(WINDOW_HEIGHT, WINDOW_WIDTH);
}

SimpleGPUSim::~SimpleGPUSim()
{
}

void SimpleGPUSim::on_actionWolf_triggered()
{
	m_Viewer->GetMeshData().SetExampleOpt(ExampleOpt::Wolf);
}

void SimpleGPUSim::on_actionSingleSphere_triggered()
{
	m_Viewer->GetMeshData().SetExampleOpt(ExampleOpt::SphereSingle);
}

void SimpleGPUSim::on_actionMultipleSpheres_triggered()
{
	m_Viewer->GetMeshData().SetExampleOpt(ExampleOpt::SphereMultiple);
}

void SimpleGPUSim::on_actionSingleThreaded_triggered()
{
	m_Viewer->GetSimGraphics().SetMaxThreadsToUse(MaxThreads::SingleThreads);
}

void SimpleGPUSim::on_actionHardwareConcurrency_triggered()
{
	m_Viewer->GetSimGraphics().SetMaxThreadsToUse(MaxThreads::HardwareConcurrency);
}

void SimpleGPUSim::on_actionEnableDisableZBufferTest_triggered()
{
	m_Viewer->GetSimGraphics().ToggleZBufferActive();
}

void SimpleGPUSim::on_actionEnableDisableBackFaceCulling_triggered()
{
	m_Viewer->GetSimGraphics().ToggleBackFaceCullingActive();
}

void SimpleGPUSim::paintEvent(QPaintEvent* event)
{
	QPainter painter;

	if (painter.begin(this))
	{
		m_Viewer->RenderFrame(painter);

		painter.end();
	}

	this->update();
}
