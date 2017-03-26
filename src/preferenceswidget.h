#ifndef PREFERENCESWIDGET_H
#define PREFERENCESWIDGET_H

#include "ui_preferenceswidget.h"

class PreferencesWidget : public QWidget, private Ui::PreferencesWidget
{
    Q_OBJECT

public:
    explicit PreferencesWidget(QWidget *parent = 0);
signals:
    /**
     * @brief Tells the main window to hide preferences
     *
     */
    void hidePreferences();

    /**
     * @brief Tells the main window to change the toolbar appearance
     *
     * @param 0- text under, 1- text beside, 2- icon only, 3- text only
     *
     */
    void changeToolBarAppearance(int);

private slots:
    /**
     * @brief emits the hidePreferences() signal
     *
     */
    void on_backButton_clicked();

    /**
     * @brief enables the updateCSSButton if set to custom CSS
     *
     */
    void on_styleBox_currentIndexChanged(int index);

    /**
     * @brief emits the changeToolBarAppearance(int) signal
     *
     */
    void on_toolBarStyleBox_currentIndexChanged(int index);
};

#endif // PREFERENCESWIDGET_H
