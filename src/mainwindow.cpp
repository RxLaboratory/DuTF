#include "mainwindow.h"
#ifdef QT_DEBUG
#include <QtDebug>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);

    //load default stylesheet
    updateCSS();

    // UI
    //search widget
    searchWidget = new SearchWidget(this);
    connect(searchWidget,SIGNAL(search(QString)),this,SLOT(search(QString)));
    connect(searchWidget,SIGNAL(clear()),this,SLOT(clearSearch()));
    mainToolBar->addWidget(searchWidget);
    //language widget
    languageWidget = new LanguageWidget(this);
    mainToolBar->addWidget(languageWidget);
    //Add window buttons
#ifndef Q_OS_MAC
    this->setWindowFlags(Qt::FramelessWindowHint);
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
    //status
    statusLabel = new QLabel("Ready");
    mainStatusBar->addWidget(statusLabel,10);
    progressBar = new QProgressBar();
    progressBar->setMinimum(0);
    progressBar->setMaximum(0);
    progressBar->setMaximumWidth(200);
    mainStatusBar->addPermanentWidget(progressBar);
    progressBar->hide();

    //Parser
    jsxParser = new JsxParser();
    jsxParser->moveToThread(&parserThread);
    connect(jsxParser,SIGNAL(languageFound(QStringList)),this,SLOT(newLanguage(QStringList)));
    connect(jsxParser,SIGNAL(newTranslation(QStringList)),this,SLOT(newTranslation(QStringList)));
    connect(jsxParser,SIGNAL(parsingFinished()),this,SLOT(jsxParsed()));
    connect(jsxParser,SIGNAL(progress(int)),progressBar,SLOT(setValue(int)));
}

void MainWindow::updateCSS()
{
#ifdef Q_OS_MAC // Maybe not needed anymore ?
    //QDir bundle = QCoreApplication::applicationDirPath();
    //bundle.cdUp();
    //QFile cssFile(bundle.path() + "/Resources/" + "style.css");
    QFile cssFile(":/styles/default");
#else
    QFile cssFile(":/styles/default");
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
    this->setEnabled(false);

    //get file
    QString fileName = QFileDialog::getOpenFileName(this,"Open a translation file","","JavaScript (*.jsx *.jsxinc *.js);;Text files (*.txt);;All files (*.*)");

    openJsxinc(fileName);

    this->setEnabled(true);
}

void MainWindow::openJsxinc(QString fileName)
{
    if (fileName.isNull()) return;

    workingFile.setFileName(fileName);
    QStringList filePath = fileName.split("/");
    QString displayFileName = filePath[filePath.count()-1];
    languageWidget->setFile(displayFileName);

    //clear table
    displayTable->clearContents();
    displayTable->setRowCount(0);
    //parse
    this->setEnabled(false);
    mainStatusBar->showMessage("Loading...");
    progressBar->setMaximum(100);
    progressBar->show();
    this->repaint();
    jsxParser->parseJsxinc(&workingFile);
    statusLabel->setText(displayFileName);
}

void MainWindow::newTranslation(QStringList translation)
{
    QString original = translation[0];
    int context = translation[1].toInt();
    QString translated = translation[2];
    QString comment = translation[3];
    QTextEdit *originalItem = new QTextEdit();
    originalItem->setPlainText(unEscape(original));
    originalItem->setReadOnly(true);
    QSpinBox *contextItem = new QSpinBox();
    contextItem->setValue(context);
    QTextEdit *translatedItem = new QTextEdit();
    translatedItem->setPlainText(unEscape(translated));
    QLineEdit *commentItem = new QLineEdit();
    commentItem->setText(unEscape(comment));
    displayTable->setRowCount(displayTable->rowCount()+1);
    displayTable->setCellWidget(displayTable->rowCount()-1,0,originalItem);
    displayTable->setCellWidget(displayTable->rowCount()-1,1,contextItem);
    displayTable->setCellWidget(displayTable->rowCount()-1,2,translatedItem);
    displayTable->setCellWidget(displayTable->rowCount()-1,3,commentItem);

    //resize sections
    displayTable->resizeColumnsToContents();
}

void MainWindow::newLanguage(QStringList language)
{
    languageWidget->setLanguage(language[1]);
    languageWidget->setCode(language[0]);
}

void MainWindow::jsxParsed()
{
    mainStatusBar->clearMessage();
    progressBar->hide();
    this->setEnabled(true);
}

