#ifndef BROWSERWIDGET_H
#define BROWSERWIDGET_H

#include <QWidget>
#include <QFileSystemWatcher>
#include <QFileIconProvider>
#include <QTimer>
#include <QCompleter>
#include <QDirModel>


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
    void switchMe();
    void copy();
    void del();
    void newFolder();

private slots:
    void enterFolder(QModelIndex);
    void handleRootPathChanged(QString);
    void handleSwitchMeRequest();
    void handleGotFocus();
    void goToParent();
    void setPath(QString);
    void setHome();
    void checkPathNotAvailable();
    void populateDriveList();

private:
    Ui::BrowserWidget *ui;
    QFileIconProvider iconProvider;
    FileSystemModel* fileSystemModel;
    QTimer* driveTimer;
    QCompleter* completer;
    QDirModel* dirModel;
};

#endif // BROWSERWIDGET_H
