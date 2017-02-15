#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    //load default stylesheet
    updateCSS();

    this->setWindowFlags(Qt::FramelessWindowHint);


    // UI
    //language widget
    languageWidget = new LanguageWidget(this);
    mainToolBar->addWidget(languageWidget);
    //Add window buttons
#ifndef Q_OS_MAC
    //Minimize and maximize (not on mac, needs testing on linux)
    maximizeButton = new QPushButton(QIcon(":/icons/maximize"),"");
    QPushButton *minimizeButton = new QPushButton(QIcon(":/icons/minimize"),"");
    connect(maximizeButton,SIGNAL(clicked()),this,SLOT(maximizeButton_clicked()));
    connect(minimizeButton,SIGNAL(clicked()),this,SLOT(showMinimized()));
    mainToolBar->addWidget(minimizeButton);
    mainToolBar->addWidget(maximizeButton);
#endif
    QPushButton *quitButton = new QPushButton(QIcon(":/icons/close"),"");
    connect(quitButton,SIGNAL(clicked()),qApp,SLOT(quit()));
    mainToolBar->addWidget(quitButton);
    //drag window
    toolBarClicked = false;
    mainToolBar->installEventFilter(this);
}

void MainWindow::updateCSS()
{
#ifdef Q_OS_MAC
    QDir bundle = QCoreApplication::applicationDirPath();
    bundle.cdUp();
    QFile cssFile(bundle.path() + "/Resources/" + "style.css");
#else
    QFile cssFile("style.css");
#endif
    cssFile.open(QFile::ReadOnly);
    QString css = QString(cssFile.readAll());
    cssFile.close();
    qApp->setStyleSheet(css);
}

QString MainWindow::unEscape(QString s)
{
    s = s.replace("\\n","\n");
    s = s.replace("\\\"","\"");
    return s;
}

QString MainWindow::escape(QString s)
{
    s = s.replace("\n","\\n");
    s = s.replace("\"","\\\"");
    return s;
}

void MainWindow::on_actionOpen_triggered()
{
    //get file
    QString fileName = QFileDialog::getOpenFileName(this,"Open a translation file","","JavaScript (*.jsx *.jsxinc *.js);;Text files (*.txt);;All files (*.*)");
    if (fileName.isNull()) return;

    //clear table
    displayTable->clearContents();
    displayTable->setRowCount(0);

    this->setEnabled(false);
    this->repaint();

    workingFile.setFileName(fileName);
    QStringList filePath = fileName.split("/");
    languageWidget->setFile(filePath[filePath.count()-1]);

    //parse file
    workingFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(&workingFile);
    //set to UTF8 by default
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    in.setCodec(codec);
    //line
    QString line;
    while (!in.atEnd())
    {
        qint64 pos = in.pos();
        line = in.readLine().trimmed();
        //if not UTF-8, try locale
        if (line.indexOf("�") > 0)
        {
            QTextCodec *codec = QTextCodec::codecForLocale();
            in.setCodec(codec);
            in.seek(pos);
            line = in.readLine().trimmed();
        }
        if (line.startsWith("DutranslatorArray.push("))
        {
            //get the first string
            QString rxString = "\\\"([^\\\"\\\\]*(?:\\\\.[^\\\"\\\\]*)*?)\\\",(\\d*),\\\"([^\\\"\\\\]*(?:\\\\.[^\\\"\\\\]*)*?)\\\"";
            QRegularExpression rx(rxString);
            QRegularExpressionMatch match = rx.match(line);
            if (match.hasMatch())
            {
                QString original = match.captured(1);
                int context = match.captured(2).toInt();
                QString translated = match.captured(3);
                QTextEdit *originalItem = new QTextEdit();
                originalItem->setPlainText(unEscape(original));
                originalItem->setReadOnly(true);
                QSpinBox *contextItem = new QSpinBox();
                contextItem->setValue(context);
                QTextEdit *translatedItem = new QTextEdit();
                translatedItem->setPlainText(unEscape(translated));
                displayTable->setRowCount(displayTable->rowCount()+1);
                displayTable->setCellWidget(displayTable->rowCount()-1,0,originalItem);
                displayTable->setCellWidget(displayTable->rowCount()-1,1,contextItem);
                displayTable->setCellWidget(displayTable->rowCount()-1,2,translatedItem);
            }
        }
        else if (line.startsWith("Dutranslator.languages.push"))
        {
            //get language code
            QString rxString = "'([^\\\\\\*\\.\\s/]+)','([^\\\\\\*\\.\\s/]+)'";
            QRegularExpression rx(rxString);
            QRegularExpressionMatch match = rx.match(line);
            if (match.hasMatch())
            {
                QString langCode = match.captured(1);
                QString lang = match.captured(2);
                languageWidget->setLanguage(lang);
                languageWidget->setCode(langCode);
            }
        }
    }
    workingFile.close();

    //resize sections
    displayTable->resizeColumnsToContents();

    this->setEnabled(true);
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::MouseButtonPress)
  {
      QMouseEvent *mouseEvent = (QMouseEvent*)event;
      if (mouseEvent->button() == Qt::LeftButton)
      {
        toolBarClicked = true;
        dragPosition = mouseEvent->globalPos() - this->frameGeometry().topLeft();
        event->accept();
      }
      return true;
  }
  else if (event->type() == QEvent::MouseMove)
  {
    QMouseEvent *mouseEvent = (QMouseEvent*)event;
    if (mouseEvent->buttons() & Qt::LeftButton && toolBarClicked)
    {
        if (this->isMaximized()) this->showNormal();
        this->move(mouseEvent->globalPos() - dragPosition);
        event->accept();
    }
    return true;
  }
  else if (event->type() == QEvent::MouseButtonRelease)
  {
      toolBarClicked = false;
      return true;
  }
