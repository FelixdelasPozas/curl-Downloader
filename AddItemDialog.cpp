/*
 File: AddItemDialog.cpp
 Created on: 04/06/2024
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
#include <AddItemDialog.h>
#include <QCloseEvent>
#include <QMessageBox>
#include <QHostAddress>
#include <QAbstractSocket>

//----------------------------------------------------------------------------
AddItemDialog::AddItemDialog(QWidget *parent, Qt::WindowFlags f)
: QDialog(parent, f)
{
  setupUi(this);

  connect(m_serverIP, SIGNAL(textChanged(const QString &)), this, SLOT(onServerTextChanged()));
}

//----------------------------------------------------------------------------
Utils::ItemInformation AddItemDialog::getItem() const
{
  return Utils::ItemInformation(QUrl(m_url->text()), 
                                m_serverIP->text(), 
                                m_serverPort->text().toUInt(), 
                                static_cast<Utils::Protocol>(m_protocolCombo->currentIndex()));
}

//----------------------------------------------------------------------------
void AddItemDialog::closeEvent(QCloseEvent *e)
{
  const Utils::ItemInformation item(QUrl(m_url->text()), 
                                    m_serverIP->text(), 
                                    m_serverPort->text().toUInt(), 
                                    static_cast<Utils::Protocol>(m_protocolCombo->currentIndex()));

  if(!item.isValid())
  {
    e->setAccepted(false);
    e->ignore();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Item information");
    msgBox.setStandardButtons(QMessageBox::Button::Ok);
    msgBox.setText("The item information is not valid.");
    msgBox.exec();

    return;
  }

  accept();
}

//----------------------------------------------------------------------------
void AddItemDialog::onServerTextChanged()
{
  const auto enabled = !m_serverIP->text().isEmpty();
  m_serverPort->setEnabled(enabled);
  m_protocolCombo->setEnabled(enabled);
}
