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
#include <AddItemDialog.h>
#include <curlErrors.h>

// Qt
#include <QPainter>
#include <QRect>
#include <QString>
#include <QDir>
#include <QMessageBox>
#include <QFile>
#include <QFontDatabase>

int ItemWidget::FONT_ID = -1;

//----------------------------------------------------------------------------
ItemWidget::ItemWidget(const Utils::Configuration &config, Utils::ItemInformation *item, QWidget* parent, Qt::WindowFlags f)
: QWidget(parent, f)
, m_item{item}
, m_config{config}
, m_finished{false}
, m_aborted{false}
, m_paused{false}
, m_supportsResume{ResumeType::UNKNOWN}
, m_resumed{0}
, m_progressVal{0}
, m_console{parent}
, m_process{this}
{
  setupUi(this);
  if(loadFont())
    applyFont();

  m_console.hide();
  m_console.setWindowTitle(tr("'%1' process console output.").arg(m_item->outputName));
  m_status->setTextFormat(Qt::TextFormat::RichText);

  connectSignals();

  m_filename->setText(m_item->outputName);
  updateWidget(0, "??", "--:--:--");
  setStatus(Status::STARTING);
  updateTooltip();
  
  startProcess();
}

//----------------------------------------------------------------------------
void ItemWidget::onNotesButtonPressed()
{
  if(!m_console.isVisible())
    m_console.show();
  else 
    m_console.raise();
}

//----------------------------------------------------------------------------
void ItemWidget::onPlayButtonPressed()
{
  m_timer.stop();

  if(m_paused)
  {
    m_paused = false;
    startProcess();
    setStatus(Status::STARTING);
    m_playPause->setIcon(QIcon(":/Downloader/pause.svg"));
  }
  else
  {
    m_paused = true;
    stopProcessImplementation();
    setStatus(Status::PAUSED);
    m_playPause->setIcon(QIcon(":/Downloader/play.svg"));
  }
}

//----------------------------------------------------------------------------
void ItemWidget::onErrorOcurred(QProcess::ProcessError error)
{
  QString errorMessage;
  switch(error)
  {
    case QProcess::ProcessError::Crashed:
      errorMessage = "crashed!";
      break;
    case QProcess::ProcessError::FailedToStart:
      errorMessage = "failed to start!";
      break;
    case QProcess::ProcessError::ReadError:
      errorMessage = "read error!";
      break;
    case QProcess::ProcessError::Timedout:
      errorMessage = "timed out!";
      break;
    case QProcess::ProcessError::WriteError:
      errorMessage = "write error!";
      break;
    default:
    case QProcess::ProcessError::UnknownError:
      errorMessage = "unknown error!";
      break;
  }

  setStatus(Status::ERROR_);
  m_console.addText("Process " + errorMessage + "\n");
}

//----------------------------------------------------------------------------
void ItemWidget:: updateWidget(const unsigned int progressValue, const QString &speed, const QString &timeRemain)
{
  if(m_progressVal != progressValue)
  {
    if(progressValue < m_progressVal)
    {
      ++m_resumed;
      updateTooltip();
    }

    m_progressVal = progressValue;
    emit progress();
  }
  
  m_progress->setText(QString("%1%").arg(m_progressVal));
  m_speed->setText(speed.isEmpty() ? "??":speed);
  m_remain->setText(timeRemain.isEmpty() ? "--:--:--": timeRemain);

  update();
}

