#include "fileoperation.h"

FileOperation::FileOperation(QObject* parent)
    :
    QObject(parent)
{

}

void FileOperation::executeOperation(FileOperation::OperationType type, QString parameter1, QString parameter2)
{
    Q_UNUSED(parameter1);
    Q_UNUSED(parameter2);

    switch (type)
    {
    case eView:
    case eEdit:
    case eCopy:
    case eMove:
    case eNewFolder:
    case eDelete:
    default:
        break;
    }
}
