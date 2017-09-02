#ifndef DRIVELIST_H
#define DRIVELIST_H

#include <QWidget>

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
    Ui::DriveList *ui;
};

#endif // DRIVELIST_H