//----------------------------------------------------------------------------
void ItemWidget::onFinished(int code , QProcess::ExitStatus status)
{
  QString message = "Process finished with code: " + QString::number(code) + " (" + curlErrorCodeToText(code) + ").\nStatus: ";
  switch(status)
  {
    case QProcess::ExitStatus::NormalExit:
      message += "normal exit.";
      break;
    case QProcess::ExitStatus::CrashExit:
      message += "crash exit.";
      break;
  }

  m_console.addText(message + "\n");

  if(m_paused)
    return;

  if(!m_finished && !m_aborted)
    m_finished = (code == 0);

  if(!m_finished && !m_aborted)
  {
    m_finished = (code == 0);
    setStatus(Status::RETRYING);
    m_console.addText(QString("Retrying in %1 seconds...\n").arg(m_config.waitSeconds));
    m_timer.singleShot(m_config.waitSeconds*1000, this, SLOT(startProcess()));
  }
  else
  {
    if(m_aborted)
    {
      setStatus(Status::ABORTED);
      m_timer.stop();

      emit cancelled();
    }
    else
    {
      setStatus(Status::FINISHED);
      m_timer.stop();

      emit finished();
    }
  }
}

//----------------------------------------------------------------------------
void ItemWidget::onTextReady()
{
  const auto stderrText = QString(m_process.readAllStandardError());
  const auto stdoutText = QString(m_process.readAllStandardOutput());

  for(auto text: {stderrText, stdoutText})
  {
    if(text.isEmpty()) continue;
    auto parts = text.split(' ');
    parts.removeAll("");
    parts.removeAll(" ");
    if(parts.size() < 1) continue;
    bool isValid = false;
    const auto percentage = parts.front().toUInt(&isValid);
    if(!isValid || percentage > 100 || parts.size() != 12) continue;

    // 0 is progress, 1 is total size.
    const auto remainSize = (parts[1].isEmpty() || parts[1].compare("0") == 0) ? QString() : parts[1];
    if(m_remainSize.isEmpty() && !remainSize.isEmpty())
    {
      m_remainSize = remainSize;
    }

    if(m_supportsResume == ResumeType::UNKNOWN && (m_resumed > 0))
    {
      if(!m_remainSize.isEmpty() && !remainSize.isEmpty())
      { 
        if(m_remainSize.compare(remainSize, Qt::CaseInsensitive) == 0)
        {
          m_supportsResume = ResumeType::NO;
        }
        else
        {
          m_supportsResume = ResumeType::YES;
        }
      
        updateTooltip();
      }
    }

    updateWidget(percentage, parts[11].remove('\n').remove('\r'), parts[10]);  
    setStatus(Status::DOWNLOADING);
    m_console.addText(text + "\n");
    break;
  }
}

//----------------------------------------------------------------------------
void ItemWidget::connectSignals()
{
  connect(&m_process, SIGNAL(errorOccurred(QProcess::ProcessError)), this, SLOT(onErrorOcurred(QProcess::ProcessError)), Qt::DirectConnection);
  connect(&m_process, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onFinished(int, QProcess::ExitStatus)), Qt::DirectConnection);

  connect(&m_process, SIGNAL(readyReadStandardError()), this, SLOT(onTextReady()), Qt::DirectConnection);
  connect(&m_process, SIGNAL(readyReadStandardOutput()), this, SLOT(onTextReady()), Qt::DirectConnection);
  
  connect(m_cancel, SIGNAL(pressed()), this, SLOT(stopProcess()));
  connect(m_notes, SIGNAL(pressed()), this, SLOT(onNotesButtonPressed()));
  connect(m_playPause, SIGNAL(pressed()), this, SLOT(onPlayButtonPressed()));
}

//----------------------------------------------------------------------------
void ItemWidget::setStatus(ItemWidget::Status status)
{
  QString statusText; 

  switch(status)
  {
    default:
    case Status::DOWNLOADING:
      statusText = QString("<b>Downloading</b>");
      break;
    case Status::ERROR_:
      statusText = QString("<b><span style=\"color:#aa0000;\">Error</span></b>");
      break;
    case Status::RETRYING:
      statusText = QString("<b><span style=\"color:#0000aa;\">Retrying</span></b>");
      break;
    case Status::STARTING:
      statusText = QString("<b>Starting</b>");
      break;
    case Status::FINISHED:
      statusText = QString("<b>Finished</b>");
      break;
    case Status::PAUSED:
      statusText = QString("<b>Paused</b>");
      break;
    case Status::ABORTED:
      statusText = QString("<b><span style=\"color:#aa00aa;\">Aborted</span></b>");
      break;
  }

  m_status->setText(statusText);
}

