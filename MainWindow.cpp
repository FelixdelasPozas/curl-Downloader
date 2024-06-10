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

// C++ 
#include <cassert>
#include <exception>

const QString CURL_LOCATION_KEY = "Curl executable location";
const QString DOWNLOAD_FOLDER_KEY = "Download folder";
const QString WAIT_TIME_KEY = "Wait time";

//----------------------------------------------------------------------------
MainWindow::MainWindow(QWidget *parent, Qt::WindowFlags flags)
: QMainWindow(parent, flags)
{
  setupUi(this);

  connectSignals();

  loadSettings();

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
    delete m_widgets[i];
  }

  m_widgets.clear();
  m_items.clear();

  saveSettings();
}

//----------------------------------------------------------------------------
void MainWindow::showAboutDialog()
{
  AboutDialog dialog(this);
  dialog.exec();
}

//----------------------------------------------------------------------------
void MainWindow::addItem()
{
  AddItemDialog dialog(this);
  const auto value = dialog.exec();

  if(value == QDialog::Rejected) return;

  auto item = dialog.getItem();

  if(Utils::findItem(item.url, m_items) != m_items.cend())
  {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Item information");
    msgBox.setStandardButtons(QMessageBox::Button::Ok);
    msgBox.setText(QString("The url '%1' is already being downloaded!").arg(item.url.fileName()));
    msgBox.exec();
    return;
  }
  
  m_items.push_back(item);
  
  auto itemWidget = new ItemWidget(m_config, m_items.back());
  m_widgets.push_back(itemWidget);

  connect(itemWidget, SIGNAL(cancelled()), this, SLOT(onProcessFinished()));
  connect(itemWidget, SIGNAL(finished()), this, SLOT(onProcessFinished()));

  m_scrollLayout->insertWidget(m_scrollLayout->count()-1, itemWidget);
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
    const auto item(widget->item());

    Utils::AutoCloseMessageBox msgBox(this);
    msgBox.setWindowTitle("Item information");
    msgBox.setStandardButtons(QMessageBox::Button::Ok);

    if(widget->hasFinished())
      msgBox.setText(QString("The url '%1' has finished downloading!").arg(item.url.fileName()));
    else
      msgBox.setText(QString("The url '%1' has been aborted!").arg(item.url.fileName()));

    msgBox.exec();

    m_scrollLayout->removeWidget(widget);

    auto itemIt = Utils::findItem(item.url, m_items);
    auto widgetIt = m_widgets.begin() + std::distance(m_items.cbegin(), itemIt);
    m_widgets.erase(widgetIt);
    m_items.erase(itemIt);
    delete widget;
  }
  else
  {
    throw std::runtime_error("Unable to identify removeItem sender!");
  }
}

//----------------------------------------------------------------------------
void MainWindow::connectSignals()
{
  connect(this->actionAbout, SIGNAL(triggered(bool)), this, SLOT(showAboutDialog()));
  connect(this->actionExit_application, SIGNAL(triggered(bool)), this, SLOT(close()));
  connect(this->actionAdd_file_to_download, SIGNAL(triggered(bool)), this, SLOT(addItem()));
  connect(this->actionApplication_settings, SIGNAL(triggered(bool)), this, SLOT(showConfigurationDialog()));
}

//----------------------------------------------------------------------------
void MainWindow::loadSettings()
{
  QSettings settings("Felix de las Pozas Alvarez", "curlDownloader");

  auto curlLocation = settings.value(CURL_LOCATION_KEY).toString();
  auto downloadFolder = settings.value(DOWNLOAD_FOLDER_KEY).toString();
  auto waitTime = settings.value(WAIT_TIME_KEY, 5).toUInt();

  Utils::Configuration config(curlLocation, downloadFolder, waitTime);

  if(!config.isValid())
    showConfigurationDialog();
  else
    m_config = config;
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
  settings.sync();
}
