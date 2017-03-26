#ifndef ROWBUTTONSWIDGET_H
#define ROWBUTTONSWIDGET_H

#include "ui_rowbuttonswidget.h"

class RowButtonsWidget : public QWidget, private Ui::RowButtonsWidget
{
    Q_OBJECT

public:
    /**
     * @brief Default constructor
     *
     * @param parent    the parent widget
     */
    explicit RowButtonsWidget(QWidget *parent = 0);

signals:
    /**
     * @brief Emitted when the remove button is clicked
     *
     * @param the index of the current row
     */
    void removeRow();
    /**
     * @brief Emitted when the add button is clicked
     *
     * @param the index of the current row
     */
    void addRow();


private slots:
    /**
     * @brief Sends the removeRow signal
     */
    void on_removeButton_clicked();

    /**
     * @brief Sends the addRow signal
     */
    void on_addButton_clicked();
};

#endif // ROWBUTTONSWIDGET_H
