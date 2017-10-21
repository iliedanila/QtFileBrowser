#include "findfileswizzard.h"

FindFilesWizzard::FindFilesWizzard(const QString& rootFolder, QWidget *parent)
:
    QWizard(parent)
{
    settings = new FindFilesSettings(rootFolder, this);
    results = new FindFilesResults(this);

    addPage(settings);
    addPage(results);

    setButtonText(NextButton, "Search");
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
        const QList<WizardButton> buttonList { BackButton, FinishButton };
        setButtonLayout(buttonList);
    }
}
