#include "findfilesresults.h"
#include "ui_findfilesresults.h"

FindFilesResults::FindFilesResults(QWidget *parent)
:
    QWizardPage(parent),
    ui(new Ui::FindFilesResults)
{
    ui->setupUi(this);
}

FindFilesResults::~FindFilesResults()
{
    delete ui;
}
