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
    explicit FindFilesSettings(const QString& rootFolder, QWidget *parent = Q_NULLPTR);
    ~FindFilesSettings();

private:
    void CustomizeUI() const;
    void Connect();

    Ui::FindFilesSettings *ui;
    QString rootFolder;
};

#endif // FINDFILESSETTINGS_H
