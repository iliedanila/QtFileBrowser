#ifndef FINDFILESDIALOG_H
#define FINDFILESDIALOG_H

#include <QDialog>

#include "findfilesoperation.h"
#include "findresultsmodel.h"

namespace Ui {
class FindFilesDialog;
}

class FindFilesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FindFilesDialog(QWidget *parent = 0);
    ~FindFilesDialog();

    void setDirectory(const QString& aDirectory);

public slots:
    void cancel();

private slots:
    void operationStarted();
    void operationFinished();
    void processedEntriesCount(int);
    void entriesCountChanged(int);

private:
    void CustomizeUI();
    void Connect();
    void browse();

    Ui::FindFilesDialog *ui;
    FindFilesOperation* findOperation;
    FindResultsModel* resultsModel;
    int processedEntries;
    int totalEntries;
};

#endif // FINDFILESDIALOG_H
