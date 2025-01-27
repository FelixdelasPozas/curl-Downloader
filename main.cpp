/*
 File: main.cpp
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

// Qt
#include <QApplication>
#include <QSharedMemory>
#include <QMessageBox>
#include <QIcon>
#include <QEventLoop>
#include <QDebug>

// C++
#include <iostream>

//-----------------------------------------------------------------
void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  const char symbols[] = { 'I', 'E', '!', 'X' };
//  QString output = QString("[%1] %2 (%3:%4 -> %5)").arg( symbols[type] ).arg( msg ).arg(context.file).arg(context.line).arg(context.function);
  QString output = QString("[%1] %2").arg( symbols[type] ).arg( msg );
  std::cerr << output.toStdString() << std::endl;
  if(type == QtFatalMsg) abort();
}

//-----------------------------------------------------------------------------
int wmain(int argc, wchar_t *argv[], wchar_t *envp[])
{
  qInstallMessageHandler(myMessageOutput);

  QApplication app(__argc, __argv);
  app.setQuitOnLastWindowClosed(false);

  // allow only one instance
  QSharedMemory guard;
  guard.setKey("CurlDownloader");

  if (!guard.create(1))
  {
    QMessageBox msgBox;
    msgBox.setWindowIcon(QIcon(":/Downloader/download.svg"));
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText("curl Downloader is already running!");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
    exit(0);
  }
 
  MainWindow application;
  application.show();

  return app.exec();
}
