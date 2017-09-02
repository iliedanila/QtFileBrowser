#include "browserWidget.h"
#include "ui_browserWidget.h"

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
    ui->fileSystemView->setRootIndex(fileSystemModel->setRootPath(""));
    ui->fileSystemView->verticalHeader()->hide();
    ui->fileSystemView->horizontalHeader()->setStretchLastSection(true);
    ui->fileSystemView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->fileSystemView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->fileSystemView->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

void BrowserWidget::Connect()
{
    connect(ui->fileSystemView,
            SIGNAL(doubleClicked(QModelIndex)),
            this,
            SLOT(enterFolder(QModelIndex)));
}

void BrowserWidget::enterFolder(QModelIndex index)
{
    auto fileInfo = fileSystemModel->fileInfo(index);
    ui->fileSystemView->setRootIndex(fileSystemModel->setRootPath(fileInfo.absoluteFilePath()));

    // Set selected item ".." from the new view.
    auto newIndex = fileSystemModel->index(0, 0);
    ui->fileSystemView->selectionModel()->select(newIndex, QItemSelectionModel::Select);
}
