/*
 File: ConfigurationDialog.h
 Created on: 06/06/2024
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

#ifndef CONSOLE_OUTPUT_DIALOG_H_
#define CONSOLE_OUTPUT_DIALOG_H_

// Project
#include "ui_ConsoleOutputDialog.h"

// Qt
#include <QDialog>

/**
 * @brief Class to show console text in a dialog. 
 */
class ConsoleOutputDialog
: public QDialog
, private Ui::ConsoleOutputDialog
{
    Q_OBJECT
  public:
    /**
     * @brief ConsoleOutputDialog class constructor. 
     * @param parent Raw pointer of the widget parent of this one. 
     * @param f Window flags.
     */
    ConsoleOutputDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
    virtual ~ConsoleOutputDialog()
    {};

    /** 
     * @brief Sets the font for the "console".
     * @param font QFont reference.
     */
    void setFont(const QFont &font);

  public slots:
    /**
     * @brief Adds the given text to the console dialog. 
     * @param text Text to add
     */
    void addText(const QString &text);
};

#endif