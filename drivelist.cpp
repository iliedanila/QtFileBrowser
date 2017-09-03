#include "drivelist.h"
#include "ui_drivelist.h"

#include <QFileIconProvider>
#include <QStorageInfo>
#include <QSpacerItem>

DriveList::DriveList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DriveList)
{
    ui->setupUi(this);

    CustomizeUI();
    Connect();
}

DriveList::~DriveList()
{
    delete ui;
}

void DriveList::CustomizeUI()
{
    ui->homeButton->setIcon(iconProvider.icon(QFileIconProvider::Computer));

    auto storageInfoList = QStorageInfo::mountedVolumes();
    for (auto& storageInfo : storageInfoList)
    {
        auto buttonText = storageInfo.rootPath() + " " + storageInfo.displayName();
        QFileInfo fileInfo(storageInfo.rootPath());
        auto icon = iconProvider.icon(fileInfo);

        auto button = new QToolButton(this);
        button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        button->setText(buttonText);
        button->setIcon(icon);
        buttonList.push_back(button);

        ui->horizontalLayout->addWidget(button);
    }
}

void DriveList::Connect()
{
    connect(ui->homeButton,
            SIGNAL(released()),
            this,
            SLOT(handleSetHome()));
}

void DriveList::handleSetHome()
{
    emit setHome();
}
