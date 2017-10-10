#ifndef FINDFILESRESULTS_H
#define FINDFILESRESULTS_H

#include <QWizardPage>

namespace Ui {
class FindFilesResults;
}

class FindFilesResults : public QWizardPage
{
    Q_OBJECT

public:
    explicit FindFilesResults(QWidget *parent = 0);
    ~FindFilesResults();

private:
    Ui::FindFilesResults *ui;
};

#endif // FINDFILESRESULTS_H
