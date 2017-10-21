#include "findfilessettings.h"
#include "ui_findfilessettings.h"

#include <QLineEdit>
#include <QDirModel>

FindFilesSettings::FindFilesSettings(const QString& aRootFolder, QWidget *parent)
:
    QWizardPage(parent),
    ui(new Ui::FindFilesSettings),
    rootFolder(aRootFolder)
{
    ui->setupUi(this);
    CustomizeUI();
    Connect();
}

FindFilesSettings::~FindFilesSettings()
{
    delete ui;
}

void FindFilesSettings::CustomizeUI() const
{
    ui->findTextComboBox->setEnabled(false);
    ui->searchInComboBox->setEditText(/*QDir::toNativeSeparators(*/rootFolder/*)*/);
}

void FindFilesSettings::Connect()
{
    bool connected = true;

    connected &= connect(ui->findTextCheckBox, &QCheckBox::stateChanged, [this](int state)
    {
        ui->findTextComboBox->setEnabled(state == Qt::Checked);
    }) != Q_NULLPTR;

    Q_ASSERT(connected);
}
