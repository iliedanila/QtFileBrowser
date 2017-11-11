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

    ui->openButton->setEnabled(false);
    findFiles->setWindowModality(Qt::WindowModal);
    ui->leftBrowser->setFocus();
}

void MainWindow::Connect()
{
    connect(ui->leftBrowser, &BrowserWidget::switchMe, this, &MainWindow::switchToRightBrowser);
    connect(ui->leftBrowser, &BrowserWidget::gotFocus, [this]{ lastActiveBrowser = ui->leftBrowser;});
    connect(ui->leftBrowser, &BrowserWidget::search, this, &MainWindow::handleSearch);
    connect(ui->leftBrowser, &BrowserWidget::copy, this, &MainWindow::handleCopy);
    connect(ui->leftBrowser, &BrowserWidget::move, this, &MainWindow::handleMove);
    connect(ui->leftBrowser, &BrowserWidget::del, this, &MainWindow::handleDel);
    connect(ui->leftBrowser, &BrowserWidget::newFolder, this, &MainWindow::handleNewFolder);

    connect(ui->rightBrowser, &BrowserWidget::switchMe, this, &MainWindow::switchToLeftBrowser);
    connect(ui->rightBrowser, &BrowserWidget::gotFocus, [this]{ lastActiveBrowser = ui->rightBrowser;});
    connect(ui->rightBrowser, &BrowserWidget::search, this, &MainWindow::handleSearch);
    connect(ui->rightBrowser, &BrowserWidget::copy, this, &MainWindow::handleCopy);
    connect(ui->rightBrowser, &BrowserWidget::move, this, &MainWindow::handleMove);
    connect(ui->rightBrowser, &BrowserWidget::del, this, &MainWindow::handleDel);
    connect(ui->rightBrowser, &BrowserWidget::newFolder, this, &MainWindow::handleNewFolder);

    connect(ui->leftDriveButton, &QPushButton::clicked, ui->leftBrowser, &BrowserWidget::toggleDriveMenu);
    connect(ui->rightDriveButton, &QPushButton::clicked, ui->rightBrowser, &BrowserWidget::toggleDriveMenu);

    connect(ui->searchButton, &QPushButton::clicked, this, &MainWindow::handleSearch);
    connect(ui->copyButton, &QPushButton::clicked, this, &MainWindow::handleCopy);
    connect(ui->moveButton, &QPushButton::clicked, this, &MainWindow::handleMove);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::handleDel);
    connect(ui->newFolderButton, &QPushButton::clicked, this, &MainWindow::handleNewFolder);

    connect(ui->actionQuit, &QAction::triggered, this, &MainWindow::close);
    connect(ui->actionDark_Theme, &QAction::triggered, [this]
    {
        dynamic_cast<QApplication*>(QApplication::instance())->setStyleSheet(darkTheme);
        ui->actionDark_Theme->setChecked(true);
        ui->actionOS_Theme->setChecked(false);
    });
    connect(ui->actionOS_Theme, &QAction::triggered, [this]
    {
        dynamic_cast<QApplication*>(QApplication::instance())->setStyleSheet("");
        ui->actionDark_Theme->setChecked(false);
        ui->actionOS_Theme->setChecked(true);
    });
    connect(findFiles, &FindFilesDialog::rejected, findFiles, &FindFilesDialog::cancel);
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
    findFiles->setFocus(Qt::OtherFocusReason);
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

        connect(copyOperation, &FileOperation::setProgress, dialog, &QProgressDialog::setValue);
        connect(copyOperation, &FileOperation::finished, copyOperation, &FileOperation::deleteLater);
        connect(copyOperation, &FileOperation::finished, dialog, &QProgressDialog::close);
        connect(dialog, &QProgressDialog::canceled, copyOperation, &FileOperation::cancel);

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

        connect(moveOperation, &FileOperation::setProgress, dialog, &QProgressDialog::setValue);
        connect(moveOperation, &FileOperation::finished, moveOperation, &FileOperation::deleteLater);
        connect(moveOperation, &FileOperation::finished, dialog, &QProgressDialog::close);
        connect(dialog, &QProgressDialog::canceled, moveOperation, &FileOperation::cancel);

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

        connect(delOperation, &FileOperation::setProgress, dialog, &QProgressDialog::setValue);
        connect(delOperation, &FileOperation::finished, delOperation, &FileOperation::deleteLater);
        connect(delOperation, &FileOperation::finished, dialog, &QProgressDialog::close);
        connect(dialog, &QProgressDialog::canceled, delOperation, &FileOperation::cancel);

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
