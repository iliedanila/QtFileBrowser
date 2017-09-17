#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <QThread>
#include <QAtomicInt>

class FileOperation : public QThread
{
    Q_OBJECT

public:
    enum OperationType
    {
        eView = 0,
        eEdit,
        eCopy,
        eMove,
        eNewFolder,
        eDelete
    };

    explicit FileOperation(
            OperationType type,
            QString rootFolder,
            QStringList sourceFiles,
            QString destination = QString(),
            QObject* parent = Q_NULLPTR);

    qint64 getTotalSize();

signals:
    void setProgress(int);

public slots:
    void cancel();

protected:
    virtual void run() override;

private:
    void copy();
    void del();

private:
    OperationType operationType;
    QString rootFolder;
    QStringList sourceFiles;
    QString destination;
    QAtomicInt atomicCancel;
    QStringList filesList;
    QStringList foldersList;
};

#endif // FILEOPERATION_H
