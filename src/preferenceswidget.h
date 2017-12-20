#ifndef PREFERENCESWIDGET_H
#define PREFERENCESWIDGET_H

#include "ui_preferenceswidget.h"
#include <QFileDialog>
#include <QSettings>

class PreferencesWidget : public QWidget, private Ui::PreferencesWidget
{
    Q_OBJECT

public:
    explicit PreferencesWidget(QWidget *parent = 0);

    /**
     * @brief Returns the CSS File Name
     */
    QString getCSS();

    /**
     * @brief Returns the toolBar style
     * 0- text under, 1- text beside, 2- icon only, 3- text only
     */
    int getToolBarStyle();

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

    /**
     * @brief Tells the main window to change CSS
     *
     * @param CSS filename
     *
     */
    void changeCSS(QString);

private slots:
    /**
     * @brief emits the hidePreferences() signal
     *
     */
    void on_backButton_clicked();

    /**
     * @brief enables the updateCSSButton if set to custom CSS
     * and emits changeCSS(QString) according to the choice of the user
     *
     */
    void on_styleBox_currentIndexChanged(int index);

    /**
     * @brief emits the changeToolBarAppearance(int) signal
     *
     */
    void on_toolBarStyleBox_currentIndexChanged(int index);

    /**
     * @brief emits changeCSS(QString) to tell the mainwindow to update CSS
     *
     */
    void on_updateCSSButton_clicked();

    /**
     * @brief Handle change of the language
     * @param index
     */
    void on_languageBox_currentIndexChanged(int index);

private:

    /**
     * @brief App settings
     */
    QSettings settings_;

    /**
     * @brief Current index in the language combo
     */
    int currentLanguage_;
};

#endif // PREFERENCESWIDGET_H
