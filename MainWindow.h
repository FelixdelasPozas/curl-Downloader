/*
 File: MainWindow.h
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

#ifndef _DOWNLOADER_MAINWINDOW_H
#define _DOWNLOADER_MAINWINDOW_H

// Process
#include "ui_MainWindow.h"
#include <Utils.h>

// Qt
#include <QMainWindow>

class ItemWidget;

/**
 * @brief MainWindow class. 
 */
class MainWindow
: public QMainWindow
, private Ui::MainWindow
{
    Q_OBJECT
  public:
    /**
     * @brief MainWindow class constructor.
     * @param parent Raw pointer of the widget parent of this one. 
     * @param flags Window flags. 
     */
    MainWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());

    /**
     * @brief MainWindow class virtual destructor. 
     */
    virtual ~MainWindow();

  private slots:
    /** 
     * @brief Shows the about dialog. 
    */
    void showAboutDialog();

    /**
     * @brief Adds a new item to download.
     */
    void addItem();

    /**
     * @brief Shows the configuration dialog and stores the changes, if any.
     */
    void showConfigurationDialog();

    /**
     * @brief Manages UI when a donwload finishes or is cancelled..
     */
    void onProcessFinished();

  private:
    /**
     * @brief Connects the signals to the slots. 
     */
    void connectSignals();

    /**
     * @brief Load application settings from the registry. 
     */
    void loadSettings();

    /**
     * @brief Save application settings of the registry.
     */
    void saveSettings();  

  private:
    Utils::Configuration m_config;               /** application configuration. */
    std::vector<Utils::ItemInformation> m_items; /** list of items being downloaded. */
    std::vector<ItemWidget *> m_widgets;         /** list of item widgets. */
};


#endif