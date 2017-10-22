#include "findfilesdialog.h"
#include "ui_findfilesdialog.h"

#include <QDesktopWidget>

FindFilesDialog::FindFilesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindFilesDialog)
{
    ui->setupUi(this);

    CustomizeUI();
    Connect();
}

FindFilesDialog::~FindFilesDialog()
{
    delete ui;
}

void FindFilesDialog::CustomizeUI()
{
    setWindowTitle(tr("Find Files"));
    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    resize(screenGeometry.width() / 2, screenGeometry.height() / 2);
}

void FindFilesDialog::Connect()
{

}