#ifndef Q_OS_MAC
  else if (event->type() == QEvent::MouseButtonDblClick)
  {
      maximizeButton_clicked();
      event->accept();
      return true;
  }
#endif
  else
  {
      // standard event processing
      return QObject::eventFilter(obj, event);
  }
}

bool MainWindow::checkLanguage()
{
    if (languageWidget->getCode() == "")
    {
        QMessageBox::warning(this,"Wrong language code","You must specify a language code");
        return false;
    }
    if (languageWidget->getLanguage() == "")
    {
        QMessageBox::warning(this,"Wrong language name","You must specify a language name");
        return false;
    }
    return true;
}

void MainWindow::on_actionSave_triggered()
{
    if (!checkLanguage()) return;

    workingFile.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&workingFile);
    out.setCodec(QTextCodec::codecForName("UTF-8"));

    //add new array
    out << "Dutranslator.languages.push(['" + languageWidget->getCode() + "','" +  languageWidget->getLanguage() + "']);" << endl;
    out << "var DutranslatorArray = [];" << endl;

    //populate
    for (int row = 0; row < displayTable->rowCount() ; row++)
    {
        QTextEdit *originalEdit = (QTextEdit*)displayTable->cellWidget(row,0);
        QSpinBox *contextBox = (QSpinBox*)displayTable->cellWidget(row,1);
        QTextEdit *translatedEdit = (QTextEdit*)displayTable->cellWidget(row,2);
        QString original = originalEdit->toPlainText();
        original = escape(original);
        QString translated = translatedEdit->toPlainText();
        translated = escape(translated);
        QString context = QString::number(contextBox->value());
        out << "DutranslatorArray.push([\"" << original << "\"," << context << ",\"" << translated << "\"]);" << endl;
    }

    //add array and free memory
    out << "Dutranslator.localizedStrings.push(DutranslatorArray);" << endl;
    out << "delete DutranslatorArray;";

    workingFile.close();
}

void MainWindow::on_actionSave_as_triggered()
{
    if (!checkLanguage()) return;
    //get file
    QString fileName = QFileDialog::getSaveFileName(this,"Save translation file as",workingFile.fileName().left(workingFile.fileName().lastIndexOf(".")),"JavaScript (*.jsxinc *.jsx *.js);;Text files (*.txt);;All files (*.*)");
    if (fileName.isNull()) return;

    workingFile.setFileName(fileName);
    QStringList filePath = fileName.split("/");
    languageWidget->setFile(filePath[filePath.count()-1]);
    on_actionSave_triggered();
}

void MainWindow::on_actionAbout_triggered()
{
    AboutDialog().exec();
}

#ifndef Q_OS_MAC
void MainWindow::maximizeButton_clicked()
{
    if (this->isMaximized())
    {
        maximizeButton->setIcon(QIcon(":/icons/maximize"));
        this->showNormal();
    }
    else
    {
        maximizeButton->setIcon(QIcon(":/icons/minimize2"));
        this->showMaximized();
    }

}
#endif
