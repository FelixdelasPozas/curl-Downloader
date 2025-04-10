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
#include <QLabel>
#include <QPainter>

class ItemWidget;
class QToolButton;
class QSettings;

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
    QUrl url;           /** url of file to download. */
    QString server;     /** server address ip. */
    unsigned int port;  /** server address port. */
    Protocol protocol;  /** protocol version used. */
    QString outputName; /** output file name. */

    /**
     * @brief ItemInformation constructor.
     * @param fileUrl url of file to download.
     * @param serverIp server address ip.
     * @param serverPort server address port.
     * @param fProtocol protocol used.
     */
    ItemInformation(const QUrl fileUrl, const QString &serverIp, const unsigned int serverPort, const Protocol fProtocol, const QString &fName)
    : url{fileUrl}, server{serverIp}, port{serverPort}, protocol{fProtocol}, outputName{fName} {};

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

    /**
     * @brief  Operator Equal
     * @param other Other ItemInformation struct
     * @return True if equal and false otherwise. 
     */
    bool operator==(const ItemInformation &other);

    /**
     * @brief  Operator Disequal
     * @param other Other ItemInformation struct
     * @return True if not equal and false if equal. 
     */
    bool operator!=(const ItemInformation &other)
    { return !this->operator==(other); }
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

  /**
   * @brief ElidedLabel class.
   */
  class ElidedLabel
  : public QLabel
  {
      Q_OBJECT
    public:
      /**
       * @brief ElidedLabel class constructor.
       * @param parent Parent widget raw pointer. 
       * @param f Window flags.
       */
      ElidedLabel(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
      : QLabel(parent, f)
      {}

      /**
       * @brief ElidedLabel class constructor.
       * @param text Text to show.
       * @param parent Parent widget raw pointer. 
       * @param f Window flags.
       */
      ElidedLabel(const QString &text, QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags())
      : QLabel(text, parent, f)
      {}

      /**
       * @brief ElidedLabel class virtual destructor.
       */
      virtual ~ElidedLabel()
      {}

    protected:
      virtual QSize sizeHint() const override
      {
        return QSize{width(), height()};
      }

      virtual QSize minimumSizeHint() const override
      {
        return QSize{width(), height()};
      }

      virtual void paintEvent(QPaintEvent *event) override
      {
        QPainter painter(this);
        painter.drawText(rect(), Qt::AlignLeft | Qt::AlignVCenter, fontMetrics().elidedText(text(), Qt::ElideRight, width()));
      }
  };

  /** \class ClickableHoverLabel
  * \brief ClickableLabel subclass that changes the mouse cursor when hovered.
  *
  */
  class ClickableHoverLabel
  : public QLabel
  {
      Q_OBJECT
    public:
      /** \brief ClickableHoverLabel class constructor.
      * \param[in] parent Raw pointer of the widget parent of this one.
      * \f Widget flags.
      *
      */
      explicit ClickableHoverLabel(QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags())
      : QLabel(parent, f)
      {};

      /** \brief ClickableHoverLabel class constructor.
      * \param[in] text Label text.
      * \param[in] parent Raw pointer of the widget parent of this one.
      * \f Widget flags.
      *
      */
      explicit ClickableHoverLabel(const QString &text, QWidget *parent=nullptr, Qt::WindowFlags f=Qt::WindowFlags())
      : QLabel(text, parent, f)
      {};
      
      /** \brief ClickableHoverLabel class virtual destructor.
      *
      */
      virtual ~ClickableHoverLabel()
      {};

    signals:
      void clicked();

    protected:
      void mousePressEvent(QMouseEvent* e)
      {
        emit clicked();
        QLabel::mousePressEvent(e);
      }  

      virtual void enterEvent(QEnterEvent *event) override
      {
        setCursor(Qt::PointingHandCursor);
        QLabel::enterEvent(event);
      }

      virtual void leaveEvent(QEvent *event) override
      {
        setCursor(Qt::ArrowCursor);
        QLabel::leaveEvent(event);
      }
  };

  /** \brief Returns the application settings depending on the existance of the INI file.
   *
   */
  std::unique_ptr<QSettings> applicationSettings();
}

#endif
