#ifndef FILESYSTEMVIEW_H
#define FILESYSTEMVIEW_H

#include <QTableView>

class FileSystemView : public QTableView
{
    Q_OBJECT

public:
    explicit FileSystemView(QWidget *parent = Q_NULLPTR);

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void focusInEvent(QFocusEvent *event) override;
    virtual void resizeEvent(QResizeEvent *event) override;

signals:
    void switchMe();
    void gotFocus();
    void goToParent();
    void search();
    void copy();
    void move();
    void del();
    void newFolder();

private:
    void setColumnsWidth();
};

#endif // FILESYSTEMVIEW_H
