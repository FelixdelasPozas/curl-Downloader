/*
 File: ItemWidget.cpp
 Created on: 05/06/2024
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
#include <ItemWidget.h>

//----------------------------------------------------------------------------
ItemWidget::ItemWidget(QWidget* parent, Qt::WindowFlags f)
: QWidget(parent, f)
{
  setupUi(this);
  connectSignals();
}

//----------------------------------------------------------------------------
ItemWidget::~ItemWidget()
{
  // Stop thread if running. 
}

//----------------------------------------------------------------------------
void ItemWidget::onCancelButtonPressed()
{
  // Stop thread if running. 
}

//----------------------------------------------------------------------------
void ItemWidget::onNotesButtonPressed()
{
  // Show thread console text. 
}

//----------------------------------------------------------------------------
void ItemWidget::onProgressModified(const unsigned int progressValue, const QString &text)
{
  QString progressText = text + " - " + QString::number(progressValue) + "%";
  m_progress->setValue(progressValue);
  m_progress->setFormat(text);
}

//----------------------------------------------------------------------------
void ItemWidget::connectSignals()
{
  connect(this->m_cancel, SIGNAL(pressed()), this, SLOT(onCancelButtonPressed()));
  connect(this->m_notes, SIGNAL(pressed()), this, SLOT(onNotesButtonPressed()));
}
