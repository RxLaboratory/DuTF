#include "mainwindow.h"
#include "utils.h"

#include <QDesktopWidget>
#include <QFileDialog>
#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMenu>
#include <QMessageBox>
#include <QMimeData>
#include <QMouseEvent>
#include <QPushButton>
#include <QSpinBox>
#include <QTextEdit>

#ifdef QT_DEBUG
#include <QtDebug>
#endif

MainWindow* MainWindow::instance_ = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    Ui::MainWindow(),
    fillTableTimer(this),
    tableFreeIndex(0)
{
    setupUi(this);

    MainWindow::instance_ = this;
    // UI

    //Initialization appearance
    //The window will be correctly displayed in endInit() after everything has been created
    //small window
    this->setMinimumSize(300,0);
    QDesktopWidget *desktop = QApplication::desktop();
    this->setGeometry(desktop->screenGeometry().width()/2-150,desktop->screenGeometry().height()/2-50,300,100);

        
    // toolbar
    mainToolBar->hide();
    //Will be shown in endInit() after everything has been created
    mainToolBar->setContextMenuPolicy(Qt::PreventContextMenu);

    // Save Menu
    QMenu * saveMenu = new QMenu(this);
    saveMenu->addAction(btn_actionSave);
    saveMenu->addAction(btn_actionSaveAs);
    saveMenu->addAction(btn_actionExport);
    menu_save->setMenu(saveMenu);
    mainToolBar->insertAction(btn_actionTools,menu_save);

    // Open Menu
    QMenu * openMenu = new QMenu(this);
    openMenu->addAction(btn_actionOpen);
    openMenu->addAction(btn_actionImport);
    openMenu->addAction(btn_actionMerge);
    menu_open->setMenu(openMenu);
    mainToolBar->insertAction(menu_save,menu_open);

    // search widget
    searchWidget = new SearchWidget(this);
    mainToolBar->addWidget(searchWidget);

    // language widget
    languageWidget = new LanguageWidget(this);
    mainToolBar->addWidget(languageWidget);

    // preferences
    preferences = new PreferencesWidget(this);
    preferencesLayout->addWidget(preferences);


    // Import / Merge
    scriptParsePreferences = new ScriptParseWidget(this);
    scriptParseLayout->addWidget(scriptParsePreferences);

    // window buttons

#ifndef Q_OS_MAC
    // Maximize and minimize only on linux and windows
    this->setWindowFlags(Qt::FramelessWindowHint);
    maximizeButton = new QPushButton(QIcon(":/icons/maximize"),"");
    minimizeButton = new QPushButton(QIcon(":/icons/minimize"),"");
    mainToolBar->addWidget(minimizeButton);
    mainToolBar->addWidget(maximizeButton);
#endif

    quitButton = new QPushButton(QIcon(":/icons/close"),"");
    mainToolBar->addWidget(quitButton);

    //drag window
    toolBarClicked = false;
    mainToolBar->installEventFilter(this);

    //status
    statusLabel = new QLabel("");
    mainStatusBar->addWidget(statusLabel,10);
    //small progress bar
    progressBar = new QProgressBar();
    progressBar->setMinimum(0);
    progressBar->setMaximum(0);
    progressBar->setMaximumWidth(200);
    mainStatusBar->addPermanentWidget(progressBar);
    progressBar->hide();
    //main progress bar
    mainProgressBar->setMinimum(0);
    mainProgressBar->setMaximum(0);


    //set style
    updateCSS(preferences->getCSS());
    setToolBarAppearance(preferences->getToolBarStyle());

    mapEvents();

    //wait
    //Will be updated in endInit() after everything has been created
    setWaiting(true,tr("Initializing Dutranslator..."),MAX_AUTO_ROW);
    mainStatusBar->showMessage("Creating stuff...");

    // Timer
    fillTableTimer.setInterval(INC_TIMER);
    fillTableTimer.start();
}

MainWindow::~MainWindow(){
    jsonParser.quit();
    jsonParser.wait();
}

