/*
 File: Utils.h
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

#ifndef _UTILS_H_
#define _UTILS_H_

// Qt
#include <QUrl>
#include <QString>

namespace Utils
{
  /**
   * @brief Socks protocol enum.
   */
  enum class Protocol : char
  {
    SOCKS4 = 0,
    SOCKS5 = 1
  };

  /**
   * @brief Item information struct.
   */
  struct ItemInformation
  {
    QUrl url;          /** url of file to download. */
    QString server;    /** server address ip. */
    unsigned int port; /** server address port. */
    Protocol protocol; /** protocol version used. */

    /**
     * @brief ItemInformation constructor.
     * @param fileUrl url of file to download.
     * @param serverIp server address ip.
     * @param serverPort server address port.
     * @param fProtocol protocol used.
     */
    ItemInformation(const QUrl fileUrl, const QString &serverIp, const unsigned int serverPort, const Protocol fProtocol)
    : url{fileUrl}, server{serverIp}, port{serverPort}, protocol{fProtocol} {};

    /**
     * @brief Returns true if the information is valid and false otherwise.
     */
    bool isValid() const;
  };

  /**
   * @brief Configuration information struct.
   */
  struct Configuration
  {
    QString curlPath;         /** path to curl executable. */
    QString downloadPath;     /** path to download folder. */
    unsigned int waitSeconds; /** seconds to wait between retries. */

    /**
     * @brief Configuration struct constructor.
     * @param wPath Path to curl executable.
     * @param dPath Path to download folder.
     * @param waitTime Time to wait between retries.
     */
    Configuration(const QString &wPath, const QString &dPath, const unsigned int waitTime)
    : curlPath{wPath}, downloadPath{dPath}, waitSeconds{waitTime} {};

    /**
     * @brief Returns true if the information is valid and false otherwise.
     */
    bool isValid() const
    {
      return !curlPath.isEmpty() && !downloadPath.isEmpty() && waitSeconds >= 5;
    }
  };

  /**
   * @brief Returns the version of the curl executable or empty if failed.
   * @param exePath Path of the executable.
   */
  QString curlExecutableVersion(const QString &exePath);
}

#endif
