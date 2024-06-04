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
}

//----------------------------------------------------------------------------
AddItemDialog::ItemInformation AddItemDialog::getItem() const
{
  return ItemInformation(QUrl(m_url->text()), m_serverIP->text(), m_serverPort->text().toUInt(), m_protocolCombo->currentIndex() == 0 ? Protocol::SOCKS4:Protocol::SOCKS5);
}

//----------------------------------------------------------------------------
void AddItemDialog::closeEvent(QCloseEvent *e)
{
  const ItemInformation item(QUrl(m_url->text()), m_serverIP->text(), m_serverPort->text().toUInt(), m_protocolCombo->currentIndex() == 0 ? Protocol::SOCKS4:Protocol::SOCKS5); 

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
bool AddItemDialog::ItemInformation::isValid() const
{
  QHostAddress address(server);
  // url.isValid() is a joke... everything goes.
  return !server.isEmpty() && url.isValid() && (QAbstractSocket::IPv4Protocol == address.protocol()) && port > 0;
}
