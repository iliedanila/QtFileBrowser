#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileoperation.h"

#include <QDir>
#include <QProgressDialog>
#include <QProcess>
#include <QInputDialog>
#include <QKeyEvent>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    lastActiveBrowser = ui->leftBrowser;
    findFiles = new FindFilesDialog(this);

    CustomizeUI();
    Connect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F1 && event->modifiers() == Qt::NoModifier)
    {
        ui->leftBrowser->toggleDriveMenu();
    }
    else if (event->key() == Qt::Key_F2 && event->modifiers() == Qt::NoModifier)
    {
        ui->rightBrowser->toggleDriveMenu();
    }
    else
    {
        QMainWindow::keyPressEvent(event);
    }
}

void MainWindow::CustomizeUI()
{
    QFile style(":qdarkstyle/style.qss");
    if (style.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream textStream(&style);
        darkTheme = textStream.readAll();
    }

    findFiles->setWindowModality(Qt::WindowModal);
    ui->leftBrowser->setFocus();
}

void MainWindow::Connect()
{
    bool connected = true;

    connected &= connect(ui->leftBrowser, SIGNAL(switchMe()), this, SLOT(switchToRightBrowser())) != Q_NULLPTR;
    connected &= connect(ui->leftBrowser, &BrowserWidget::gotFocus, [this]{ lastActiveBrowser = ui->leftBrowser;}) != Q_NULLPTR;
    connected &= connect(ui->leftBrowser, SIGNAL(search()), this, SLOT(handleSearch())) != Q_NULLPTR;
    connected &= connect(ui->leftBrowser, SIGNAL(copy()), this, SLOT(handleCopy())) != Q_NULLPTR;
    connected &= connect(ui->leftBrowser, SIGNAL(move()), this, SLOT(handleMove())) != Q_NULLPTR;
    connected &= connect(ui->leftBrowser, SIGNAL(del()), this, SLOT(handleDel())) != Q_NULLPTR;
    connected &= connect(ui->leftBrowser, SIGNAL(newFolder()), this, SLOT(handleNewFolder())) != Q_NULLPTR;

    connected &= connect(ui->rightBrowser, SIGNAL(switchMe()), this, SLOT(switchToLeftBrowser())) != Q_NULLPTR;
    connected &= connect(ui->rightBrowser, &BrowserWidget::gotFocus, [this]{ lastActiveBrowser = ui->rightBrowser;}) != Q_NULLPTR;
    connected &= connect(ui->rightBrowser, SIGNAL(search()), this, SLOT(handleSearch())) != Q_NULLPTR;
    connected &= connect(ui->rightBrowser, SIGNAL(copy()), this, SLOT(handleCopy())) != Q_NULLPTR;
    connected &= connect(ui->rightBrowser, SIGNAL(move()), this, SLOT(handleMove())) != Q_NULLPTR;
    connected &= connect(ui->rightBrowser, SIGNAL(del()), this, SLOT(handleDel())) != Q_NULLPTR;
    connected &= connect(ui->rightBrowser, SIGNAL(newFolder()), this, SLOT(handleNewFolder())) != Q_NULLPTR;

    connected &= connect(ui->leftDriveButton, SIGNAL(clicked()), ui->leftBrowser, SLOT(toggleDriveMenu())) != Q_NULLPTR;
    connected &= connect(ui->rightDriveButton, SIGNAL(clicked()), ui->rightBrowser, SLOT(toggleDriveMenu())) != Q_NULLPTR;

    connected &= connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(handleSearch())) != Q_NULLPTR;
    connected &= connect(ui->copyButton, SIGNAL(clicked()), this, SLOT(handleCopy())) != Q_NULLPTR;
    connected &= connect(ui->moveButton, SIGNAL(clicked()), this, SLOT(handleMove())) != Q_NULLPTR;
    connected &= connect(ui->deleteButton, SIGNAL(clicked()), this, SLOT(handleDel())) != Q_NULLPTR;
    connected &= connect(ui->newFolderButton, SIGNAL(clicked()), this, SLOT(handleNewFolder())) != Q_NULLPTR;

    connected &= connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close())) != Q_NULLPTR;
    connected &= connect(ui->actionDark_Theme, &QAction::triggered, [this]{
        dynamic_cast<QApplication*>(QApplication::instance())->setStyleSheet(darkTheme);
        ui->actionDark_Theme->setChecked(true);
        ui->actionOS_Theme->setChecked(false);
    }) != Q_NULLPTR;
    connected &= connect(ui->actionOS_Theme, &QAction::triggered, [this]{
        dynamic_cast<QApplication*>(QApplication::instance())->setStyleSheet("");
        ui->actionDark_Theme->setChecked(false);
        ui->actionOS_Theme->setChecked(true);
    }) != Q_NULLPTR;

    Q_ASSERT(connected);
}