//----------------------------------------------------------------------------
void ItemWidget::startProcess()
{
  const QStringList protocols = {"--socks4", "--socks5"};

  if(m_process.state() != QProcess::ProcessState::NotRunning)
    stopProcess();

  m_process.setWorkingDirectory(m_config.downloadPath);
  m_process.setProgram(m_config.curlPath);
  
  QStringList arguments;
  arguments << "--disable"; // Disable .curlrc
  arguments << "--create-dirs"; // Create necessary local directory hierarchy
  arguments << "--connect-timeout" << "60"; // Maximum time allowed for connection
  arguments << "--insecure"; // Allow insecure server connections when using SSL
  arguments << "--location"; // Follow redirects
  arguments << "--show-error"; // Show error even when -s is used
  arguments << "--retry" << "999"; // <num> Retry request if transient problems occur
  arguments << "--retry-connrefused"; // Retry on connection refused (use with --retry)
  arguments << "--retry-all-errors"; // Retry all errors.
  arguments << "--retry-delay" << QString::number(m_config.waitSeconds); // <seconds> Wait time between retries
  arguments << "--globoff"; // Switch off the URL globbing function, parses urls with {}[] chars.  
  arguments << "--output" << m_item->outputName + m_config.extension; // with temporal extension, if any.
  if(!m_item->server.isEmpty() && (m_item->protocol != Utils::Protocol::NONE))
  {
    arguments << "--proxy-insecure"; // Do HTTPS proxy connections without verifying the proxy

    const auto serverText = QString("%1:%2").arg(m_item->server).arg(m_item->port);
    arguments << protocols.at(static_cast<int>(m_item->protocol)) << serverText;
  }

  // Continue if possible
  if(QDir(m_config.downloadPath).exists(m_item->outputName + m_config.extension))
    arguments << "--continue-at" << "-";

  arguments << "--url" << m_item->url.toString();

  m_paused = false;
  m_process.setArguments(arguments);
  m_process.start();
  m_process.setTextModeEnabled(true);  
  m_process.waitForStarted();
}

//----------------------------------------------------------------------------
void ItemWidget::stopProcess()
{
  int remainingTime = 0;
  const bool timerIsActive = m_timer.isActive();

  auto reactivateTimer = [&]()
  {
    if(!timerIsActive) return;
    m_timer.setInterval(remainingTime);
    m_timer.start();
  };

  if(timerIsActive)
  {
    remainingTime = m_timer.remainingTime();
    m_timer.stop();
  }

  if(!m_aborted)
  {
    const auto filename = m_item->outputName;

    QMessageBox msgBox(this);
    msgBox.setWindowTitle(filename);
    msgBox.setStandardButtons(QMessageBox::Button::Yes | QMessageBox::Button::No);
    msgBox.setText(QString("Do you want to cancel the download of '%1'?").arg(filename));

    if (msgBox.exec() == QMessageBox::No)
    {
      reactivateTimer();
      return;
    }

    m_cancel->setEnabled(false);
    m_playPause->setEnabled(false);

    m_aborted = true;
    if(m_paused)
    {
      m_paused = false;
      onFinished(0, QProcess::ExitStatus::NormalExit);
    }
    else
    {
      stopProcessImplementation();
    }
  }
  else
    reactivateTimer();
}

