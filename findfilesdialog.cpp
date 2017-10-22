#include "findfilesdialog.h"
#include "ui_findfilesdialog.h"

#include <QDesktopWidget>
#include <QFileDialog>

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

void FindFilesDialog::setDirectory(const QString &aDirectory)
{
    ui->searchInComboBox->setEditText(aDirectory);
}

void FindFilesDialog::CustomizeUI()
{
    setWindowTitle(tr("Find Files"));

    ui->searchForComboBox->setEditText("*");
    ui->findTextComboBox->setEnabled(false);
    ui->searchProgressBar->setMaximum(100);
    ui->searchProgressBar->setValue(0);

    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    resize(screenGeometry.width() / 2, screenGeometry.height() / 2);
}

void FindFilesDialog::Connect()
{
    bool connected = true;

    connected &= connect(ui->browseButton, &QPushButton::clicked, [this]
    {
        QString directory = QFileDialog::getExistingDirectory(
            this,
            "Select Directory",
            ui->searchInComboBox->currentText());

        if (!directory.isEmpty())
        {
            if (ui->searchInComboBox->findText(directory) == -1)
            {
                ui->searchInComboBox->addItem(directory);
            }
            ui->searchInComboBox->setCurrentIndex(ui->searchInComboBox->findText(directory));
        }
    }) != Q_NULLPTR;

    connected &= connect(ui->finishButton, &QPushButton::clicked, [this]{ hide(); }) != Q_NULLPTR;
    connected &= connect(ui->findTextCheckBox, &QCheckBox::toggled, [this](bool checked)
    {
        ui->findTextComboBox->setEnabled(checked);
    }) != Q_NULLPTR;

    Q_ASSERT(connected);
}
