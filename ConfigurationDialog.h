/*
 File: ConfigurationDialog.h
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

#ifndef _CONFIGURATION_DIALOG_H_ 
#define _CONFIGURATION_DIALOG_H_ 

// Project
#include "ui_ConfigurationDialog.h"

/**
 * @brief Configuration dialog.
 */
class ConfigurationDialog
: public QDialog
, private Ui::ConfigurationDialog
{
    Q_OBJECT
  public:
    /**
     * @brief ConfigurationDialog class constructor. 
     * @param parent Raw pointer of the widget parent of this one. 
     * @param f Dialog flags. 
     */
    ConfigurationDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    /**
     * @brief ConfigurationDialog class virtual destructor. 
     */
    virtual ~ConfigurationDialog()
    {};

  private:
    /**
     * @brief Connects signals to slots. 
     */
    void connectSignals();
};

#endif


