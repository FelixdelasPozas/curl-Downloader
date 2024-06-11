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

// Qt
#include <QPainter>
#include <QRect>
#include <QString>
#include <QDir>
#include <QMessageBox>

// C++
#include <unistd.h> // sleep

//----------------------------------------------------------------------------
ItemWidget::ItemWidget(const Utils::Configuration &config, Utils::ItemInformation *item, QWidget* parent, Qt::WindowFlags f)
: QWidget(parent, f)
, m_item{item}
, m_config{config}
, m_finished{false}
, m_aborted{false}
, m_progressVal{0}
, m_console{parent}
, m_process{this}
, m_addItem{nullptr}
{
  setupUi(this);
  m_console.hide();
  m_console.setWindowTitle(tr("%1 process console output.").arg(m_item->url.fileName()));
  m_status->setTextFormat(Qt::TextFormat::RichText);

  connectSignals();

  setToolTip(m_item->toText());

  m_filename->setText("<p style='white-space:pre'><b>" + m_item->url.fileName() + "</b>");
  updateWidget(0, "??", "??:??:??");
  setStatus(Status::STARTING);
  startProcess();
}

//----------------------------------------------------------------------------
ItemWidget::~ItemWidget()
{
  if(m_addItem)
  {
    if (m_addItem->isVisible())
      m_addItem->done(QDialog::Rejected);

    m_addItem->deleteLater();
    m_addItem = nullptr;
  }

  m_addItem = nullptr;
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
void ItemWidget::onErrorOcurred(QProcess::ProcessError error)
{
  QString errorMessage;
  switch(error)
  {
    case QProcess::ProcessError::Crashed:
      errorMessage = "Process crashed!";
      break;
    case QProcess::ProcessError::FailedToStart:
      errorMessage = "Process failed to start!";
      break;
    case QProcess::ProcessError::ReadError:
      errorMessage = "Process read error!";
      break;
    case QProcess::ProcessError::Timedout:
      errorMessage = "Process timed out!";
      break;
    case QProcess::ProcessError::WriteError:
      errorMessage = "Process write error!";
      break;
    default:
    case QProcess::ProcessError::UnknownError:
      errorMessage = "Process unknown error!";
      break;
  }

  setStatus(Status::ERROR);
  m_console.addText("\n" + errorMessage + "\n");
}

//----------------------------------------------------------------------------
void ItemWidget:: updateWidget(const unsigned int progressValue, const QString &speed, const QString &timeRemain)
{
  m_progressVal = progressValue;
  m_progress->setText(QString("%1%").arg(progressValue));
  m_speed->setText(speed.isEmpty() ? "??":speed);
  m_remain->setText(timeRemain.isEmpty() ? "--:--:--": timeRemain);

  update();
}

//----------------------------------------------------------------------------
void ItemWidget::onFinished(int code , QProcess::ExitStatus status)
{
  QString message = "Process finished with code: " + QString::number(code) + ". Status: ";
  switch(status)
  {
    case QProcess::ExitStatus::NormalExit:
      message += "normal exit.";
      break;
    case QProcess::ExitStatus::CrashExit:
      message += "crash exit.";
      break;
  }

  m_console.addText("\n" + message + "\n");
  m_console.addText(message);

  if(!m_finished && !m_aborted)
    m_finished = (code == 0);

  if(!m_finished && !m_aborted)
  {
    m_finished = (code == 0);
    setStatus(Status::RETRYING);
    m_console.addText(QString("\nRetrying in %1 seconds...\n").arg(m_config.waitSeconds));
    sleep(m_config.waitSeconds);
    startProcess();
  }
  else
  {
    if(m_aborted)
    {
      setStatus(Status::ABORTED);

      emit cancelled();
    }
    else
    {
      setStatus(Status::FINISHED);

      emit finished();
    }
  }
}

//----------------------------------------------------------------------------
void ItemWidget::onTextReady()
{
  const auto stderrText = QString(m_process.readAllStandardError());
  const auto stdoutText = QString(m_process.readAllStandardOutput());
  unsigned int percentage = 0;
  QString speed, remain;

  for(auto text: {stderrText, stdoutText})
  {
    if(text.isEmpty()) continue;
    auto parts = text.split(' ');
    parts.removeAll("");
    parts.removeAll(" ");
    bool isValid = false;
    if(parts.size() < 1) continue;
    percentage = parts.front().toUInt(&isValid);
    if(!isValid || parts.size() < 12) continue;
    remain = parts[10];
    speed = parts[11].replace("\n","");
    break;
  }

  updateWidget(percentage, speed, remain);
  setStatus(Status::DOWNLOADING);

  // Use with progress bar mode --------
  // auto pos = stderrText.indexOf('%');
  // if(pos != -1)
  // {
  //   const QStringRef percentageStr(&stderrText, pos-5,5);
  //   m_progressVal = percentageStr.toFloat();
  //   onProgressChanged(m_progressVal);
  //   setStatus(Status::DOWNLOADING);
  // }

  if(!stderrText.isEmpty())
    m_console.addText(stderrText + "\n");
  if(!stdoutText.isEmpty())
    m_console.addText(stdoutText + "\n");
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
    case Status::ERROR:
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
  //m_process.setProcessChannelMode(QProcess::SeparateChannels);
  QStringList arguments;
  arguments << "--disable"; // Disable .curlrc
  arguments << "--create-dirs"; // Create necessary local directory hierarchy
//  arguments << "--progress-bar"; // use progress bar
  arguments << "--connect-timeout" << "60"; // Maximum time allowed for connection
  arguments << "--insecure"; // Allow insecure server connections when using SSL
  arguments << "--location"; // Follow redirects
  arguments << "--retry" << "999"; // <num> Retry request if transient problems occur
  arguments << "--retry-connrefused"; // Retry on connection refused (use with --retry)
  arguments << "--retry-delay" << QString::number(m_config.waitSeconds); // <seconds> Wait time between retries  
  arguments << "--output" << m_item->url.fileName();
  if(!m_item->server.isEmpty())
  {
    if(m_item->protocol != Utils::Protocol::NONE)
    {
      arguments << "--proxy-insecure"; // Do HTTPS proxy connections without verifying the proxy

      const auto serverText = QString("%1:%2").arg(m_item->server).arg(m_item->port);
      arguments << protocols.at(static_cast<int>(m_item->protocol)) << serverText;
    }
  }
  arguments << "--url" << m_item->url.toString();

  // Continue.
  if(QDir(m_config.downloadPath).exists(m_item->url.fileName()))
    arguments << "-C" << "-";

  m_process.setArguments(arguments);
  m_process.start();
  m_process.setTextModeEnabled(true);  
  m_process.waitForStarted();
}

//----------------------------------------------------------------------------
void ItemWidget::stopProcess()
{
  if(!m_aborted && m_process.state() != QProcess::ProcessState::NotRunning)
  {
    const auto filename = m_item->url.fileName();
    {
      QMessageBox msgBox(this);
      msgBox.setWindowTitle(filename);
      msgBox.setStandardButtons(QMessageBox::Button::Yes|QMessageBox::Button::No);
      msgBox.setText(QString("Do you want to cancel the download of '%1'?").arg(filename));

      if(msgBox.exec() == QMessageBox::No)
        return;
    }

    m_aborted = true;

    m_cancel->setEnabled(false);

    m_process.terminate();
    m_process.kill();
    m_process.waitForFinished();
  }
}

//----------------------------------------------------------------------------
void ItemWidget::paintEvent(QPaintEvent *event)
{
  const int progressXPoint = size().width() * m_progressVal/100.f;
  auto wRect = rect();
  wRect.setWidth(std::min(progressXPoint, wRect.width()));

  // green progress background.
	QPainter painter(this);
  painter.setPen(Qt::transparent);
  painter.setBrush(QColor(120, 255, 120));
  painter.drawRect(wRect);
  painter.end();

  QWidget::paintEvent(event);
}

//----------------------------------------------------------------------------
void ItemWidget::mousePressEvent(QMouseEvent *)
{
  if(!m_addItem)
  {
    m_addItem = new AddItemDialog(this);
    m_addItem->setItem(m_item);
    m_addItem->setWindowTitle("Modify item server and port");
    m_addItem->m_url->setEnabled(false);
  }

  if(m_addItem->exec() == QDialog::Accepted)
  {
    const auto item = m_addItem->getItem();
    m_item->url = item->url;
    m_item->port = item->port;
    m_item->protocol = item->protocol;
    m_item->server = item->server;
    delete item;
  }
}
