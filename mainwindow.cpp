#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "customfilesystemmodel.h"
#include "qdesktopservices.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    model = new CustomFileSystemModel(this);
    QString path = QStandardPaths::displayName(QStandardPaths::HomeLocation);
    model->setRootPath(path);
    model->setFilter(QDir::NoDot | QDir::AllDirs /*| QDir::Files*/);

    ui->folderView->setModel(model);
    ui->folderView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->folderView->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    ui->currentPath->setText("");

    connect(ui->folderView, SIGNAL(doubleClicked(const QModelIndex&)), model, SLOT(enterFolder(const QModelIndex&)));
    connect(model, SIGNAL(folderDoubleClicked(const QString&)), this, SLOT(setNewFolder(const QString&)));
    connect(ui->homeButton, SIGNAL(released()), this, SLOT(setHome()));
    connect(model, SIGNAL(directoryLoaded(const QString &)), ui->currentPath, SLOT(setText(const QString &)));
    connect(
        ui->folderView->selectionModel(),
        SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
        this,
        SLOT(handleSelectionChanged(QItemSelection,QItemSelection)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setNewFolder(const QString &newFolder)
{
    ui->folderView->setRootIndex(model->setRootPath(newFolder));
}

void MainWindow::setHome()
{
    ui->folderView->setRootIndex(model->setRootPath(""));
    ui->currentPath->setText(model->rootPath());
}

void MainWindow::handleSelectionChanged(QItemSelection selected, QItemSelection deselected)
{
    qDebug() << "handleSelectionChanged\n";
}
