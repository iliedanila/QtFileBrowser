#ifndef FINDFILESDIALOG_H
#define FINDFILESDIALOG_H

#include <QDialog>

#include "findfilesoperation.h"
#include "findresultsmodel.h"
#include "lineedit.h"

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

protected:
    void focusInEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

public slots:
    void cancel();

private slots:
    void operationStarted();
    void operationFinished();
    void processedEntriesCount(int);
    void entriesCountChanged(int);
    void openFileLocation(const QModelIndex&) const;

private:
    void CustomizeUI();
    void Connect();
    void browse();

    Ui::FindFilesDialog *ui;
    FindFilesOperation* findOperation;
    FindResultsModel* resultsModel;
    LineEdit* lineEdit;
    int processedEntries;
    int totalEntries;
    bool searchRunning;
};

#endif // FINDFILESDIALOG_H
