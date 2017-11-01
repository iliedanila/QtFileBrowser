#include "findfilesdialog.h"
#include "ui_findfilesdialog.h"

#include <QDesktopWidget>
#include <QFileDialog>
#include <QDebug>
#include <QKeyEvent>
#include <QLineEdit>
#include <QDesktopServices>

FindFilesDialog::FindFilesDialog(QWidget *parent)
:
    QDialog(parent),
    ui(new Ui::FindFilesDialog),
    processedEntries(0),
    totalEntries(0),
    searchRunning(false)
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

void FindFilesDialog::focusInEvent(QFocusEvent* event)
{
    ui->searchForComboBox->lineEdit()->setFocus(Qt::OtherFocusReason);
    QDialog::focusInEvent(event);
}

void FindFilesDialog::keyPressEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_Return && event->modifiers() == Qt::NoModifier)
    {
        resultsModel->clear();
        findOperation->start();
    }
    else if (event->key() == Qt::Key_Escape && event->modifiers() == Qt::NoModifier && searchRunning)
    {
        findOperation->cancel();
    }
    else
    {
        QDialog::keyPressEvent(event);
    }
}

void FindFilesDialog::cancel()
{
    findOperation->cancel();
}

void FindFilesDialog::operationStarted()
{
    searchRunning = true;
    ui->stackedWidget->setCurrentIndex(1);
}

void FindFilesDialog::operationFinished()
{
    searchRunning = false;
    ui->stackedWidget->setCurrentIndex(0);
}

void FindFilesDialog::processedEntriesCount(int count)
{
    processedEntries = count;
    ui->counterLabel->setText(QString::number(processedEntries) + " / " + QString::number(totalEntries) + " entries.");
    auto percent = processedEntries * 100 / totalEntries;
    ui->searchProgressBar->setValue(percent);
}

void FindFilesDialog::entriesCountChanged(int count)
{
    totalEntries = count;
    ui->counterLabel->setText(QString::number(processedEntries) + " / " + QString::number(totalEntries) + " entries.");
    auto percent = processedEntries * 100 / totalEntries;
    ui->searchProgressBar->setValue(percent);
}

void FindFilesDialog::openFileLocation(const QModelIndex& index) const
{
    QFileInfo fileInfo(resultsModel->fileName(index.row()));
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileInfo.dir().absolutePath()));
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
    connect(ui->searchButton, &QPushButton::clicked, resultsModel, &FindResultsModel::clear);
    connect(ui->cancelButton, &QPushButton::clicked, findOperation, &FindFilesOperation::cancel);
    connect(findOperation, &FindFilesOperation::started, this, &FindFilesDialog::operationStarted);
    connect(findOperation, &FindFilesOperation::finished, this, &FindFilesDialog::operationFinished);
    connect(findOperation, &FindFilesOperation::entriesCountChanged, this, &FindFilesDialog::entriesCountChanged);
    connect(findOperation, &FindFilesOperation::processedEntriesCount, this, &FindFilesDialog::processedEntriesCount);
    connect(findOperation, &FindFilesOperation::foundMatch, resultsModel, &FindResultsModel::foundMatch);
    connect(ui->searchForComboBox, &QComboBox::currentTextChanged, findOperation, &FindFilesOperation::setFileName);
    connect(ui->resultsView, &QTableView::doubleClicked, this, &FindFilesDialog::openFileLocation);
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
