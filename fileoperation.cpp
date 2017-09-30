#include "fileoperation.h"

#include <QFileInfo>
#include <QFile>
#include <QDir>

FileOperation::FileOperation(
    OperationType type,
    QString _rootFolder,
    QStringList _sourceFiles,
    QString _destination,
    QObject* parent)
:
    QThread(parent),
    operationType(type),
    rootFolder(_rootFolder),
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
        filesList = sourceFiles;
        foldersList.clear();

        int index = 0;
        while(index < filesList.count() && !atomicCancel)
        {
            QFileInfo fileInfo(filesList.at(index));
            if (fileInfo.isDir())
            {
                foldersList.push_back(fileInfo.absoluteFilePath());
                filesList.removeAt(index);
                QDir dir(fileInfo.absoluteFilePath());
                auto entryInfoList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
                for (auto entryInfo : entryInfoList)
                {
                    filesList.push_back(entryInfo.absoluteFilePath());
                }
            }
            else
            {
                index++;
                totalSize += fileInfo.size();
            }
        }
    }
    else if (operationType == eDelete || operationType == eMove)
    {
        filesList = sourceFiles;
        foldersList.clear();

        int index = 0;
        while(index < filesList.count() && !atomicCancel)
        {
            QFileInfo fileInfo(filesList.at(index));
            if (fileInfo.isDir())
            {
                foldersList.push_back(fileInfo.absoluteFilePath());
                filesList.removeAt(index);
                QDir dir(fileInfo.absoluteFilePath());
                auto entryInfoList = dir.entryInfoList(QDir::NoDotAndDotDot | QDir::Dirs | QDir::Files);
                for (auto entryInfo : entryInfoList)
                {
                    filesList.push_back(entryInfo.absoluteFilePath());
                }
            }
            else
            {
                index++;
            }
        }

        totalSize = filesList.count() + foldersList.count();
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
    else if (operationType == eMove)
    {
        move();
    }
}

void FileOperation::copy()
{
    qint64 totalSize = getTotalSize();
    qint64 bytesCopied = 0;

    for (QString folderPath : foldersList)
    {
        auto relativePathToRoot = folderPath.remove(rootFolder);
        QDir dir(destination + "/" + relativePathToRoot);
        if (!dir.exists())
        {
            dir.mkpath(".");
        }
    }

    for (auto filePath : filesList)
    {
        QFile sourceFile(filePath);
        QFileInfo sourceFileInfo(filePath);
        QString relativePathToRoot = sourceFileInfo.absoluteFilePath().remove(rootFolder);
        QFile destinationFile(destination + "/" + relativePathToRoot);

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
    qint64 totalCount = getTotalSize();
    qint64 filesDeleted = 0;
    qint64 foldersDeleted = 0;

    while(filesList.count() && !atomicCancel)
    {
        QFile sourceFile(filesList.first());
        filesList.pop_front();
        sourceFile.remove();
        filesDeleted++;
        int percent = 100 * filesDeleted / totalCount;
        emit setProgress(percent);
    }

    while (foldersList.count() && !atomicCancel)
    {
        QDir dir(foldersList.last());
        foldersList.pop_back();
        dir.removeRecursively();
        foldersDeleted++;
        int percent = 100 * (filesDeleted + foldersDeleted) / totalCount;
        emit setProgress(percent);
    }
}

void FileOperation::move()
{
    qint64 totalCount = getTotalSize();
    qint64 filesMoved = 0;
    qint64 foldersMoved = 0;
    QStringList tempFolderList;

    // Copy folders (empty).
    while (foldersList.count() && !atomicCancel)
    {
        auto folderPath = foldersList.last();
        foldersList.pop_back();
        tempFolderList.push_front(folderPath);

        auto relativePathToRoot = folderPath.remove(rootFolder);
        QDir dir(destination + "/" + relativePathToRoot);
        if (!dir.exists())
        {
            dir.mkpath(".");
        }
        foldersMoved++;
        int percent = 100 * foldersMoved / totalCount;
        emit setProgress(percent);
    }

    // Move files.
    while (filesList.count() && !atomicCancel)
    {
        auto filePath = filesList.front();
        QFile sourceFile(filePath);
        filesList.pop_front();
        QFileInfo sourceFileInfo(filePath);
        QString relativePathToRoot = sourceFileInfo.absoluteFilePath().remove(rootFolder);
        QString destinationFilePath(destination + "/" + relativePathToRoot);

        sourceFile.rename(destinationFilePath);
        filesMoved++;
        int percent = 100 * (filesMoved + foldersMoved) / totalCount;
        emit setProgress(percent);
    }

    // Delete initial folders.
    while (tempFolderList.count())
    {
        QDir dir(tempFolderList.last());
        tempFolderList.pop_back();
        dir.removeRecursively();
    }
}

