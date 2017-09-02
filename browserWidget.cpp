#include "browserWidget.h"
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
    ui->fileSystemView->horizontalHeader()->setStretchLastSection(true);
    ui->fileSystemView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->fileSystemView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->fileSystemView->setSelectionMode(QAbstractItemView::ExtendedSelection);

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
}

void BrowserWidget::handleSwitchMeRequest()
{
    emit switchMe();
}
