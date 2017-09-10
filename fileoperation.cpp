#include "fileoperation.h"

#include <QFileInfo>
#include <QFile>

FileOperation::FileOperation(OperationType type, QStringList _sourceFiles, QString _destination, QObject* parent)
    :
    QThread(parent),
    operationType(type),
    sourceFiles(_sourceFiles),
    destination(_destination)
{
}

qint64 FileOperation::getTotalSize()
{
    qint64 totalSize = 0;
    for (auto filePath : sourceFiles)
    {
        QFileInfo fileInfo(filePath);
        if (!fileInfo.isDir())
        {
            totalSize += fileInfo.size();
        }
    }
    return totalSize;
}

void FileOperation::run()
{
    qint64 totalSize = getTotalSize();
    qint64 bytesCopied = 0;

    for (auto filePath : sourceFiles)
    {
        QFile sourceFile(filePath);
        QFileInfo sourceFileInfo(filePath);
        QFile destinationFile(destination + "/" + sourceFileInfo.fileName());

        if (destinationFile.exists())
            continue;

        sourceFile.open(QFile::ReadOnly);
        destinationFile.open(QFile::WriteOnly);

        QByteArray buffer;
        while (!(buffer = sourceFile.read(1024 * 100)).isEmpty())
        {
            bytesCopied += buffer.size();
            destinationFile.write(buffer);
            int percent = 100 * bytesCopied / totalSize;
            emit setProgress(percent);
        }
    }
}