//----------------------------------------------------------------------------
void ItemWidget::paintEvent(QPaintEvent *event)
{
  const int progressXPoint = size().width() * m_progressVal/100.f;
  auto wRect = rect();

	QPainter painter(this);
  painter.setPen(Qt::transparent);

  if(m_resumed > 0 && m_supportsResume == ResumeType::NO)
  {
    // red background to notify user.
    painter.setBrush(QColor(255,200,200));
    painter.drawRect(wRect);
  }

  wRect.setWidth(std::min(progressXPoint, wRect.width()));

  // green progress background.
  painter.setBrush(QColor(120, 255, 120));
  painter.drawRect(wRect);

  painter.end();

  QWidget::paintEvent(event);
}

//----------------------------------------------------------------------------
void ItemWidget::mousePressEvent(QMouseEvent *)
{
  AddItemDialog dialog(this);
  dialog.setWindowTitle("Modify item");
  dialog.m_url->setReadOnly(true);
  dialog.setItem(m_item);

  if(dialog.exec() == QDialog::Accepted)
  {
    const auto item = dialog.getItem();
    if(m_item->operator!=(*item))
    {
      m_item->port = item->port;
      m_item->protocol = item->protocol;
      m_item->server = item->server;
      const auto previousName = m_item->outputName;
      m_item->outputName = item->outputName;

      // force process restart and stop timer in case is already retrying
      m_timer.stop(); 
      m_process.terminate();
      m_process.kill();
      m_process.waitForFinished();

      if(previousName.compare(m_item->outputName, Qt::CaseSensitive) == 0)
      {
        auto itemDir = QDir(m_config.downloadPath);
        if(itemDir.exists(previousName) && !itemDir.rename(previousName + m_config.extension, m_item->outputName + m_config.extension))
        {
          QMessageBox::critical(this, previousName, QString("Unable to rename file '%1' to '%2'.").arg(previousName).arg(m_item->outputName));
          m_item->outputName = previousName;
        }
        else
        {
          m_filename->setText(m_item->outputName);
          m_console.setWindowTitle(tr("%1 process console output.").arg(m_item->outputName));
        }
      }
    }

    delete item;
  }
}

//----------------------------------------------------------------------------
void ItemWidget::enterEvent(QEnterEvent* event)
{
  QWidget::enterEvent(event);
  setCursor(Qt::ArrowCursor);
  QApplication::processEvents();
}

//----------------------------------------------------------------------------
bool ItemWidget::loadFont()
{
  if(FONT_ID == -1)
  {
    QFile file(":/Downloader/Ubuntu-R.ttf");
    if(file.exists() && file.open(QIODevice::ReadOnly))
      FONT_ID = QFontDatabase::addApplicationFontFromData(file.readAll());
  }

  return FONT_ID != -1;
}

//----------------------------------------------------------------------------
void ItemWidget::applyFont()
{
  if(FONT_ID != -1)
  {
    const QString family = QFontDatabase::applicationFontFamilies(FONT_ID).at(0);   
    QFont font(family);

    QList<QLabel*> widgets = {m_filename, m_remain, m_status, m_speed, m_progress};

    for(auto w: widgets)
    {
      auto wFont = w->font();
      font.setPointSize(wFont.pointSize());
      font.setBold(wFont.bold());
      w->setFont(font);
    }

    font.setBold(true);
    font.setPointSize(10);
    m_console.setFont(font);
  }  
}

//----------------------------------------------------------------------------
void ItemWidget::stopProcessImplementation()
{
  if(m_process.state() != QProcess::ProcessState::NotRunning)
  {
    m_process.terminate();
    m_process.kill();
    m_process.waitForFinished();
  }
}

//----------------------------------------------------------------------------
void ItemWidget::updateTooltip()
{
  auto toText = [](const ResumeType &value){ return value == ResumeType::UNKNOWN ? "Unknown" : (value == ResumeType::NO ? "No":"Yes"); };

  const QString tooltipText = m_item->toText() + "\nTimes resumed: " + QString::number(m_resumed) + "\nServer can resume: " + toText(m_supportsResume);
  setToolTip(tooltipText);
}
