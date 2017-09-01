#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelection>
#include "folderViewModel.h"
#include "fileViewModel.h"

namespace Ui {
class MainWindow;
//class CustomFileSystemModel;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void setNewFolder(const QString& newFolder);
    void setHome();
    void handleSelectionChanged(QItemSelection,QItemSelection);
    void handleFileModelLayoutChanged(QList<QPersistentModelIndex>, QAbstractItemModel::LayoutChangeHint);
    void handleShowHidden(bool);

private:
    Ui::MainWindow *ui;
    CustomFileSystemModel* folderModel;
    FileViewModel* fileModel;
};

#endif // MAINWINDOW_H
