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

// Qt
#include <QHostAddress>
#include <QProcess>

//----------------------------------------------------------------------------
bool Utils::ItemInformation::isValid() const
{
  QHostAddress address(server);
  // url.isValid() is a joke... everything goes.
  return !server.isEmpty() && url.isValid() && (QAbstractSocket::IPv4Protocol == address.protocol()) && port > 0;
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