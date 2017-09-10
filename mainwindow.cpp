#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QProgressDialog>

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
    ui->currentPath->setText(QDir::homePath());
}

void MainWindow::Connect()
{
    bool connected = true;
    connected &= connect(ui->leftBrowser,
            SIGNAL(rootPathChanged(QString)),
            this,
            SLOT(handleRootPathChanged(QString))) != Q_NULLPTR;

    connected &= connect(ui->rightBrowser,
            SIGNAL(rootPathChanged(QString)),
            this,
            SLOT(handleRootPathChanged(QString))) != Q_NULLPTR;

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

    Q_ASSERT(connected);
}

void MainWindow::handleRootPathChanged(QString newPath)
{
    ui->currentPath->setText(newPath);
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
    QString destination;
    if (ui->leftBrowser->hasFocus())
    {
        filePaths = ui->leftBrowser->getSelected();
        destination = ui->rightBrowser->getRootPath();
    }
    if (ui->rightBrowser->hasFocus())
    {
        filePaths = ui->rightBrowser->getSelected();
        destination = ui->leftBrowser->getRootPath();
    }

    if (filePaths.count())
    {
        FileOperation* copyOperation = new FileOperation(FileOperation::eCopy, filePaths, destination, this);

        QProgressDialog* dialog = new QProgressDialog("Copy files...", "Cancel", 0, 100, this);
        dialog->setWindowModality(Qt::WindowModal);

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

