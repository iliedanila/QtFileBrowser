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

    timer = new QTimer(this);
    CustomizeUI();
    Connect();

    timer->setInterval(1000);
    timer->start(1000);
}

DriveList::~DriveList()
{
    delete ui;
}

void DriveList::CustomizeUI()
{
    ui->homeButton->setIcon(iconProvider.icon(QFileIconProvider::Computer));
    CreateButtonsForDrives();
}

void DriveList::Connect()
{
    connect(ui->homeButton,
            SIGNAL(released()),
            this,
            SLOT(handleSetHome()));

    connect(timer, SIGNAL(timeout()), this, SLOT(onTimer()));
}

void DriveList::CreateButtonsForDrives()
{
    auto storageInfoList = QStorageInfo::mountedVolumes();

    if (storageInfoList.count() != buttonList.count())
    {
        while(buttonList.count())
        {
            QToolButton* button = *buttonList.begin();
            ui->horizontalLayout->removeWidget(button);
            buttonList.pop_front();
            delete button;
        }

        for (auto& storageInfo : storageInfoList)
        {
            auto buttonText = storageInfo.rootPath();
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
}

void DriveList::handleSetHome()
{
    emit setHome();
}

void DriveList::onTimer()
{
    CreateButtonsForDrives();
}
