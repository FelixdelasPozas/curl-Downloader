/*
 File: ConfigurationDialog.cpp
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
#include <ConfigurationDialog.h>

// Qt
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>

//----------------------------------------------------------------------------
ConfigurationDialog::ConfigurationDialog(QWidget *parent, Qt::WindowFlags f)
: QDialog(parent, f)
{
  setupUi(this);

  connectSignals();
}

//----------------------------------------------------------------------------
Utils::Configuration ConfigurationDialog::getConfiguration() const
{
  return Utils::Configuration(m_curlLocation->text(), m_DownloadFolder->text(), m_waitSpinbox->value());
}

//----------------------------------------------------------------------------
void ConfigurationDialog::setConfiguration(const Utils::Configuration &config)
{
  if(config.isValid())
  {
    m_curlLocation->setText(config.curlPath);
    m_DownloadFolder->setText(config.downloadPath);
    m_waitSpinbox->setValue(config.waitSeconds);
  }
}

//----------------------------------------------------------------------------
void ConfigurationDialog::onCurlFolderClicked()
{
  const auto curlPath = QFileDialog::getOpenFileName(this, 
                                                     "Select curl executable", 
                                                     QDir::currentPath(),
                                                     "Executable file (*.exe)",
                                                     nullptr,
                                                     QFileDialog::Options::enum_type::ReadOnly|QFileDialog::Options::enum_type::DontUseNativeDialog);

  if(curlPath.isEmpty()) return;

  const auto version = Utils::curlExecutableVersion(curlPath);

  QMessageBox msgBox(this);
  msgBox.setWindowTitle("Curl executable");
  msgBox.setStandardButtons(QMessageBox::Button::Ok);
  msgBox.setText(version.isEmpty() ? QString("Unable to verify curl version.")
                                   : QString("Detected curl version %1").arg(version));
  msgBox.exec();

  if(version.isEmpty()) return;

  m_curlLocation->setText(curlPath);
}

//----------------------------------------------------------------------------
void ConfigurationDialog::onDownloadFolderClicked()
{
  const auto folderPath = QFileDialog::getExistingDirectory(this, 
                                                            "Select download folder", 
                                                            QDir::currentPath(),
                                                            QFileDialog::Options::enum_type::ShowDirsOnly|QFileDialog::Options::enum_type::DontUseNativeDialog);

  if(folderPath.isEmpty()) return;

  QDir testDir{folderPath};
  if(!testDir.isReadable())
  {
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Downloads folder");
    msgBox.setStandardButtons(QMessageBox::Button::Ok);
    msgBox.setText(QString("Unable to read the folder: %1").arg(folderPath));
    msgBox.exec();

    return;
  }

  m_DownloadFolder->setText(folderPath);
}

//----------------------------------------------------------------------------
void ConfigurationDialog::closeEvent(QCloseEvent *e)
{
  const Utils::Configuration config(m_curlLocation->text(), m_DownloadFolder->text(), m_waitSpinbox->value());

  if(!config.isValid())
  {
    e->setAccepted(false);
    e->ignore();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Configuration");
    msgBox.setStandardButtons(QMessageBox::Button::Ok);
    msgBox.setText("The configuration data is not valid.");
    msgBox.exec();
    return;
  }

  accept();
}

//----------------------------------------------------------------------------
void ConfigurationDialog::connectSignals()
{
  connect(this->m_curlButton, SIGNAL(clicked()), this, SLOT(onCurlFolderClicked()));
  connect(this->m_downloadsButton, SIGNAL(clicked()), this, SLOT(onDownloadFolderClicked()));
}

