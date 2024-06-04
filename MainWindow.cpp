/*
 File: MainWindow.cpp
 Created on: 03/06/2024
 Author: Felix de las Pozas Alvarez

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// Project
#include <MainWindow.h>
#include <AboutDialog.h>

//----------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags)
{
  setupUi(this);

  connectSignals();

  loadSettings();
}

//----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
  saveSettings();
}

//----------------------------------------------------------------------------
void MainWindow::showAboutDialog()
{
  AboutDialog dialog(this);
  dialog.exec();
}

//----------------------------------------------------------------------------
void MainWindow::addItem()
{
}

//----------------------------------------------------------------------------
void MainWindow::removeItem()
{
}

//----------------------------------------------------------------------------
void MainWindow::showConfigurationDialog()
{
}

//----------------------------------------------------------------------------
void MainWindow::connectSignals()
{
  connect(this->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAboutDialog()));
  connect(this->actionExit_application, SIGNAL(triggered(bool)), this, SLOT(close()));
  connect(this->actionAdd_file_to_download, SIGNAL(triggered(bool)), this, SLOT(addItem()));
  connect(this->actionRemove_file, SIGNAL(triggered(bool)), this, SLOT(removeItem()));
  connect(this->actionApplication_settings, SIGNAL(triggered(bool)), this, SLOT(showConfigurationDialog()));
}

//----------------------------------------------------------------------------
void MainWindow::loadSettings()
{
}

//----------------------------------------------------------------------------
void MainWindow::saveSettings()
{
}