MainWindow & MainWindow::instance()
{
    return *(MainWindow::instance_);
}

void MainWindow::actionAbout()
{
    AboutDialog().exec();
}

void MainWindow::actionAddRow()
{
    //get the sender widget, to find the current row
    QWidget* button = qobject_cast<QWidget*>(sender());
    int currentRow = 0;
    bool found = false;
    for(currentRow = 0; currentRow < displayTable->rowCount(); ++currentRow){
      if(displayTable->cellWidget(currentRow, 0) == button){
          displayTable->selectRow(currentRow); // To avoid automatic scroll
          found = true;
          break;
      }
    }

    if(found)
    {
        addTableRow(currentRow);
        //displayTable->resizeRowsToContents(); does a terrible job
    }
}

void MainWindow::actionImport()
{
    scriptParsePreferences->setMode(ScriptParseWidget::Mode::ImportMerge);
    mainStack->setCurrentIndex(4);
    return;
}

void MainWindow::actionOpen()
{

    //get file
    QString fileName = QFileDialog::getOpenFileName(this,"Open a translation file","","JSON (*.json);;Text files (*.txt);;All files (*.*)");

    if(fileName.isEmpty()) return; // Dialog canceled
    fillTableTimer.stop();

    QFile checkFile(fileName);
    if (!checkFile.exists()) return;
    openTranslationFile(fileName);

}

void MainWindow::actionPreferences(bool checked)
{
    if (checked)
    {
        btn_actionTools->setChecked(false);
        //show the preferences page
        mainStack->setCurrentIndex(2);
    }
    else
    {
        //show the main page
        mainStack->setCurrentIndex(0);
    }
}

void MainWindow::actionSave()
{
    if (!checkLanguage()) return;

    if(workingFile.fileName() == "")
    {
        actionSaveAs();
    }


    workingFile.open(QIODevice::WriteOnly | QIODevice::Text);

    QJsonArray transList;

    //populate
    for (int row = 0; row < tableFreeIndex ; row++)
    {
        // Access widgets
        QTextEdit *originalEdit = (QTextEdit*)displayTable->cellWidget(row,1);
        QTextEdit *translatedEdit = (QTextEdit*)displayTable->cellWidget(row,2);
        QLineEdit *contextEdit = (QLineEdit*)displayTable->cellWidget(row,3);
        QLineEdit *commentEdit = (QLineEdit*)displayTable->cellWidget(row,4);
        QSpinBox *contextIdBox = (QSpinBox*)displayTable->cellWidget(row,5);

        // Access values
        QString original = utils::escape(originalEdit->toPlainText());
        QString translated = utils::escape(translatedEdit->toPlainText());
        QString context = utils::escape(contextEdit->text());
        QString comment = utils::escape(commentEdit->text());
        int contextId = contextIdBox->value();

        Translation obj {original, translated, context, comment, contextId};
        QJsonObject jRow = obj.toJson();
        transList.append(jRow);
    }

    QJsonObject jDetails
    {
        {"version", "1.0"},
        {"languageId", languageWidget->getCode()},
        {"languageName", languageWidget->getLanguage()}
    };

    QJsonObject jTranslations
    {
        {"translations", transList}
    };

    QJsonArray jContent { jDetails, jTranslations };

    QJsonObject jObj
    {
        {languageWidget->getApp(), jContent}
    };

    QJsonDocument jDoc(jObj);

    workingFile.write(jDoc.toJson());

    workingFile.close();
}

void MainWindow::actionSaveAs()
{
    if (!checkLanguage()) return;
    //get file
    QString fileName = QFileDialog::getSaveFileName(this,"Save translation file as",workingFile.fileName().left(workingFile.fileName().lastIndexOf(".")),"JSON (*.json);;Text files (*.txt);;All files (*.*)");
    if (fileName.isNull()) return;

    if(!fileName.endsWith(".json")) fileName += ".json";
    workingFile.setFileName(fileName);
    QStringList filePath = fileName.split("/");
    languageWidget->setFile(filePath[filePath.count()-1]);
    actionSave();
}

