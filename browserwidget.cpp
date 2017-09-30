#include "browserwidget.h"
#include "ui_browserWidget.h"

#include <QDebug>
#include <QDir>
#include <QDirModel>
#include <QStorageInfo>

BrowserWidget::BrowserWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BrowserWidget)
{
    ui->setupUi(this);

    fileSystemModel = new FileSystemModel(this);
    driveTimer = new QTimer(this);
    completer = new QCompleter(this);
    dirModel = new QDirModel(completer);
    dirModel->setFilter(QDir::Dirs | QDir::Drives | QDir::NoDotAndDotDot);
    completer->setModel(dirModel);
    ui->currentPath->setCompleter(completer);

    CustomizeUI();
    Connect();

    driveTimer->setInterval(1000);
    driveTimer->start(1000);
}

BrowserWidget::~BrowserWidget()
{
    delete ui;
}

QStringList BrowserWidget::getSelected()
{
    QStringList filePaths;
    for (auto index : ui->fileSystemView->selectionModel()->selection().indexes())
    {
        if (index.column() == 0)
        {
            filePaths.push_back(fileSystemModel->fileInfo(index).absoluteFilePath());
        }
    }
    return filePaths;
}

QString BrowserWidget::getRootPath()
{
    return fileSystemModel->rootPath();
}

void BrowserWidget::CustomizeUI()
{
    populateDriveList();

    QString osType = QSysInfo::productType();

    ui->homeButton->setIcon(iconProvider.icon(QFileIconProvider::Computer));
    fileSystemModel->setFilter(QDir::AllDirs | QDir::NoDot | QDir::Dirs | QDir::Files);
    ui->fileSystemView->setModel(fileSystemModel);

    if (osType == "osx" /* || some linux*/)
    {
        QString homePath = QDir::homePath();
        ui->currentPath->setText(homePath);
        ui->fileSystemView->setRootIndex(fileSystemModel->setRootPath(QDir::homePath()));
    }
    else //windows
    {
        ui->currentPath->setText(ui->driveList->itemText(0));
        ui->fileSystemView->setRootIndex(fileSystemModel->setRootPath(ui->driveList->itemText(0)));
    }

    ui->fileSystemView->verticalHeader()->hide();

    ui->fileSystemView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->fileSystemView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->fileSystemView->setDragEnabled(true);
    ui->fileSystemView->setAcceptDrops(true);
    ui->fileSystemView->setDropIndicatorShown(true);
    ui->fileSystemView->setDragDropMode(QAbstractItemView::DragDrop);

    ui->fileSystemView->horizontalHeader()->setStretchLastSection(true);
    ui->fileSystemView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->fileSystemView->setAlternatingRowColors(true);
    ui->fileSystemView->setShowGrid(false);

    setFocusProxy(ui->fileSystemView);
}

void BrowserWidget::Connect()
{
    bool connected = true;
    connected &= connect(
        ui->fileSystemView,
        SIGNAL(doubleClicked(QModelIndex)),
        this,
        SLOT(enterFolder(QModelIndex))) != Q_NULLPTR;

    connected &= connect(
        fileSystemModel,
        SIGNAL(rootPathChanged(QString)),
        this,
        SLOT(handleRootPathChanged(QString))) != Q_NULLPTR;

    connected &= connect(
        ui->fileSystemView,
        SIGNAL(switchMe()),
        this,
        SLOT(handleSwitchMeRequest())) != Q_NULLPTR;

    connected &= connect(
        ui->fileSystemView,
        SIGNAL(gotFocus()),
        this,
        SLOT(handleGotFocus())) != Q_NULLPTR;

    connected &= connect(
        ui->fileSystemView,
        SIGNAL(goToParent()),
        this,
        SLOT(goToParent())) != Q_NULLPTR;

    connected &= connect(
        ui->fileSystemView,
        SIGNAL(copy()),
        this,
        SIGNAL(copy())) != Q_NULLPTR;

    connected &= connect(
        ui->fileSystemView,
        SIGNAL(move()),
        this,
        SIGNAL(move())) != Q_NULLPTR;

    connected &= connect(
        ui->fileSystemView,
        SIGNAL(del()),
        this,
        SIGNAL(del())) != Q_NULLPTR;

    connected &= connect(
        ui->fileSystemView,
        SIGNAL(newFolder()),
        this,
        SIGNAL(newFolder())) != Q_NULLPTR;

    connected &= connect(
        ui->homeButton,
        SIGNAL(clicked()),
        this,
        SLOT(setHome())) != Q_NULLPTR;

    connected &= connect(
        driveTimer,
        SIGNAL(timeout()),
        this,
        SLOT(populateDriveList())) != Q_NULLPTR;

    connected &= connect(
        ui->driveList,
        SIGNAL(currentIndexChanged(QString)),
        this,
        SLOT(setPath(QString))) != Q_NULLPTR;

    connected &= connect(
        ui->currentPath,
        SIGNAL(textChanged(QString)),
        this,
        SLOT(setPath(QString))) != Q_NULLPTR;

    Q_ASSERT(connected);
}

void BrowserWidget::SelectFirstRow(bool directoryChanged)
{
    auto indexes = ui->fileSystemView->selectionModel()->selection().indexes();
    if (indexes.count() == 0 || directoryChanged)
    {
        ui->fileSystemView->clearSelection();
        ui->fileSystemView->selectRow(0);
    }
}

void BrowserWidget::toggleDriveMenu()
{
    ui->driveList->showPopup();
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
    ui->currentPath->setText(newPath);
    SelectFirstRow(true);
}

void BrowserWidget::handleSwitchMeRequest()
{
    emit switchMe();
}

void BrowserWidget::handleGotFocus()
{
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

void BrowserWidget::setHome()
{
    setPath("");
}

void BrowserWidget::checkPathNotAvailable()
{
    if (!QDir(fileSystemModel->rootPath()).exists())
    {
        ui->fileSystemView->setRootIndex(fileSystemModel->setRootPath(QDir::homePath()));
    }
}

void BrowserWidget::populateDriveList()
{
    auto storageInfoList = QStorageInfo::mountedVolumes();

    if (storageInfoList.count() != ui->driveList->count())
    {
        ui->driveList->clear();

        for (auto storageInfo : storageInfoList)
        {
            auto storagePath = storageInfo.rootPath();
            auto icon = iconProvider.icon(QFileInfo(storageInfo.rootPath()));

            ui->driveList->addItem(icon, storagePath);
        }

        checkPathNotAvailable();
    }
}
