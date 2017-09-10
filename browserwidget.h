#ifndef BROWSERWIDGET_H
#define BROWSERWIDGET_H

#include <QWidget>
#include <QFileSystemWatcher>
#include "filesystemmodel.h"

namespace Ui {
class BrowserWidget;
}

class BrowserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BrowserWidget(QWidget *parent = 0);
    ~BrowserWidget();
    QStringList getSelected();
    QString getRootPath();

private:
    void CustomizeUI();
    void Connect();
    void SelectFirstRow(bool directoryChanged);

signals:
    void rootPathChanged(QString);
    void switchMe();

private slots:
    void enterFolder(QModelIndex);
    void handleRootPathChanged(QString);
    void handleSwitchMeRequest();
    void handleGotFocus();
    void goToParent();
    void setPath(QString);
    void pathNotAvailable(QString);

private:
    Ui::BrowserWidget *ui;

    FileSystemModel* fileSystemModel;
};

#endif // BROWSERWIDGET_H
