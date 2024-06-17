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

// c++
#include <vector>

// Qt
#include <QUrl>
#include <QString>
#include <QMessageBox>

class ItemWidget;
class QToolButton;

namespace Utils
{
  /**
   * @brief Socks protocol enum.
   */
  enum class Protocol : char
  {
    SOCKS4 = 0,
    SOCKS5 = 1,
    NONE = 2
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
     * @brief ItemInformation empty constructor. 
     */
    ItemInformation(){};

    /**
     * @brief Returns true if the information is valid and false otherwise.
     */
    bool isValid() const;

    /**
     * @brief Returns the information as a text string.
     */
    QString toText() const;
  };

  /**
   * @brief Finds and returns the item iterator that has the same url as the given one. 
   * @param url Url to find.
   * @param items List of items.
   * @return Item const_iterator or cend().
   */
  std::vector<ItemInformation*>::const_iterator findItem(const QUrl &url, const std::vector<ItemInformation *> &items);

  /**
   * @brief Configuration information struct.
   */
  struct Configuration
  {
    QString curlPath;         /** path to curl executable. */
    QString downloadPath;     /** path to download folder. */
    unsigned int waitSeconds; /** seconds to wait between retries. */
    QString extension;        /** extensio to use when downloading. */

    /**
     * @brief Configuration struct constructor.
     * @param wPath Path to curl executable.
     * @param dPath Path to download folder.
     * @param waitTime Time to wait between retries.
     */
    Configuration(const QString &wPath, const QString &dPath, const unsigned int waitTime, const QString &tempExtension)
    : curlPath{wPath}, downloadPath{dPath}, waitSeconds{waitTime}, extension{tempExtension} {};

    /**
     * @brief Configuration struct empty constructor.
     */
    Configuration(){};

    /**
     * @brief Returns true if the information is valid and false otherwise.
     */
    bool isValid() const;
  };

  /**
   * @brief Returns the version of the curl executable or empty if failed.
   * @param exePath Path of the executable.
   */
  QString curlExecutableVersion(const QString &exePath);

  /**
   * @brief Returns the widget that contains the given button.
   * @param button Button pointer to find. 
   * @param widgets List of widgets. 
   */
  ItemWidget* findWidgetWithButton(const QToolButton *button, std::vector<ItemWidget *> widgets);

  /** 
   * @brief Implementation of an autoclose QMessageBox.
   */
  class AutoCloseMessageBox
  : public QMessageBox
  {
      Q_OBJECT
    public:
      /**
       * @AutoCloseMessageBox class constructor. 
       * @param parent Raw pointer of the widget parent of this one.
       */
      AutoCloseMessageBox(QWidget *parent = nullptr)
      : QMessageBox(parent)
      {};

      /**
       * @brief AutoCloseMessageBox class constructor. 
       * @param icon Message box icon.
       * @param title Dialog title.
       * @param text Dialog text.
       * @param buttons Buttons to show. 
       * @param parent Raw pointer of the widget parent of this one. 
       * @param flags Dialog flags. 
       */
      AutoCloseMessageBox(Icon icon, const QString &title, const QString &text,
                          StandardButtons buttons = NoButton, QWidget *parent = Q_NULLPTR,
                          Qt::WindowFlags flags = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint)
      : QMessageBox(icon, title, text, buttons, parent, flags)
      {};

      virtual ~AutoCloseMessageBox()
      {};

      /**
       * @brief Sets if the dialog must auto-close.
       * @param value True to auto-close and false otherwise. 
       */
      void setAutoClose(const bool value);

      /**
       * @bried Sets the closing time.
       * @param seconds Closing time in seconds. 
       */
      void setCloseTime(const unsigned int seconds);

    protected:
      void showEvent ( QShowEvent * event ) override;
      void timerEvent( QTimerEvent *event ) override;

    private:
      QString m_text;                  /** text to show. */
      unsigned int m_closeSeconds = 5; /** seconds to close. */
      bool m_autoClose = true;         /** true to auto-close false to act as a regular QMessageBox. */
      unsigned int m_currentTime = 0;  /** current time since showing the messagebox. */
      int m_timerId = 0;               /** current timer id. */
  };
}

#endif
