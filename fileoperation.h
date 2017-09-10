#ifndef FILEOPERATION_H
#define FILEOPERATION_H

#include <QObject>

class FileOperation : public QObject
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

    explicit FileOperation(QObject* parent = Q_NULLPTR);

public slots:
    void executeOperation(OperationType type, QString parameter1, QString parameter2 = QString());
};

#endif // FILEOPERATION_H
