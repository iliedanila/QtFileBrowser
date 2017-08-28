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

    folderModel = new CustomFileSystemModel(this);
    QString path = QStandardPaths::displayName(QStandardPaths::HomeLocation);
    folderModel->setRootPath(path);
    folderModel->setFilter(QDir::NoDot | QDir::AllDirs /*| QDir::Files*/);

    fileModel = new FileViewModel(this);

    ui->folderView->setModel(folderModel);
    ui->folderView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->folderView->setSelectionMode(QAbstractItemView::SelectionMode::ExtendedSelection);
    ui->currentPath->setText("");

    ui->filesView->setModel(fileModel);

    connect(ui->folderView, SIGNAL(doubleClicked(const QModelIndex&)), folderModel, SLOT(enterFolder(const QModelIndex&)));
    connect(folderModel, SIGNAL(folderDoubleClicked(const QString&)), this, SLOT(setNewFolder(const QString&)));
    connect(ui->homeButton, SIGNAL(released()), this, SLOT(setHome()));
    connect(folderModel, SIGNAL(directoryLoaded(const QString &)), ui->currentPath, SLOT(setText(const QString &)));
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
    ui->folderView->setRootIndex(folderModel->setRootPath(newFolder));
}

void MainWindow::setHome()
{
    ui->folderView->setRootIndex(folderModel->setRootPath(""));
    ui->currentPath->setText(folderModel->rootPath());
}

void MainWindow::handleSelectionChanged(QItemSelection selected, QItemSelection deselected)
{
    qDebug() << "handleSelectionChanged\n";

    QList<QString> added;
    QList<QString> removed;

    for (auto& index : selected.indexes())
    {
        if (index.column() == 0)
        {
            auto fileInfo = folderModel->fileInfo(index);
            fileModel->addFilesInFolder(fileInfo.absoluteFilePath());
        }
    }

    for (auto& index : deselected.indexes())
    {
        if (index.column() == 0)
        {
            auto fileInfo = folderModel->fileInfo(index);
            fileModel->removeFilesInFolder(fileInfo.absoluteFilePath());
        }
    }
}
