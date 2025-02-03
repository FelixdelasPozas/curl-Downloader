/*
 File: AboutDialog.cpp
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
#include <AboutDialog.h>
#include <Utils.h>

// Qt
#include <QDesktopServices>
#include <QtGlobal>
#include <QDateTime>

const QString AboutDialog::VERSION = QString("version 1.4.0");
const QString COPYRIGHT{"Copyright (c) 2024-%1 Félix de las Pozas Álvarez"};

//-----------------------------------------------------------------
AboutDialog::AboutDialog(const Utils::Configuration &config, QWidget *parent, Qt::WindowFlags flags)
: QDialog(parent, flags)
{
  setupUi(this);

  setWindowFlags(windowFlags() & ~(Qt::WindowContextHelpButtonHint) & ~(Qt::WindowMaximizeButtonHint) & ~(Qt::WindowMinimizeButtonHint));

  auto compilation_date = QString(__DATE__);
  auto compilation_time = QString(" (") + QString(__TIME__) + QString(")");

  m_compilationDate->setText(tr("Compiled on ") + compilation_date + compilation_time);
  m_version->setText(VERSION);

  QString curlVersion = tr("unknown");
  if(config.isValid())
    curlVersion = Utils::curlExecutableVersion(config.curlPath);
  m_curlVersion->setText(tr("version %1").arg(curlVersion));

  m_qtVersion->setText(tr("version %1").arg(qVersion()));
  m_copy->setText(COPYRIGHT.arg(QDateTime::currentDateTime().date().year()));


  QObject::connect(m_kofiLabel, &Utils::ClickableHoverLabel::clicked,
                   [this](){ QDesktopServices::openUrl(QUrl{"https://ko-fi.com/felixdelaspozas"}); });
}
