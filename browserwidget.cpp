#include "browserwidget.h"
#include "ui_browserWidget.h"

#include <QDebug>
#include <QDir>

BrowserWidget::BrowserWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BrowserWidget)
{
    ui->setupUi(this);
    fileSystemModel = new FileSystemModel(this);

    CustomizeUI();
    Connect();
}

BrowserWidget::~BrowserWidget()
{
    delete ui;
}

void BrowserWidget::CustomizeUI()
{
    fileSystemModel->setFilter(QDir::AllDirs | QDir::NoDot | QDir::Dirs | QDir::Files);

    ui->fileSystemView->setModel(fileSystemModel);
    ui->fileSystemView->setRootIndex(fileSystemModel->setRootPath(QDir::homePath()));
    ui->fileSystemView->verticalHeader()->hide();

    ui->fileSystemView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->fileSystemView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->fileSystemView->setDragEnabled(true);
    ui->fileSystemView->setAcceptDrops(true);
    ui->fileSystemView->setDropIndicatorShown(true);
    ui->fileSystemView->setDragDropMode(QAbstractItemView::DragDrop);

    ui->fileSystemView->horizontalHeader()->setStretchLastSection(true);
    ui->fileSystemView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->fileSystemView->setShowGrid(true);

    setFocusProxy(ui->fileSystemView);
}

void BrowserWidget::Connect()
{
    connect(ui->fileSystemView,
            SIGNAL(doubleClicked(QModelIndex)),
            this,
            SLOT(enterFolder(QModelIndex)));

    connect(fileSystemModel,
            SIGNAL(rootPathChanged(QString)),
            this,
            SLOT(handleRootPathChanged(QString)));

    connect(ui->fileSystemView,
            SIGNAL(switchMe()),
            this,
            SLOT(handleSwitchMeRequest()));

    connect(ui->fileSystemView,
            SIGNAL(gotFocus()),
            this,
            SLOT(handleGotFocus()));

    connect(ui->fileSystemView,
            SIGNAL(goToParent()),
            this,
            SLOT(goToParent()));

    connect(ui->driveList,
            SIGNAL(setPath(QString)),
            this,
            SLOT(setPath(QString)));
    connect(ui->driveList,
            SIGNAL(pathNotAvailable(QString)),
            this,
            SLOT(pathNotAvailable(QString)));
}

void BrowserWidget::SelectFirstRow(bool directoryChanged)
{
    if (ui->fileSystemView->selectionModel()->selectedRows().count() == 0 ||
        directoryChanged)
    {
        ui->fileSystemView->selectRow(0);
    }
}

void BrowserWidget::enterFolder(QModelIndex index)
{
    auto fileInfo = fileSystemModel->fileInfo(index);
    if (fileInfo.absoluteFilePath() != "/..")
    {
        ui->fileSystemView->setRootIndex(fileSystemModel->setRootPath(fileInfo.absoluteFilePath()));
    }
    else
    {
        ui->fileSystemView->setRootIndex(fileSystemModel->setRootPath(""));
    }
}

void BrowserWidget::handleRootPathChanged(QString newPath)
{
    emit rootPathChanged(newPath);
    SelectFirstRow(true);
}

void BrowserWidget::handleSwitchMeRequest()
{
    emit switchMe();
}

void BrowserWidget::handleGotFocus()
{
    emit rootPathChanged(fileSystemModel->rootPath());
    SelectFirstRow(false);
}

void BrowserWidget::goToParent()
{
    QDir currentDir(fileSystemModel->rootPath());
    if (currentDir.cdUp())
    {
        ui->fileSystemView->setRootIndex(fileSystemModel->setRootPath(currentDir.path()));
    }
}

void BrowserWidget::setPath(QString path)
{
    ui->fileSystemView->setRootIndex(fileSystemModel->setRootPath(path));
}

void BrowserWidget::pathNotAvailable(QString /*path*/)
{
    if (!QDir(fileSystemModel->rootPath()).exists())
    {
        ui->fileSystemView->setRootIndex(fileSystemModel->setRootPath(QDir::homePath()));
    }
}
