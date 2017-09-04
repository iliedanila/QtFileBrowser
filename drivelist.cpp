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
    signalMapper = new QSignalMapper(this);

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
            disconnect(button, 0, 0, 0);
            ui->horizontalLayout->removeWidget(button);
            buttonList.pop_front();

            signalMapper->removeMappings(button);
            emit pathNotAvailable(button->text());
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

            connect(button, SIGNAL(released()), signalMapper, SLOT(map()));
            signalMapper->setMapping(button, buttonText);

            ui->horizontalLayout->addWidget(button);
        }

        connect(signalMapper, SIGNAL(mapped(QString)), this, SLOT(buttonClicked(QString)));
    }
}

void DriveList::handleSetHome()
{
    emit setPath("");
}

void DriveList::onTimer()
{
    CreateButtonsForDrives();
}

void DriveList::buttonClicked(QString path)
{
    emit setPath(path);
}
