#ifndef FINDFILES_H
#define FINDFILES_H

#include <QWizard>

#include "findfilessettings.h"
#include "findfilesresults.h"

class FindFilesWizzard : public QWizard
{
    Q_OBJECT

public:
    explicit FindFilesWizzard(const QString& rootFolder, QWidget *parent = Q_NULLPTR);
    ~FindFilesWizzard();

protected:
    void initializePage(int id) override;

private:
    FindFilesSettings* settings;
    FindFilesResults* results;
};

#endif // FINDFILES_H
