#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QItemSelection>
#include "customfilesystemmodel.h"

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

private:
    Ui::MainWindow *ui;
    CustomFileSystemModel* model;
};

#endif // MAINWINDOW_H
