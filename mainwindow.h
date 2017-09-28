#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "fileoperation.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    void CustomizeUI();
    void Connect();

private slots:
    void switchToLeftBrowser();
    void switchToRightBrowser();
    void handleCopy();
    void handleDel();
    void handleNewFolder();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
