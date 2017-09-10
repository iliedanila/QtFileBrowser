#include "fileoperation.h"

FileOperation::FileOperation(OperationType type, QStringList _sourceFiles, QString _destination, QObject* parent)
    :
    QThread(parent),
    operationType(type),
    sourceFiles(_sourceFiles),
    destination(_destination)
{

}

