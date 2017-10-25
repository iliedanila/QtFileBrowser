#ifndef FINDFILESDIALOG_H
#define FINDFILESDIALOG_H

#include <QDialog>

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

private:
    void CustomizeUI();
    void Connect();
    void browse();

    Ui::FindFilesDialog *ui;
};

#endif // FINDFILESDIALOG_H
