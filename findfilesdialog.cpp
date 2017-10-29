#include "findfilesdialog.h"
#include "ui_findfilesdialog.h"

#include <QDesktopWidget>
#include <QFileDialog>
#include <QDebug>

FindFilesDialog::FindFilesDialog(QWidget *parent)
:
    QDialog(parent),
    ui(new Ui::FindFilesDialog),
    processedEntries(0),
    totalEntries(0)
{
    ui->setupUi(this);
    findOperation = new FindFilesOperation(this);
    resultsModel = new FindResultsModel(this);

    CustomizeUI();
    Connect();
}

FindFilesDialog::~FindFilesDialog()
{
    findOperation->cancel();
    QThread::currentThread()->msleep(200);
    delete ui;
}

void FindFilesDialog::setDirectory(const QString &aDirectory)
{
    ui->searchInComboBox->setEditText(aDirectory);
    findOperation->setSearchFolder(aDirectory);
}

void FindFilesDialog::cancel()
{
    findOperation->cancel();
}

void FindFilesDialog::operationStarted()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void FindFilesDialog::operationFinished()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void FindFilesDialog::processedEntriesCount(int count)
{
    processedEntries = count;
    ui->counterLabel->setText(QString::number(processedEntries) + " / " + QString::number(totalEntries));
}

void FindFilesDialog::entriesCountChanged(int count)
{
    totalEntries = count;
    ui->counterLabel->setText(QString::number(processedEntries) + " / " + QString::number(totalEntries));
}

void FindFilesDialog::CustomizeUI()
{
    setWindowTitle(tr("Find Files"));

    ui->searchForComboBox->setEditText("*");
    ui->findTextComboBox->setEnabled(false);
    ui->searchProgressBar->setMaximum(100);
    ui->searchProgressBar->setValue(0);
    ui->resultsView->setModel(resultsModel);
    ui->resultsView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->resultsView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->resultsView->setShowGrid(false);
    ui->resultsView->verticalHeader()->hide();
    ui->resultsView->horizontalHeader()->setStretchLastSection(true);
    ui->searchButton->setFocus();

    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    resize(screenGeometry.width() / 2, screenGeometry.height() / 2);
}

void FindFilesDialog::Connect()
{
    connect(ui->browseButton, &QPushButton::clicked, this, &FindFilesDialog::browse);
    connect(ui->findTextCheckBox, &QCheckBox::toggled, [this](bool checked)
    {
        ui->findTextComboBox->setEnabled(checked);
    });
    connect(ui->searchButton, &QPushButton::clicked, [this]{ findOperation->start(); });
    connect(ui->cancelButton, &QPushButton::clicked, findOperation, &FindFilesOperation::cancel);
    connect(findOperation, &FindFilesOperation::started, this, &FindFilesDialog::operationStarted);
    connect(findOperation, &FindFilesOperation::finished, this, &FindFilesDialog::operationFinished);
    connect(findOperation, &FindFilesOperation::entriesCountChanged, this, &FindFilesDialog::entriesCountChanged);
    connect(findOperation, &FindFilesOperation::processedEntriesCount, this, &FindFilesDialog::processedEntriesCount);
    connect(findOperation, &FindFilesOperation::foundMatch, resultsModel, &FindResultsModel::foundMatch);
    connect(ui->searchForComboBox, &QComboBox::currentTextChanged, findOperation, &FindFilesOperation::setFileName);
}

void FindFilesDialog::browse()
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
}
