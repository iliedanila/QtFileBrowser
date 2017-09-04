#ifndef DRIVELIST_H
#define DRIVELIST_H

#include <QWidget>
#include <QToolButton>
#include <QFileIconProvider>
#include <QTimer>
#include <QSignalMapper>

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
    void setPath(QString);
    void pathNotAvailable(QString);

private slots:
    void handleSetHome();
    void onTimer();
    void buttonClicked(QString);

private:
    Ui::DriveList *ui;
    QFileIconProvider iconProvider;
    QList<QToolButton*> buttonList;
    QTimer* timer;
    QSignalMapper* signalMapper;
};

#endif // DRIVELIST_H
