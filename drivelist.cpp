#include "drivelist.h"
#include "ui_drivelist.h"

#include <QFileIconProvider>

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
    QFileIconProvider iconProvider;
    ui->homeButton->setIcon(iconProvider.icon(QFileIconProvider::Computer));
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
