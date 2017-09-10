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
    OperationType operationType;
    QStringList sourceFiles;
    QString destination;
    QAtomicInt atomicCancel;

};

#endif // FILEOPERATION_H
