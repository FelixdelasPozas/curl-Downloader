/*
 File: ConfigurationDialog.cpp
 Created on: 06/06/2024
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
#include <ConsoleOutputDialog.h>
#include <QTextCursor>

//----------------------------------------------------------------------------
ConsoleOutputDialog::ConsoleOutputDialog(QWidget *parent, Qt::WindowFlags f)
: QDialog(parent, f)
{
  setupUi(this);

  m_console->setReadOnly(true);
}

//----------------------------------------------------------------------------
void ConsoleOutputDialog::addText(const QString &text)
{
  if(m_console->toPlainText().length() > 100000)
  {
    m_console->clear();
    m_console->insertPlainText("Cleared console!\n");
  }

  m_console->insertPlainText(text);
  m_console->moveCursor(QTextCursor::MoveOperation::End);  
}