void MainWindow::switchToLeftBrowser()
{
    ui->leftBrowser->setFocus();
}

void MainWindow::switchToRightBrowser()
{
    ui->rightBrowser->setFocus();
}

void MainWindow::handleSearch()
{
    findFiles->setDirectory(lastActiveBrowser->getRootPath());
    findFiles->show();
}

void MainWindow::handleCopy()
{
    QStringList filePaths;
    QString rootFolder;
    QString destination;
    if (lastActiveBrowser == ui->leftBrowser)
    {
        filePaths = ui->leftBrowser->getSelected();
        rootFolder = ui->leftBrowser->getRootPath();
        destination = ui->rightBrowser->getRootPath();
    }
    if (lastActiveBrowser == ui->rightBrowser)
    {
        filePaths = ui->rightBrowser->getSelected();
        rootFolder = ui->rightBrowser->getRootPath();
        destination = ui->leftBrowser->getRootPath();
    }

    QMessageBox* confirmMessage = new QMessageBox(this);
    confirmMessage->setText("Copy file(s)...");

    QString informativeString;
    for (auto filePath : filePaths)
    {
        QFileInfo info(filePath);
        QString name = info.fileName();
        informativeString += ("    " + name + "\n");
    }
    informativeString += "\nto\n\n";
    informativeString += ("    " + destination + "\n");

    confirmMessage->setInformativeText(informativeString);
    confirmMessage->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    confirmMessage->setDefaultButton(QMessageBox::Ok);
    confirmMessage->setIcon(QMessageBox::Question);

    if (confirmMessage->exec() == QMessageBox::Cancel)
    {
        return;
    }

    if (filePaths.count())
    {
        FileOperation* copyOperation = new FileOperation(FileOperation::eCopy, rootFolder, filePaths, destination, this);

        QProgressDialog* dialog = new QProgressDialog("Copy files...", "Cancel", 0, 100, this);
        dialog->setWindowModality(Qt::NonModal);

        connect(copyOperation, SIGNAL(setProgress(int)), dialog, SLOT(setValue(int)));
        connect(copyOperation, SIGNAL(finished()), copyOperation, SLOT(deleteLater()));
        connect(copyOperation, SIGNAL(finished()), dialog, SLOT(close()));
        connect(dialog, SIGNAL(canceled()), copyOperation, SLOT(cancel()));

        copyOperation->start();
        dialog->show();
    }
}

