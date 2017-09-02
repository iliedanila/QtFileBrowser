#ifndef BROWSERWIDGET_H
#define BROWSERWIDGET_H

#include <QWidget>
#include <QFileSystemWatcher>
#include "fileSystemModel.h"

namespace Ui {
class BrowserWidget;
}

class BrowserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BrowserWidget(QWidget *parent = 0);
    ~BrowserWidget();

private:
    void CustomizeUI();
    void Connect();

private slots:
    void enterFolder(QModelIndex);

private:
    Ui::BrowserWidget *ui;

    FileSystemModel* fileSystemModel;
};

#endif // BROWSERWIDGET_H
