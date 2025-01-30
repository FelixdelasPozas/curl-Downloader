/*
 File: Utils.cpp
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
#include <Utils.h>
#include <ItemWidget.h>

// Qt
#include <QHostAddress>
#include <QApplication>
#include <QProcess>
#include <QDir>
											 
//----------------------------------------------------------------------------
bool Utils::ItemInformation::isValid() const
{
  QHostAddress address(server);
  // url.isValid() is a joke... everything goes. Server and port can be empty.
  return !url.isEmpty() && url.isValid() && (server.isEmpty() || (QAbstractSocket::UnknownNetworkLayerProtocol != address.protocol()));
}

//----------------------------------------------------------------------------
QString Utils::ItemInformation::toText() const
{
  QString text;
  text += QString("File: %1\n").arg(url.toString());
  if(!server.isEmpty())
  {
    text += QString("Proxy server: %1:%2\n").arg(server).arg(port);
    text += QString("Protocol: %1\n").arg(protocol == Protocol::SOCKS4 ? "SOCKS4": (protocol == Protocol::NONE) ? "None":"SOCKS5");
  }
  else
    text += QString("Proxy server: None\n");

  text += "Output name: " + outputName;
  
  return text;
}

//----------------------------------------------------------------------------
bool Utils::ItemInformation::operator==(const ItemInformation &other)
{
  return (url == other.url) && (server == other.server) && (port == other.port) && (protocol == other.protocol) && (outputName == other.outputName);
}

//----------------------------------------------------------------------------
std::vector<Utils::ItemInformation *>::const_iterator Utils::findItem(const QUrl &url, const std::vector<Utils::ItemInformation *> &items)
{
  auto isSameUrl = [&url](const Utils::ItemInformation *item)
  {
    return url == item->url;
  };
  auto it = std::find_if(items.cbegin(), items.cend(), isSameUrl);
  return it;
}

//----------------------------------------------------------------------------
QString Utils::curlExecutableVersion(const QString &exePath)
{
  QProcess process;
  process.start(exePath,
                QStringList{"--version"},
                QIODevice::OpenMode::enum_type::ReadOnly);
  process.waitForFinished();

  if (process.exitCode() != 0)
    return QString();

  auto outputText = process.readAllStandardOutput();

  if(!outputText.startsWith("curl"))
    return QString();

  outputText = outputText.split('\n').first();

  return outputText.split(' ').at(1);
}

//----------------------------------------------------------------------------
ItemWidget *Utils::findWidgetWithButton(const QToolButton *button, std::vector<ItemWidget *> widgets)
{
  for(auto w: widgets)
  {
    auto buttons = w->findChildren<QToolButton*>();
    auto it = std::find_if(buttons.cbegin(), buttons.cend(), [button](const QToolButton *b){ return b == button; });
    if(it != buttons.cend())
      return w;
  }

  return nullptr;
}

//----------------------------------------------------------------------------
bool Utils::Configuration::isValid() const
{
  QDir directory(downloadPath);
  return !curlPath.isEmpty() && !downloadPath.isEmpty() && directory.exists() && waitSeconds >= 5 && !curlExecutableVersion(curlPath).isEmpty();
}

//----------------------------------------------------------------------------
void Utils::AutoCloseMessageBox::showEvent(QShowEvent *event)
{   
  QMessageBox::showEvent(event);
  QApplication::beep();
  m_text = text();

  m_currentTime = 0;
  if (m_autoClose)
    this->startTimer(1000); // counting is done in 'timerEvent'.
}

//----------------------------------------------------------------------------
void Utils::AutoCloseMessageBox::timerEvent(QTimerEvent *event)
{
  m_currentTime++; // counting.

  if(m_autoClose)
  {
    setText(m_text + QString("\nThis dialog will close in %1 seconds.").arg(m_closeSeconds - m_currentTime));
    if(m_currentTime >= m_closeSeconds)
      this->done(0);
  }
}

//----------------------------------------------------------------------------
void Utils::AutoCloseMessageBox::setAutoClose(const bool value)
{
  m_autoClose = value;
};

//----------------------------------------------------------------------------
void Utils::AutoCloseMessageBox::setCloseTime(const unsigned int seconds)
{
  m_closeSeconds = seconds;
}

