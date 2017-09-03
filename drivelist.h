#ifndef DRIVELIST_H
#define DRIVELIST_H

#include <QWidget>
#include <QToolButton>
#include <QFileIconProvider>
#include <QTimer>

namespace Ui {
class DriveList;
}

class DriveList : public QWidget
{
    Q_OBJECT

public:
    explicit DriveList(QWidget *parent = 0);
    ~DriveList();

private:
    void CustomizeUI();
    void Connect();
    void CreateButtonsForDrives();

signals:
    void setHome();

private slots:
    void handleSetHome();
    void onTimer();

private:
    Ui::DriveList *ui;
    QFileIconProvider iconProvider;
    QList<QToolButton*> buttonList;
    QTimer* timer;
};

#endif // DRIVELIST_H
