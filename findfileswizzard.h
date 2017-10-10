#ifndef FINDFILES_H
#define FINDFILES_H

#include <QWizard>

#include "findfilessettings.h"
#include "findfilesresults.h"

class FindFilesWizzard : public QWizard
{
    Q_OBJECT

public:
    explicit FindFilesWizzard(QWidget *parent = Q_NULLPTR);

private:
    FindFilesSettings* settings;
    FindFilesResults* results;
};

#endif // FINDFILES_H
