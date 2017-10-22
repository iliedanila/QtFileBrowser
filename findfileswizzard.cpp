#include "findfileswizzard.h"

#include <QAbstractButton>
#include <QApplication>
#include <QDesktopWidget>

FindFilesWizzard::FindFilesWizzard(const QString& rootFolder, QWidget *parent)
:
    QWizard(parent)
{
    settings = new FindFilesSettings(rootFolder, this);
    results = new FindFilesResults(this);

    CustomizeUI();
}

FindFilesWizzard::~FindFilesWizzard()
{
}

void FindFilesWizzard::initializePage(int id)
{
    if (page(id) == settings)
    {
        const QList<WizardButton> buttonList { NextButton, CancelButton };
        setButtonLayout(buttonList);
        setButtonText(NextButton, "Search");
    }
    else
    {
        const QList<WizardButton> buttonList { BackButton, CancelButton, FinishButton };
        setButtonLayout(buttonList);
        button(CancelButton)->setEnabled(false);
    }
}

void FindFilesWizzard::CustomizeUI()
{
    addPage(settings);
    addPage(results);

    setButtonText(NextButton, "Search");

    setWindowTitle("Find Files");
    const QRect screenGeometry = QApplication::desktop()->screenGeometry(this);
    resize(screenGeometry.width() / 2, screenGeometry.height() / 3);
}