bool MainWindow::checkLanguage()
{

    if (languageWidget->getCode() == "")
    {
        QMessageBox mb(QMessageBox::Information,"Wrong language code","You must specify a language code",QMessageBox::Ok,this,Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
        mb.exec();
        return false;
    }
    if (languageWidget->getLanguage() == "")
    {
        QMessageBox mb(QMessageBox::Information,"Wrong language name","You must specify a language name",QMessageBox::Ok,this,Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
        mb.exec();
        return false;
    }
    return true;
}

void MainWindow::search(QString s)
{
    mainStatusBar->showMessage("Searching...");
    progressBar->setMaximum(displayTable->rowCount()-1);
    progressBar->show();

    bool original = searchWidget->searchOriginal();
    bool translated = searchWidget->searchTranslated();
    bool comment = searchWidget->searchComment();
    Qt::CaseSensitivity caseSensitive = Qt::CaseInsensitive;
    if (searchWidget->caseSensitive()) caseSensitive = Qt::CaseSensitive;

    for (int row = 0 ; row < displayTable->rowCount() ; row++)
    {
        progressBar->setValue(row);
        bool found = false;
        //TODO highlight found strings
        if (original || (!original && !translated && !comment))
        {
            QTextEdit *originalEdit = (QTextEdit*)displayTable->cellWidget(row,0);
            if (originalEdit->toPlainText().indexOf(s,0,caseSensitive) >= 0) found = true;
        }
        if (translated || (!original && !translated && !comment))
        {
            QTextEdit *translatedEdit = (QTextEdit*)displayTable->cellWidget(row,2);
            if (translatedEdit->toPlainText().indexOf(s,0,caseSensitive) >= 0) found = true;
        }
        if (comment || (!original && !translated && !comment))
        {
            QLineEdit *commentEdit = (QLineEdit*)displayTable->cellWidget(row,3);
            if (commentEdit->text().indexOf(s,0,caseSensitive) >= 0) found = true;
        }
        displayTable->setRowHidden(row,!found);
    }

    mainStatusBar->clearMessage();
    progressBar->hide();
}

void MainWindow::clearSearch()
{
    mainStatusBar->showMessage("Clear!");
    progressBar->setMaximum(displayTable->rowCount()-1);
    progressBar->show();

    for (int row = 0 ; row < displayTable->rowCount() ; row++)
    {
        progressBar->setValue(row);

        displayTable->setRowHidden(row,false);
    }

    mainStatusBar->clearMessage();
    progressBar->hide();
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
        QLineEdit *commentEdit = (QLineEdit*)displayTable->cellWidget(row,3);
        QString original = originalEdit->toPlainText();
        original = escape(original);
        QString translated = translatedEdit->toPlainText();
        translated = escape(translated);
        QString context = QString::number(contextBox->value());
        QString comment = commentEdit->text();
        out << "DutranslatorArray.push([\"" << original << "\"," << context << ",\"" << translated << "\"]);";
        if (comment != "")
        {
            out << " //" << comment;
        }
        out << endl;
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

//DRAG AND DROP

void MainWindow::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData();

    //TODO ask if merge or open

    if (mimeData->hasUrls())
    {
        //open the first file found
        QUrl url = mimeData->urls()[0];
        QString fileName = url.toLocalFile();
        //only if it exists
        if (QFile(fileName).exists())
        {
            openJsxinc(fileName);
        }
    }
    else if (mimeData->hasText())
    {
        //get text
        QString text = mimeData->text();
        //if it is a file path, open the file
        if (QFile(text).exists())
        {
            openJsxinc(text);
        }
        //else try to parse the text
        else
        {
            //clear table
            displayTable->clearContents();
            displayTable->setRowCount(0);
            mainStatusBar->showMessage("Loading");
            progressBar->setMaximum(100);
            progressBar->show();
            //parse
            jsxParser->parseJsxinc(&text);
        }
    }

    event->acceptProposedAction();
    this->setEnabled(true);
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    mainStatusBar->showMessage("Drop any file or text here to load it.");
    progressBar->show();
    event->acceptProposedAction();
    this->setEnabled(false);
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
     event->acceptProposedAction();
}

void MainWindow::dragLeaveEvent(QDragLeaveEvent *event)
{
    mainStatusBar->clearMessage();
    progressBar->hide();
    this->setEnabled(true);
    event->accept();
}

//EVENT FILTER

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
