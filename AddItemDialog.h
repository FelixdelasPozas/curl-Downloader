/*
 File: AddItemDialog.h
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

#ifndef _ADD_ITEM_DIALOG_H_
#define _ADD_ITEM_DIALOG_H_

// Project
#include "ui_AddItemDialog.h"
#include <Utils.h>

// Qt
#include <QWidget>

/**
 * @brief Dialog to add a new download url and server to use.
 */
class AddItemDialog
: public QDialog
, private Ui::AddItemDialog
{
    Q_OBJECT
  public:
    /**
     * @brief AddItemDialog class constructor.
     * @param parent Raw pointer of the widget parent of this one. 
     * @param f Dialog flags. 
     */
    AddItemDialog(QWidget *parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    /**
     * @brief AddItemDialog class virtual destructor. 
     */
    virtual ~AddItemDialog()
    {};

    /**
     * @brief Returns the item information in the dialog. 
     */
    Utils::ItemInformation getItem() const;

  protected:
    void closeEvent(QCloseEvent *) override;
};

#endif