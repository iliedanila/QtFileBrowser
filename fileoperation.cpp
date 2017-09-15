#include "fileoperation.h"

#include <QFileInfo>
#include <QFile>

FileOperation::FileOperation(OperationType type, QStringList _sourceFiles, QString _destination, QObject* parent)
    :
    QThread(parent),
    operationType(type),
    sourceFiles(_sourceFiles),
    destination(_destination),
    atomicCancel(0)
{
}

qint64 FileOperation::getTotalSize()
{
    qint64 totalSize = 0;

    if (operationType == eCopy)
    {
        for (auto filePath : sourceFiles)
        {
            QFileInfo fileInfo(filePath);
            if (!fileInfo.isDir())
            {
                totalSize += fileInfo.size();
            }
        }
    }
    else if (operationType == eDelete)
    {
        totalSize = sourceFiles.count();
    }
    return totalSize;
}

void FileOperation::cancel()
{
    atomicCancel = true;
}

void FileOperation::run()
{
    if (operationType == eCopy)
    {
        copy();
    }
    else if (operationType == eDelete)
    {
        del();
    }
}

void FileOperation::copy()
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
        while (!(buffer = sourceFile.read(1024 * 100)).isEmpty() && !atomicCancel)
        {
            bytesCopied += buffer.size();
            destinationFile.write(buffer);
            int percent = 100 * bytesCopied / totalSize;
            emit setProgress(percent);
        }

        if (atomicCancel)
        {
            destinationFile.remove();
        }
    }
}

void FileOperation::del()
{
    qint64 totalFileCount = getTotalSize();
    qint64 filesDeleted = 0;

    while(sourceFiles.count() && !atomicCancel)
    {
        QFile sourceFile(*sourceFiles.begin());
        sourceFiles.pop_front();
        sourceFile.remove();
        filesDeleted++;
        int percent = 100 * filesDeleted / totalFileCount;
        emit setProgress(percent);
    }
}