void MainWindow::actionRemoveRow()
{
        QWidget* button = qobject_cast<QWidget*>(sender());
        int deleteRow = 0;
        for(deleteRow = 0; deleteRow < displayTable->rowCount(); deleteRow++)
        {
            if(displayTable->cellWidget(deleteRow, 0) == button)
            {
                QMessageBox mb(QMessageBox::Question,"Remove Translation","Are you sure you want to remove the row " + QString::number(deleteRow+1) + "?",QMessageBox::Yes | QMessageBox::No,this,Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
                mb.exec();
                if (mb.result() == QMessageBox::Yes)
                {
                  removeTableRow(deleteRow);
                  return;
                }
            }
        }
}

void MainWindow::actionTools(bool checked)
{
    if (checked)
    {
        btn_actionPreferences->setChecked(false);
        //show the tools page
        mainStack->setCurrentIndex(3);
    }
    else
    {
        //show the main page
        mainStack->setCurrentIndex(0);
    }
}

void MainWindow::addTableRow(int index){

    bool userRow = false;

    if(index >= 0 && index < displayTable->rowCount()){
        index++;
        displayTable->insertRow(index);
        userRow = true;
    }else{
        displayTable->setRowCount(displayTable->rowCount()+1);
        index = displayTable->rowCount() - 1;
    }

    QTextEdit *originalItem = new QTextEdit();
    originalItem->setReadOnly(true);
    originalItem->setEnabled(userRow);

    QTextEdit *translatedItem = new QTextEdit();
    translatedItem->setEnabled(userRow);

    QLineEdit *contextItem = new QLineEdit();
    contextItem->setEnabled(userRow);

    QLineEdit *commentItem = new QLineEdit();
    commentItem->setEnabled(userRow);

    QSpinBox *contextIdItem = new QSpinBox();
    contextIdItem->setEnabled(userRow);

    // Actions
    RowButtonsWidget *rowButtons = new RowButtonsWidget();
    connect(rowButtons, SIGNAL(removeRow()), this, SLOT(actionRemoveRow()));
    connect(rowButtons, SIGNAL(addRow()), this, SLOT(actionAddRow()));

    displayTable->setCellWidget(index,0,rowButtons);
    displayTable->setCellWidget(index,1,originalItem);
    displayTable->setCellWidget(index,2,translatedItem);
    displayTable->setCellWidget(index,3,contextItem);
    displayTable->setCellWidget(index,4,commentItem);
    displayTable->setCellWidget(index,5,contextIdItem);

    if(displayTable->rowCount() >= MAX_AUTO_ROW && fillTableTimer.isActive())
    {
        // This function can be used outside the timer
        // so we must check if it wasn't the timer
        endInit();
    }

    if (fillTableTimer.isActive())
    {
        //if the timer is running, the progressbar must be updated
        mainProgressBar->setValue(displayTable->rowCount());
    }

    if(!userRow)
    {
        //make the UI blink on Linux, needs to check if it still the case when the table is hidden
        // Hide only if the row was added by the timer

        displayTable->setRowHidden(displayTable->rowCount() -1,true);
    }
}

void MainWindow::addTableRowContent(Translation pTr){

    if(tableFreeIndex > displayTable->rowCount() -1) addTableRow();

    QTextEdit * originalItem = dynamic_cast<QTextEdit *>(displayTable
            ->cellWidget(tableFreeIndex, 1));
    originalItem->setPlainText(utils::unEscape(pTr.source));
    originalItem->setEnabled(true);

    QTextEdit * translatedItem = dynamic_cast<QTextEdit *>(displayTable
            ->cellWidget(tableFreeIndex, 2));
    translatedItem->setPlainText(utils::unEscape(pTr.translated));
    translatedItem->setEnabled(true);

    QLineEdit * contextItem = dynamic_cast<QLineEdit *>(displayTable
            ->cellWidget(tableFreeIndex, 3));
    contextItem->setText(utils::unEscape(pTr.context));
    contextItem->setEnabled(true);

    QLineEdit * commentItem = dynamic_cast<QLineEdit *>(displayTable
            ->cellWidget(tableFreeIndex, 4));
    commentItem->setText(utils::unEscape(pTr.comment));
    commentItem->setEnabled(true);


    QSpinBox * contextIdItem = dynamic_cast<QSpinBox *>(displayTable
            ->cellWidget(tableFreeIndex, 5));
    contextIdItem->setValue(pTr.contextId);
    contextIdItem->setEnabled(true);

    displayTable->setRowHidden(tableFreeIndex,false);

    tableFreeIndex++;



}

void MainWindow::btn_generateTranslator_clicked()
{
    //get file
    QString fileName = QFileDialog::getSaveFileName(this,"Save translator file as",workingFile.fileName().left(workingFile.fileName().lastIndexOf(".")),"JavaScript (*.jsxinc *.jsx *.js);;All files (*.*)");
    if (fileName.isNull()) return;

    QFile translatorFile(":/export/Dutranslator.jsxinc");
    translatorFile.copy(fileName);
}

void MainWindow::clearSearch()
{
    mainStatusBar->showMessage("Clear!");
    progressBar->setMaximum(displayTable->rowCount()-1);
    progressBar->show();

    for (int row = 0 ; row < tableFreeIndex ; row++)
    {
        progressBar->setValue(row);

        displayTable->setRowHidden(row,false);
    }

    mainStatusBar->clearMessage();
    progressBar->hide();
}

void MainWindow::clearTableToTheEnd(){

   QTextEdit * originalItem, * translatedItem;
   QLineEdit * commentItem, * contextItem;
   QSpinBox * contextIdItem;

   int index;
   for(index = tableFreeIndex; index < displayTable->rowCount(); index++){
       originalItem = dynamic_cast<QTextEdit *>(displayTable
               ->cellWidget(index, 1));
       originalItem->setPlainText("");
       originalItem->setEnabled(false);

       translatedItem = dynamic_cast<QTextEdit *>(displayTable
               ->cellWidget(index, 2));
       translatedItem->setPlainText("");
       translatedItem->setEnabled(false);

       contextItem = dynamic_cast<QLineEdit *>(displayTable
               ->cellWidget(index, 3));
       contextItem->setText("");
       contextItem->setEnabled(false);

       commentItem = dynamic_cast<QLineEdit *>(displayTable
               ->cellWidget(index, 4));
       commentItem->setText("");
       commentItem->setEnabled(false);

       contextIdItem = dynamic_cast<QSpinBox *>(displayTable
               ->cellWidget(index, 5));
       contextIdItem->setValue(0);
       contextIdItem->setEnabled(false);

       // Make the ui blink (not on windows ; test on Mac)
       displayTable->setRowHidden(index,true);
   }
}

std::vector<Translation> MainWindow::getTranslations() const
{
    std::vector<Translation> list;

    for (int row = 0 ; row < tableFreeIndex ; row++)
    {
        Translation t;
        t.source = ((QTextEdit*)(displayTable->cellWidget(row, 1)))->toPlainText();
        t.translated = ((QTextEdit*)(displayTable->cellWidget(row, 2)))->toPlainText();
        t.context = ((QLineEdit*)(displayTable->cellWidget(row, 3)))->text();
        t.comment = ((QLineEdit*)(displayTable->cellWidget(row, 4)))->text();
        t.contextId = ((QSpinBox*)(displayTable->cellWidget(row, 5)))->value();
        list.push_back(t);
    }

    return list;
}

#ifndef Q_OS_MAC
void MainWindow::maximize()
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

void MainWindow::newApplication(QString app)
{
    languageWidget->setApp(app);
}

void MainWindow::newLanguage(QStringList language)
{
    languageWidget->setLanguage(language[1]);
    languageWidget->setCode(language[0]);
}

void MainWindow::newTranslation(Translation pTr)
{
    addTableRowContent(pTr);
}

void MainWindow::openTranslationFile(QString fileName)
{

    // Restart table
    tableFreeIndex = 0;

    workingFile.setFileName(fileName);
    QString prettyName = utils::fileName(fileName);
    languageWidget->setFile(prettyName);

    //waiting mode
    setWaiting(true,"Loading " + prettyName + "...");
    // The ui will be re-enabled when the parser sends an END signal
    mainStatusBar->showMessage("Loading...");
    statusLabel->setText(prettyName);

    //parse
    jsonParser.preParseFile(fileName);
}

void MainWindow::parsingFailed(Parser::ParsingErrors flag)
{
    mainStatusBar->clearMessage();
    progressBar->hide();
    statusLabel->clear();
    this->setEnabled(true);

    QString messageTitle = tr("Parsing failed");
    QString messageContent;

    if(flag.testFlag(Parser::ParsingError::FileOpen))
        messageContent = tr("Unable to open the file.");
    else if(flag.testFlag(Parser::ParsingError::ParsingNotImplemented))
        messageContent = tr("Parsing not implemented.");
    else
        messageContent = tr("An error has occured while parsing the file.");

    QMessageBox mb(QMessageBox::Warning,
                   messageTitle, messageContent,
                   QMessageBox::Ok,
                   this,
                   Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
    mb.exec();
}

void MainWindow::parsingFinished()
{
    //resize sections
    clearTableToTheEnd();
    displayTable->resizeColumnsToContents();
    adjustColumnSizes();

    mainStatusBar->clearMessage();
    setWaiting(false);

}

void MainWindow::removeTableRow(int index)
{
          displayTable->selectRow(index); // To avoid automatic scroll
          displayTable->removeRow(index);
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

    for (int row = 0 ; row < tableFreeIndex ; row++)
    {
        progressBar->setValue(row);
        bool found = false;
        //TODO highlight found strings
        if (original || (!original && !translated && !comment))
        {
            QTextEdit *originalEdit = (QTextEdit*)displayTable->cellWidget(row,1);
            if (originalEdit->toPlainText().indexOf(s,0,caseSensitive) >= 0) found = true;
        }
        if (translated || (!original && !translated && !comment))
        {
            QTextEdit *translatedEdit = (QTextEdit*)displayTable->cellWidget(row,3);
            if (translatedEdit->toPlainText().indexOf(s,0,caseSensitive) >= 0) found = true;
        }
        if (comment || (!original && !translated && !comment))
        {
            QLineEdit *commentEdit = (QLineEdit*)displayTable->cellWidget(row,4);
            if (commentEdit->text().indexOf(s,0,caseSensitive) >= 0) found = true;
        }
        displayTable->setRowHidden(row,!found);
    }

    mainStatusBar->clearMessage();
    progressBar->hide();
}

void MainWindow::setToolBarAppearance(int appearance)
{
    if (appearance == 0)
    {
        this->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    }
    else if (appearance == 1)
    {
        this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    }
    else if (appearance == 2)
    {
        this->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
    else if (appearance == 3)
    {
        this->setToolButtonStyle(Qt::ToolButtonTextOnly);
    }
}

void MainWindow::setWaiting(bool wait, QString status, int max)
{
    mainProgressBar->setFormat(status + " | %p%");
    mainProgressBar->setMaximum(max);
    if (wait)
    {
        //show the progress bar page
        mainStack->setCurrentIndex(1);
        btn_actionAbout->setEnabled(false);
        btn_actionOpen->setEnabled(false);
        btn_actionSave->setEnabled(false);
        btn_actionSaveAs->setEnabled(false);
        btn_actionPreferences->setEnabled(false);
        languageWidget->setEnabled(false);
        searchWidget->setEnabled(false);
    }
    else
    {
        //show the display page
        mainStack->setCurrentIndex(0);
        btn_actionAbout->setEnabled(true);
        btn_actionOpen->setEnabled(true);
        btn_actionSave->setEnabled(true);
        btn_actionSaveAs->setEnabled(true);
        btn_actionPreferences->setEnabled(true);
        languageWidget->setEnabled(true);
        searchWidget->setEnabled(true);
    }
}

void MainWindow::showMainPage()
{
    btn_actionPreferences->setChecked(false);
    mainStack->setCurrentIndex(0);
}

void MainWindow::startImportPorcess(StringParser::TranslationParsingModes flags)
{
    //get file
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Import strings from a source file"),
                                                    "",
                                                    "All files (*.*)");
    if(fileName.isEmpty()) return; // Dialog canceled
    QFile file(fileName);

    fillTableTimer.stop();

    tableFreeIndex = 0;
    languageWidget->clear();
    workingFile.setFileName("");

    file.fileName();
    //waiting mode
    QString prettyName = utils::fileName(fileName);
    setWaiting(true,tr("Loading file %1...").arg(prettyName));
    // The ui will be re-enabled when the parser sends an END signal
    mainStatusBar->showMessage("Loading...");

    //parse
    stringParser.setMode(flags);
    stringParser.preParseFile(fileName);
    //jsonParser->parseFile(&workingFile);
}

void MainWindow::updateCSS(QString cssFileName)
{
    setWaiting(true,tr("Loading StyleSheet"),0);
    mainStatusBar->showMessage(tr("Loading CSS: ") + cssFileName);
    repaint();
    QString css = "";

    QFile cssFile(cssFileName);
    if (cssFile.exists())
    {
        cssFile.open(QFile::ReadOnly);
        css = QString(cssFile.readAll());
        cssFile.close();
    }

    qApp->setStyleSheet(css);
    setWaiting(false);
    btn_actionPreferences->setChecked(false);
    mainStatusBar->clearMessage();
}

void MainWindow::adjustColumnSizes()
{
    //get available space
    int availableWidth = displayTable->width();
    //get occupied space
    //header
    int occupiedWidth = displayTable->verticalHeader()->width();
    //and each column
    for (int column = 0;column < displayTable->columnCount();column++)
    {
        occupiedWidth += displayTable->columnWidth(column);
        //don't forget margins
        occupiedWidth += 3;
    }

    //use available space
    int emptySpace = availableWidth - occupiedWidth;
    if (emptySpace > 0)
    {
        displayTable->setColumnWidth(1,displayTable->columnWidth(1)+emptySpace/2);
        displayTable->setColumnWidth(3,displayTable->columnWidth(3)+emptySpace/2);
    }
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
    if (languageWidget->getApp() == "")
    {
        QMessageBox mb(QMessageBox::Information,tr("Wront application name"),
                       tr("You must specify an application name"),QMessageBox::Ok,this,Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
        mb.exec();
        return false;
    }
    return true;
}

void MainWindow::endInit()
{
    //stop filling the table
    fillTableTimer.stop();
    //update UI
    QDesktopWidget *desktop = QApplication::desktop();
    this->setGeometry(desktop->screenGeometry().width()/2-720,desktop->screenGeometry().height()/2-360,1440,720);
    this->setMinimumSize(1200,0);
    mainToolBar->show();
    //end wainting
    mainStatusBar->clearMessage();
    setWaiting(false);
}

void MainWindow::mapEvents(){
    // Parser
    connect(&jsonParser,SIGNAL(languageFound(QStringList)),this,SLOT(newLanguage(QStringList)));
    connect(&jsonParser,SIGNAL(applicationFound(QString)),this,SLOT(newApplication(QString)));
    connect(&jsonParser,SIGNAL(newTranslation(Translation)),this,SLOT(newTranslation(Translation)));
    connect(&jsonParser,SIGNAL(parsingFinished()),this,SLOT(parsingFinished()));
    connect(&jsonParser,SIGNAL(parsingFailed(Parser::ParsingErrors)),this,SLOT(parsingFailed(Parser::ParsingErrors)));
    connect(&jsonParser,SIGNAL(progress(int)),progressBar,SLOT(setValue(int)));
    connect(&jsonParser,SIGNAL(progress(int)),mainProgressBar,SLOT(setValue(int)));

    connect(&stringParser,SIGNAL(parsingFinished()),this,SLOT(parsingFinished()));
    connect(&stringParser,SIGNAL(parsingFailed(Parser::ParsingErrors)),this,SLOT(parsingFailed(Parser::ParsingErrors)));
    connect(&stringParser,SIGNAL(newTranslation(Translation)),this,SLOT(newTranslation(Translation)));
    connect(&stringParser,SIGNAL(progress(int)),progressBar,SLOT(setValue(int)));
    connect(&stringParser,SIGNAL(progress(int)),mainProgressBar,SLOT(setValue(int)));

    // Actions
    connect(this->btn_actionSaveAs, SIGNAL(triggered(bool)), this, SLOT(actionSaveAs()));
    connect(this->btn_actionSave, SIGNAL(triggered(bool)), this, SLOT(actionSave()));
    connect(this->menu_save, SIGNAL(triggered(bool)), this, SLOT(actionSave()));
    connect(this->btn_actionOpen, SIGNAL(triggered(bool)), this, SLOT(actionOpen()));
    connect(this->btn_actionImport, SIGNAL(triggered(bool)), this, SLOT(actionImport()));
    connect(this->menu_open, SIGNAL(triggered(bool)), this, SLOT(actionOpen()));
    connect(this->btn_actionAbout, SIGNAL(triggered(bool)), this, SLOT(actionAbout()));
    connect(this->btn_actionPreferences, SIGNAL(triggered(bool)), this, SLOT(actionPreferences(bool)));
    connect(this->btn_actionTools, SIGNAL(triggered(bool)), this, SLOT(actionTools(bool)));

    // Search
    connect(searchWidget,SIGNAL(search(QString)),this,SLOT(search(QString)));
    connect(searchWidget,SIGNAL(clear()),this,SLOT(clearSearch()));

    // Tools
    connect(btn_generateTranslator,SIGNAL(clicked()),this,SLOT(btn_generateTranslator_clicked()));

    // Preferences
    connect(preferences,SIGNAL(hidePreferences()),this,SLOT(showMainPage()));
    connect(preferences,SIGNAL(changeToolBarAppearance(int)),this,SLOT(setToolBarAppearance(int)));
    connect(preferences,SIGNAL(changeCSS(QString)),this,SLOT(updateCSS(QString)));

    // Import / Merge
    connect(scriptParsePreferences, SIGNAL(canceled()), this, SLOT(showMainPage()));
    connect(scriptParsePreferences, SIGNAL(importOptionsSaved(StringParser::TranslationParsingModes)), this, SLOT(showMainPage()));
    connect(scriptParsePreferences, SIGNAL(importOptionsSaved(StringParser::TranslationParsingModes)), this,
            SLOT(startImportPorcess(StringParser::TranslationParsingModes)));

    // Window management
#ifndef Q_OS_MAC
    // Windows and linux
    connect(maximizeButton,SIGNAL(clicked()),this,SLOT(maximize()));
    connect(minimizeButton,SIGNAL(clicked()),this,SLOT(showMinimized()));
#endif
    connect(quitButton,SIGNAL(clicked()),qApp,SLOT(quit()));

    // Timer
    connect(&fillTableTimer, SIGNAL(timeout()), this, SLOT(addTableRow()));

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
            openTranslationFile(fileName);
        }
    }
    else if (mimeData->hasText())
    {
        //get text
        QString text = mimeData->text();
        //if it is a file path, open the file
        if (QFile(text).exists())
        {
            openTranslationFile(text);
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
            jsonParser.preParseText(text);
        }
    }

    event->acceptProposedAction();
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
      maximize();
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

void MainWindow::resizeEvent(QResizeEvent *event)
{
    adjustColumnSizes();
}
