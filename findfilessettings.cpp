#include "findfilessettings.h"
#include "ui_findfilessettings.h"

FindFilesSettings::FindFilesSettings(QWidget *parent)
:
    QWizardPage(parent),
    ui(new Ui::FindFilesSettings)
{
    ui->setupUi(this);
}

FindFilesSettings::~FindFilesSettings()
{
    delete ui;
}
