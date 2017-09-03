#ifndef DRIVELIST_H
#define DRIVELIST_H

#include <QWidget>
#include <QToolButton>
#include <QFileIconProvider>

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

signals:
    void setHome();

private slots:
    void handleSetHome();

private:
    Ui::DriveList *ui;
    QFileIconProvider iconProvider;
    QList<QToolButton*> buttonList;
};

#endif // DRIVELIST_H
