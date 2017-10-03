#include "browserwidget.h"
#include "ui_browserWidget.h"

#include <QDebug>
#include <QDir>
#include <QDirModel>
#include <QStorageInfo>
#include <QDesktopServices>
#include <QUrl>
#include <QAbstractItemView>
#include <QHeaderView>
#include <QMenu>

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

QStringList BrowserWidget::getSelected() const
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

QString BrowserWidget::getRootPath() const
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

    if (osType == "osx" || osType == "fedora")
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
    ui->fileSystemView->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->fileSystemView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->fileSystemView->setSelectionMode(QAbstractItemView::ExtendedSelection);
    ui->fileSystemView->setDragEnabled(true);
    ui->fileSystemView->setAcceptDrops(true);
    ui->fileSystemView->setDropIndicatorShown(true);
    ui->fileSystemView->setDragDropMode(QAbstractItemView::DragDrop);
    ui->fileSystemView->setSortingEnabled(true);
    ui->fileSystemView->sortByColumn(0, Qt::AscendingOrder);

    ui->fileSystemView->horizontalHeader()->setStretchLastSection(true);
    ui->fileSystemView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->fileSystemView->setAlternatingRowColors(true);
    ui->fileSystemView->setShowGrid(false);

    ui->showHiddenFilesButton->setChecked(false);

    setFocusProxy(ui->fileSystemView);
}

void BrowserWidget::Connect()
{
    bool connected = true;

    connected &= connect(ui->fileSystemView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(open(QModelIndex))) != Q_NULLPTR;
    connected &= connect(fileSystemModel, SIGNAL(directoryLoaded(QString)), this, SLOT(handleRootPathChanged(QString))) != Q_NULLPTR;
    connected &= connect(fileSystemModel, SIGNAL(directoryLoaded(QString)), this, SLOT(matchDriveToPath(QString))) != Q_NULLPTR;
    connected &= connect(ui->fileSystemView, SIGNAL(switchMe()), this, SLOT(handleSwitchMeRequest())) != Q_NULLPTR;
    connected &= connect(ui->fileSystemView, SIGNAL(gotFocus()), this, SLOT(handleGotFocus())) != Q_NULLPTR;
    connected &= connect(ui->fileSystemView, SIGNAL(goToParent()), this, SLOT(goToParent())) != Q_NULLPTR;
    connected &= connect(ui->fileSystemView, SIGNAL(copy()), this, SIGNAL(copy())) != Q_NULLPTR;
    connected &= connect(ui->fileSystemView, SIGNAL(move()), this, SIGNAL(move())) != Q_NULLPTR;
    connected &= connect(ui->fileSystemView, SIGNAL(del()), this, SIGNAL(del())) != Q_NULLPTR;
    connected &= connect(ui->fileSystemView, SIGNAL(newFolder()), this, SIGNAL(newFolder())) != Q_NULLPTR;
    connected &= connect(ui->homeButton, SIGNAL(clicked()), this, SLOT(setHome())) != Q_NULLPTR;
    connected &= connect(driveTimer, SIGNAL(timeout()), this, SLOT(populateDriveList())) != Q_NULLPTR;
    connected &= connect(ui->driveList, SIGNAL(currentIndexChanged(QString)), this, SLOT(setPath(QString))) != Q_NULLPTR;
    connected &= connect(ui->currentPath, SIGNAL(textChanged(QString)), this, SLOT(setPath(QString))) != Q_NULLPTR;
    connected &= connect(ui->fileSystemView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint))) != Q_NULLPTR;
    connected &= connect(ui->showHiddenFilesButton, SIGNAL(toggled(bool)), this, SLOT(showHiddenFiles(bool))) != Q_NULLPTR;

    Q_ASSERT(connected);
}

void BrowserWidget::SelectFirstRow(bool directoryChanged)
{
    if (directoryChanged ||
        ui->fileSystemView->selectionModel()->selection().indexes().count() == 0)
    {
        ui->fileSystemView->clearSelection();
        ui->fileSystemView->selectRow(0);
    }
}

void BrowserWidget::toggleDriveMenu()
{
    ui->driveList->showPopup();
}

void BrowserWidget::open(QModelIndex index)
{
    auto fileInfo = fileSystemModel->fileInfo(index);
    if (fileInfo.isDir())
    {
        if (fileInfo.absoluteFilePath() != "/..")
        {
            ui->fileSystemView->setRootIndex(fileSystemModel->setRootPath(fileInfo.absoluteFilePath()));
        }
        else
        {
            ui->fileSystemView->setRootIndex(fileSystemModel->setRootPath(""));
        }
    }
    else
    {
        QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()));
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

void BrowserWidget::customContextMenuRequested(QPoint position)
{
    auto index = ui->fileSystemView->indexAt(position);
    auto path = fileSystemModel->fileInfo(index).absoluteFilePath();

    if (!index.isValid())
    {
        path = fileSystemModel->rootPath();
    }

    QMenu* menu = new QMenu(this);
    menu->addAction(new QAction(path, this));
    menu->popup(ui->fileSystemView->viewport()->mapToGlobal(position));
}

void BrowserWidget::showHiddenFiles(bool show)
{
    QDir::Filters fileSystemFilter = fileSystemModel->filter();
    QDir::Filters dirFilter = dirModel->filter();

    if (show)
    {
        fileSystemFilter |= (QDir::Hidden | QDir::System);
        dirFilter |= (QDir::Hidden | QDir::System);
    }
    else
    {
        fileSystemFilter &= (~(QDir::Hidden | QDir::System));
        dirFilter &= (~(QDir::Hidden | QDir::System));
    }

    fileSystemModel->setFilter(fileSystemFilter);
    dirModel->setFilter(dirFilter);
}

void BrowserWidget::matchDriveToPath(QString currentPath)
{
    if (currentPath.startsWith(ui->driveList->currentText()))
    {
        return;
    }

    for (auto index = 0; index < ui->driveList->count(); index++)
    {
        if (currentPath.startsWith(ui->driveList->itemText(index)))
        {
            ui->driveList->setCurrentIndex(index);
            return;
        }
    }
}