void MainWindow::handleMove()
{
    QStringList filePaths;
    QString rootFolder;
    QString destination;
    if (lastActiveBrowser == ui->leftBrowser)
    {
        filePaths = ui->leftBrowser->getSelected();
        rootFolder = ui->leftBrowser->getRootPath();
        destination = ui->rightBrowser->getRootPath();
    }
    if (lastActiveBrowser == ui->rightBrowser)
    {
        filePaths = ui->rightBrowser->getSelected();
        rootFolder = ui->rightBrowser->getRootPath();
        destination = ui->leftBrowser->getRootPath();
    }

    QMessageBox* confirmMessage = new QMessageBox(this);
    confirmMessage->setText("Move file(s)...");

    QString informativeString;
    for (auto filePath : filePaths)
    {
        QFileInfo info(filePath);
        QString name = info.fileName();
        informativeString += ("    " + name + "\n");
    }
    informativeString += "\nto\n\n";
    informativeString += ("    " + destination + "\n");

    confirmMessage->setInformativeText(informativeString);
    confirmMessage->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    confirmMessage->setDefaultButton(QMessageBox::Ok);
    confirmMessage->setIcon(QMessageBox::Question);

    if (confirmMessage->exec() == QMessageBox::Cancel)
    {
        return;
    }

    if (filePaths.count())
    {
        FileOperation* moveOperation = new FileOperation(FileOperation::eMove, rootFolder, filePaths, destination, this);

        QProgressDialog* dialog = new QProgressDialog("Move files...", "Cancel", 0, 100, this);
        dialog->setWindowModality(Qt::NonModal);

        connect(moveOperation, SIGNAL(setProgress(int)), dialog, SLOT(setValue(int)));
        connect(moveOperation, SIGNAL(finished()), moveOperation, SLOT(deleteLater()));
        connect(moveOperation, SIGNAL(finished()), dialog, SLOT(close()));
        connect(dialog, SIGNAL(canceled()), moveOperation, SLOT(cancel()));

        moveOperation->start();
        dialog->show();
    }
}

void MainWindow::handleDel()
{
    QStringList filePaths;
    QString rootFolder;

    if (lastActiveBrowser == ui->leftBrowser)
    {
        filePaths = ui->leftBrowser->getSelected();
        rootFolder = ui->leftBrowser->getRootPath();
    }
    if (lastActiveBrowser == ui->rightBrowser)
    {
        filePaths = ui->rightBrowser->getSelected();
        rootFolder = ui->rightBrowser->getRootPath();
    }

    QMessageBox* confirmMessage = new QMessageBox(this);
    confirmMessage->setText("Delete file(s)...");

    QString informativeString;
    for (auto filePath : filePaths)
    {
        QFileInfo info(filePath);
        QString name = info.fileName();
        informativeString += ("    " + name + "\n");
    }
    informativeString += "\nfrom\n\n";
    informativeString += ("    " + rootFolder + "\n");

    confirmMessage->setInformativeText(informativeString);
    confirmMessage->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    confirmMessage->setDefaultButton(QMessageBox::Ok);
    confirmMessage->setIcon(QMessageBox::Question);

    if (confirmMessage->exec() == QMessageBox::Cancel)
    {
        return;
    }

    if (filePaths.count())
    {
        FileOperation* delOperation = new FileOperation(FileOperation::eDelete, rootFolder, filePaths, QString(), this);
        QProgressDialog* dialog = new QProgressDialog("Deleting files...", "Cancel", 0, 100, this);
        dialog->setWindowModality(Qt::NonModal);

        connect(delOperation, SIGNAL(setProgress(int)), dialog, SLOT(setValue(int)));
        connect(delOperation, SIGNAL(finished()), delOperation, SLOT(deleteLater()));
        connect(delOperation, SIGNAL(finished()), dialog, SLOT(close()));
        connect(dialog, SIGNAL(canceled()), delOperation, SLOT(cancel()));

        delOperation->start();
        dialog->show();

    }
}

void MainWindow::handleNewFolder()
{
    QString rootFolder;
    if (lastActiveBrowser == ui->leftBrowser)
    {
        rootFolder = ui->leftBrowser->getRootPath();
    }
    else
    {
        rootFolder = ui->rightBrowser->getRootPath();
    }

    bool ok;
    QString folderName = QInputDialog::getText(this, "New Folder Name", "Insert new folder name:",
                                               QLineEdit::Normal, QString(), &ok);
    if (ok && !folderName.isEmpty())
    {
        QDir currentFolder(rootFolder);
        currentFolder.mkdir(folderName);
    }
}
