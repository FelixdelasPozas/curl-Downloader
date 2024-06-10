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
#include <Utils.h>
#include <ConsoleOutputDialog.h>
#include "ui_ItemWidget.h"

// Qt
#include <QWidget>
#include <QProcess>

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
     * @brief config Application configuration struct reference. 
     * @param item Item information struct reference.
     * @param parent Raw pointer of thw widget parent of this one. 
     * @param f Window flags.
     */
    ItemWidget(const Utils::Configuration &m_config, Utils::ItemInformation &item, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());

    /** 
     * @brief ItemWidget class virtual destructor. 
    */
    virtual ~ItemWidget();

    /**
     * @brief Returns the item information of this widget. 
     */
    const Utils::ItemInformation &item() const
    { return m_item; }

    /**
     * @brief Returns true if the widget's item has been downloaded and false otherwise. 
     */
    bool hasFinished() const
    { return m_finished; }

    /**
     * @brief Returns true if the process has been aborted and false otherwise.
     */
    bool hasAborted() const
    { return m_aborted; }

  public slots:
    /**
     * @brief Stops the curl process.
     */
    void stopProcess();

  signals:
    void cancelled();
    void finished();

  protected:
    virtual void paintEvent(QPaintEvent *event);

  private: 
    enum class Status: char { STARTING = 0, DOWNLOADING = 1, RETRYING = 2, ERROR = 3, FINISHED = 4, ABORTED = 5 };

  private slots:
    /**
     * @brief Shows the process console in a dialog.
    */
    void onNotesButtonPressed();

    /**
     * @brief Handles curl process errors. 
     * @param error Error value. 
     */
    void onErrorOcurred(QProcess::ProcessError error);

    /**
     * @brief Handles the curl process exit status
     * @param code curl exit code.
     * @param status Process exit status. 
     */
    void onFinished(int code, QProcess::ExitStatus status);

    /**
     * @brief Handles the text produced by the curl process.
     */
    void onTextReady();

  private:
    /**
     * @brief Connects signals to slots.
    */
    void connectSignals();

    /**
     * @brief Updates the progress value and text in the progress bar. 
     * @param progressValue Progress value. 
     */
    void onProgressChanged(const float progressValue);

    /**
     * @brief Sets the text of the status label. 
     * @param status Status value.
     */
    void setStatus(const Status status);

    /**
     * @brief Starts the curl process. 
     */
    void startProcess();

  private:
    Utils::ItemInformation &m_item;       /** item information. */
    const Utils::Configuration &m_config; /** application configuration reference. */
    bool m_finished;                      /** true if the item has been downloaded and false otherwise. */
    bool m_aborted;                       /** true if aborted and false otherwise. */
    float m_progressVal;                  /** progress value in [0,100] */
    ConsoleOutputDialog m_console;        /** console text dialog. */
    QProcess m_process;                   /** curl process. */
};

#endif