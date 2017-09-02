#include "drivelist.h"
#include "ui_drivelist.h"

DriveList::DriveList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DriveList)
{
    ui->setupUi(this);
}

DriveList::~DriveList()
{
    delete ui;
}
