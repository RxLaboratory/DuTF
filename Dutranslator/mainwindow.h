#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ui_mainwindow.h"
#include <QStyleFactory>
#include <QFile>
#include <QLabel>
#include <QFileDialog>
#include <QJsonValue>
#include <QMouseEvent>
#include <QPushButton>
#include <QTextEdit>
#include <QSpinBox>
#include <QMessageBox>
#include <QTextStream>
#include "aboutdialog.h"
#include "languagewidget.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
private slots:
    void on_actionOpen_triggered();
    // =======WINDOW BUTTONS
#ifndef Q_OS_MAC
    void maximizeButton_clicked();
#endif
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();

    void on_actionAbout_triggered();

private:
    //METHODS
    void updateCSS();
    QString unEscape(QString s);
    QString escape(QString s);
    bool checkLanguage();
    //OBJECTS
    QFile workingFile;
    //drag main window
    QPoint dragPosition;
    bool toolBarClicked;
    //buttons
#ifndef Q_OS_MAC
    QPushButton *maximizeButton;
#endif
    //language
    LanguageWidget *languageWidget;

protected:
    //events
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
