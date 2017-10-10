#include "findfileswizzard.h"

FindFilesWizzard::FindFilesWizzard(QWidget *parent)
:
    QWizard(parent)
{
    settings = new FindFilesSettings(this);
    results = new FindFilesResults(this);

    addPage(settings);
    addPage(results);
}
