#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
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

    Q_ASSERT(connected);
}

void MainWindow::handleRootPathChanged(QString newPath)
{
    ui->currentPath->setText(newPath);
}

void MainWindow::switchToLeftBrowser()
{
    ui->leftBrowser->setFocus();
    ui->leftBrowser->activateWindow();
}

void MainWindow::switchToRightBrowser()
{
    ui->rightBrowser->setFocus();
    ui->rightBrowser->activateWindow();
}

