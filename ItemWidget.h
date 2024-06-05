/*
 File: ItemWidget.h
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

#ifndef _ITEM_WIDGET_H_
#define _ITEM_WIDGET_H_

// Project
#include <AddItemDialog.h>
#include "ui_ItemWidget.h"

// Qt
#include <QWidget>

/**
 * @brief Widget for the list widget representing an item. 
 */
class ItemWidget
: public QWidget
, private Ui::ItemWidget
{
    Q_OBJECT
  public:
    /**
     * @brief ItemWidget class constructor. 
     * @param item Item information struct.
     * @param parent Raw pointer of thw widget parent of this one. 
     * @param f Window flags.
     */
    ItemWidget(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    /** 
     * @brief ItemWidget class virtual destructor. 
    */
    virtual ~ItemWidget();

  signals:
    void cancelled();
    void finished();

  private slots:
    /** 
     * @brief Cancels the thread and finishes.
    */
    void onCancelButtonPressed();

    /**
     * @brief Shows the process console in a dialog.
    */
    void onNotesButtonPressed();

    /**
     * @brief Updates the progress value and text in the progress bar. 
     * @param progressValue Progress value. 
     * @param text Progress text. 
     */
    void onProgressModified(const unsigned int progressValue = 0u, const QString &text = QString());

  private:
    /**
     * @brief Connects signals to slots.
    */
    void connectSignals();
  
};

#endif