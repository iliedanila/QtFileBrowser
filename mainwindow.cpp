#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QProgressDialog>
#include <QProcess>

MainWindow::MainWindow(QWidget *parent)
    :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    CustomizeUI();
    Connect();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::CustomizeUI()
{
}

void MainWindow::Connect()
{
    bool connected = true;
    connected &= connect(ui->leftBrowser,
            SIGNAL(switchMe()),
            this,
            SLOT(switchToRightBrowser())) != Q_NULLPTR;

    connected &= connect(ui->rightBrowser,
            SIGNAL(switchMe()),
            this,
            SLOT(switchToLeftBrowser())) != Q_NULLPTR;

    connected &= connect(ui->copyButton,
            SIGNAL(pressed()),
            this,
            SLOT(handleCopy())) != Q_NULLPTR;

    connected &= connect(ui->leftBrowser,
                         SIGNAL(copy()),
                         this,
                         SLOT(handleCopy())) != Q_NULLPTR;

    connected &= connect(ui->rightBrowser,
                         SIGNAL(copy()),
                         this,
                         SLOT(handleCopy())) != Q_NULLPTR;

    connected &= connect(ui->deleteButton,
                         SIGNAL(pressed()),
                         this,
                         SLOT(handleDel())) != Q_NULLPTR;

    connected &= connect(ui->leftBrowser,
                         SIGNAL(del()),
                         this,
                         SLOT(handleDel())) != Q_NULLPTR;

    connected &= connect(ui->rightBrowser,
                         SIGNAL(del()),
                         this,
                         SLOT(handleDel())) != Q_NULLPTR;

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

void MainWindow::handleCopy()
{
    QStringList filePaths;
    QString rootFolder;
    QString destination;
    if (ui->leftBrowser->hasFocus())
    {
        filePaths = ui->leftBrowser->getSelected();
        rootFolder = ui->leftBrowser->getRootPath();
        destination = ui->rightBrowser->getRootPath();
    }
    if (ui->rightBrowser->hasFocus())
    {
        filePaths = ui->rightBrowser->getSelected();
        rootFolder = ui->rightBrowser->getRootPath();
        destination = ui->leftBrowser->getRootPath();
    }

    if (filePaths.count())
    {
        FileOperation* copyOperation = new FileOperation(FileOperation::eCopy, rootFolder, filePaths, destination, this);

        QProgressDialog* dialog = new QProgressDialog("Copy files...", "Cancel", 0, 100, this);
        dialog->setWindowModality(Qt::NonModal);

        connect(copyOperation,
                SIGNAL(setProgress(int)),
                dialog,
                SLOT(setValue(int)));
        connect(copyOperation, SIGNAL(finished()), copyOperation, SLOT(deleteLater()));
        connect(copyOperation, SIGNAL(finished()), dialog, SLOT(close()));
        connect(dialog, SIGNAL(canceled()), copyOperation, SLOT(cancel()));

        copyOperation->start();
        dialog->show();
    }
}

void MainWindow::handleDel()
{
    QStringList filePaths;
    QString rootFolder;

    if (ui->leftBrowser->hasFocus())
    {
        filePaths = ui->leftBrowser->getSelected();
        rootFolder = ui->leftBrowser->getRootPath();
    }
    if (ui->rightBrowser->hasFocus())
    {
        filePaths = ui->rightBrowser->getSelected();
        rootFolder = ui->rightBrowser->getRootPath();
    }

    if (filePaths.count())
    {
        FileOperation* delOperation = new FileOperation(FileOperation::eDelete, rootFolder, filePaths, QString(), this);
        QProgressDialog* dialog = new QProgressDialog("Deleting files...", "Cancel", 0, 100, this);
        dialog->setWindowModality(Qt::NonModal);

        connect(delOperation,
                SIGNAL(setProgress(int)),
                dialog,
                SLOT(setValue(int)));
        connect(delOperation, SIGNAL(finished()), delOperation, SLOT(deleteLater()));
        connect(delOperation, SIGNAL(finished()), dialog, SLOT(close()));
        connect(dialog, SIGNAL(canceled()), delOperation, SLOT(cancel()));

        delOperation->start();
        dialog->show();

    }
}
