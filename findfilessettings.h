#ifndef FINDFILESSETTINGS_H
#define FINDFILESSETTINGS_H

#include <QWizardPage>

namespace Ui {
class FindFilesSettings;
}

class FindFilesSettings : public QWizardPage
{
    Q_OBJECT

public:
    explicit FindFilesSettings(QWidget *parent = 0);
    ~FindFilesSettings();

private:
    Ui::FindFilesSettings *ui;
};

#endif // FINDFILESSETTINGS_H
