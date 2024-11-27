/*
 File: MainWindow.cpp
 Created on: 03/06/2024
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
#include <MainWindow.h>
#include <AboutDialog.h>
#include <ConfigurationDialog.h>
#include <AddItemDialog.h>
#include <ItemWidget.h>

// Qt
#include <QMessageBox>
#include <QSettings>
#include <QMenu>
#include <QFile>
#include <QDir>
#include <QtWinExtras/QWinTaskbarProgress>

const QString CURL_LOCATION_KEY = "Curl executable location";
const QString DOWNLOAD_FOLDER_KEY = "Download folder";
const QString WAIT_TIME_KEY = "Wait time";
const QString TEMPORAL_EXTENSION = "Temporal extension";
const QString GEOMETRY = "Window geometry";
const QString STATE = "GUI State";

//----------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags)
, m_needsExit{false}
, m_trayIcon{new QSystemTrayIcon(QIcon(":/Downloader/download-bold.svg"), this)}
, m_taskbarButton{nullptr}
{
  setupUi(this);
  setMinimumWidth(600);
  connectSignals();

  loadSettings();

  setupTrayIcon();

  this->actionAdd_file_to_download->setEnabled(m_config.isValid());
}

//----------------------------------------------------------------------------
MainWindow::~MainWindow()
{
  if(!m_items.empty())
  {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("curl Downloader");
    msgBox.setStandardButtons(QMessageBox::Button::Yes|QMessageBox::Button::No);
    msgBox.setText(QString("There are still %1 items being downloaded. Do you really want to exit?").arg(m_items.size()));
    if(QDialog::Rejected == msgBox.exec())
      return;    
  }

  for(size_t i = 0; i < m_items.size(); ++i)
  {
    disconnect(m_widgets[i]);
    m_widgets[i]->stopProcess();
    m_scrollLayout->removeWidget(m_widgets[i]);
    m_widgets[i]->deleteLater();
  }

  m_widgets.clear();
  m_items.clear();

  saveSettings();

  if(m_taskbarButton)
    m_taskbarButton->deleteLater();
}

//----------------------------------------------------------------------------
void MainWindow::showAboutDialog()
{
  AboutDialog dialog(m_config, this);
  dialog.exec();
}

//----------------------------------------------------------------------------
void MainWindow::addItem()
{
  AddItemDialog dialog(this);
  const auto value = dialog.exec();

  if(value == QDialog::Rejected) return;

  auto item = dialog.getItem();

  if(Utils::findItem(item->url, m_items) != m_items.cend())
  {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Item information");
    msgBox.setStandardButtons(QMessageBox::Button::Ok);
    msgBox.setText(QString("The url '%1' is already being downloaded!").arg(item->url.fileName()));
    msgBox.exec();

    delete item;
    return;
  }
  
  m_items.push_back(item);
  
  auto itemWidget = new ItemWidget(m_config, m_items.back());
  m_widgets.push_back(itemWidget);

  connect(itemWidget, SIGNAL(cancelled()), this, SLOT(onProcessFinished()));
  connect(itemWidget, SIGNAL(finished()), this, SLOT(onProcessFinished()));
  connect(itemWidget, SIGNAL(progress()), this, SLOT(onWidgetProgress()));
  
  m_scrollLayout->insertWidget(m_scrollLayout->count()-1, itemWidget);
  onWidgetProgress();
}

//----------------------------------------------------------------------------
void MainWindow::showConfigurationDialog()
{
  ConfigurationDialog dialog(this);
  dialog.setConfiguration(m_config);

  if(dialog.exec() == QDialog::Accepted)
  {
    m_config = dialog.getConfiguration();
    this->actionAdd_file_to_download->setEnabled(true);
  }
}

//----------------------------------------------------------------------------
void MainWindow::onProcessFinished()
{
  const auto widget = qobject_cast<ItemWidget*>(sender());
  if(widget)
  {
    const auto hasFinished = widget->isFinished();

    const auto item = widget->item();
    const QString title("Item information");

    if(hasFinished)
    {
      if (!isVisible())
      {
        m_trayIcon->showMessage("File downloaded!", item->outputName);
      }
      else
      {
        Utils::AutoCloseMessageBox msgBox(this);
        msgBox.setWindowTitle(title);
        msgBox.setStandardButtons(QMessageBox::Button::Ok);
        msgBox.setText(QString("The file '%1' has finished downloading!").arg(item->outputName));
        msgBox.exec();
      }
    }

    auto itemIt = Utils::findItem(item->url, m_items);
    auto widgetIt = m_widgets.begin() + std::distance(m_items.cbegin(), itemIt);
    m_widgets.erase(widgetIt);
    m_items.erase(itemIt);
    
    m_scrollLayout->removeWidget(widget);
    widget->deleteLater();

    // rename and remove only if QProcess no longer exists and curl has finished.
    if(hasFinished)
    {
      if (!m_config.extension.isEmpty())
      {
        QDir downloadDir(m_config.downloadPath);
        if (!QFile::rename(downloadDir.absoluteFilePath(item->outputName + m_config.extension), downloadDir.absoluteFilePath(item->outputName)))
        {
          const auto message = QString("Unable to rename the file '%1' to '%2'!").arg(item->outputName + m_config.extension).arg(item->outputName);
          QMessageBox::critical(this, "Error!", message, QMessageBox::Button::Ok);
        }
      }
    }
    else
    {
      QMessageBox msgBox(this);
      msgBox.setWindowTitle(title);
      msgBox.setStandardButtons(QMessageBox::Button::Yes|QMessageBox::Button::No);
      msgBox.setText(QString("The file '%1' has been aborted!\nDo you want to remove the temporal file?").arg(item->outputName));

      if (QMessageBox::Yes == msgBox.exec())
      {
        QDir downloadDir(m_config.downloadPath);
        if(!QFile::exists(downloadDir.absoluteFilePath(item->outputName + m_config.extension)))
        {
          const auto message = QString("Unable to find the file '%1'!").arg(item->outputName + m_config.extension);
          QMessageBox::critical(this, "Error!", message, QMessageBox::Button::Ok);
        }
        else if (!QFile::remove(downloadDir.absoluteFilePath(item->outputName + m_config.extension)))
        {
          const auto message = QString("Unable to remove the file '%1'!").arg(item->outputName + m_config.extension);
          QMessageBox::critical(this, "Error!", message, QMessageBox::Button::Ok);
        }
      }
    }

    delete item;
  }
  else
  {
    QMessageBox::critical(this, "Crash!", "Unable to identify removeItem sender!", QMessageBox::Button::Ok);
    throw std::runtime_error("Unable to identify removeItem sender!");
  }

  // update global progress.
  onWidgetProgress();
}

//----------------------------------------------------------------------------
void MainWindow::connectSignals()
{
  connect(this->actionExit_application,     SIGNAL(triggered(bool)), this, SLOT(quitApplication()));
  connect(this->actionAbout,                SIGNAL(triggered(bool)), this, SLOT(showAboutDialog()));
  connect(this->actionExit_application,     SIGNAL(triggered(bool)), this, SLOT(close()));
  connect(this->actionAdd_file_to_download, SIGNAL(triggered(bool)), this, SLOT(addItem()));
  connect(this->actionApplication_settings, SIGNAL(triggered(bool)), this, SLOT(showConfigurationDialog()));

  connect(m_trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
          this,       SLOT(onTrayActivated(QSystemTrayIcon::ActivationReason)));  
}

//----------------------------------------------------------------------------
void MainWindow::quitApplication()
{
  m_needsExit = true;
  close();
}

//----------------------------------------------------------------------------
void MainWindow::closeEvent(QCloseEvent *e)
{
  if(!m_needsExit)
  {
    hide();
    m_trayIcon->show();

    e->accept();
  }
  else
  {
    QMainWindow::closeEvent(e);
    QApplication::exit(0);
  }
}

//-----------------------------------------------------------------
void MainWindow::showEvent(QShowEvent* e)
{
  QMainWindow::showEvent(e);

  if(!m_taskbarButton)
  {
    m_taskbarButton = new QWinTaskbarButton(this);
    m_taskbarButton->setWindow(this->windowHandle());
    m_taskbarButton->progress()->setRange(0, 100);
    m_taskbarButton->progress()->setVisible(true);
    m_taskbarButton->progress()->setValue(0);
  }
}
//----------------------------------------------------------------------------
void MainWindow::loadSettings()
{
  QSettings settings("Felix de las Pozas Alvarez", "curlDownloader");

  auto curlLocation = settings.value(CURL_LOCATION_KEY).toString();
  auto downloadFolder = settings.value(DOWNLOAD_FOLDER_KEY).toString();
  auto waitTime = settings.value(WAIT_TIME_KEY, 5).toUInt();
  auto extension = settings.value(TEMPORAL_EXTENSION).toString();

  Utils::Configuration config(curlLocation, downloadFolder, waitTime, extension);

  if(!config.isValid())
    showConfigurationDialog();
  else
    m_config = config;

  if(settings.contains(GEOMETRY))
  {
    auto geometry = settings.value(GEOMETRY).toByteArray();
    restoreGeometry(geometry);
  }

  if(settings.contains(STATE))
  {
    auto state = settings.value(STATE).toByteArray();
    restoreState(state);
  }
}

//----------------------------------------------------------------------------
void MainWindow::saveSettings()
{
  if(!m_config.isValid())
    return;

  QSettings settings("Felix de las Pozas Alvarez", "curlDownloader");

  settings.setValue(CURL_LOCATION_KEY, m_config.curlPath);
  settings.setValue(DOWNLOAD_FOLDER_KEY, m_config.downloadPath);
  settings.setValue(WAIT_TIME_KEY, m_config.waitSeconds);
  settings.setValue(TEMPORAL_EXTENSION, m_config.extension);
  settings.setValue(GEOMETRY, saveGeometry());
  settings.setValue(STATE, saveState());
  settings.sync();
}

//----------------------------------------------------------------------------
void MainWindow::setupTrayIcon()
{
  auto menu = new QMenu(tr("Menu"));

  auto showAction = new QAction(QIcon(":/Downloader/file-download.svg"), tr("Restore..."));
  connect(showAction, SIGNAL(triggered(bool)), this, SLOT(onTrayActivated()));

  auto addFile = new QAction(QIcon(":/Downloader/add.svg"), tr("Add item..."));
  connect(addFile, SIGNAL(triggered(bool)), this, SLOT(addItem()));

  auto aboutAction = new QAction(QIcon(":/Downloader/info.svg"), tr("About..."));
  connect(aboutAction, SIGNAL(triggered(bool)), this, SLOT(showAboutDialog()));

  auto quitAction = new QAction(QIcon(":/Downloader/exit.svg"), tr("Quit"));
  connect(quitAction, SIGNAL(triggered(bool)), this, SLOT(quitApplication()));

  menu->addAction(showAction);
  menu->addSeparator();
  menu->addAction(addFile);
  menu->addSeparator();
  menu->addAction(aboutAction);
  menu->addSeparator();
  menu->addAction(quitAction);

  m_trayIcon->setContextMenu(menu);
  m_trayIcon->setToolTip(tr("No downloads."));
  m_trayIcon->hide();
}

//----------------------------------------------------------------------------
void MainWindow::onTrayActivated(QSystemTrayIcon::ActivationReason reason)
{
  if(m_trayIcon->isVisible() && reason == QSystemTrayIcon::DoubleClick)
  {
    showNormal();
    m_trayIcon->hide();
  }
}

//----------------------------------------------------------------------------
void MainWindow::onWidgetProgress()
{
  int progressValue = 0;

  if(!m_items.empty())
  {
    std::for_each(m_widgets.cbegin(), m_widgets.cend(), [&progressValue](const ItemWidget *w){ progressValue += w->progress(); });
    progressValue /= m_items.size();
    progressValue = std::min(100, std::max(0, progressValue));
    m_trayIcon->setToolTip(QString("Downloading %1 file%2.\nProgress: %3%").arg(m_items.size()).arg(m_items.size() > 1 ? "s":"").arg(progressValue));
  }
  else
    m_trayIcon->setToolTip(tr("No downloads."));

  if(m_taskbarButton)
    m_taskbarButton->progress()->setValue(static_cast<int>(progressValue));
}
