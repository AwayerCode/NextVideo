#include "FileHandler.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>

FileHandler::FileHandler(QObject *parent)
    : QObject(parent)
{
}

void FileHandler::init()
{
    initData();
    initConnections();
}

void FileHandler::initData()
{
    m_deleter = std::make_unique<Deleter>();
    m_fileInfo = std::make_unique<FileInfo>();
}

void FileHandler::initConnections()
{
    connect(m_fileInfo.get(), &FileInfo::logUpdated, this, &FileHandler::logUpdated);
}

void FileHandler::openFileDialog()
{
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString filePath = QFileDialog::getOpenFileName(nullptr,
        tr("Select Process File"),
        defaultPath,
        tr("Executable Files (*.exe);;All Files (*.*)"));

    if (!filePath.isEmpty()) {
        m_selectedFile = filePath;
        emit fileSelected(filePath);
    }
}

void FileHandler::startOperation(const QString& operation)
{
    if (operation.isEmpty()) {
        emit logUpdated("Please select an operation");
        return;
    }

    if (operation == "Delete") {
        deleteFile();
    } else if (operation == "File Info") {
        getFileInfo();
    }
}

void FileHandler::deleteFile()
{
    if (m_selectedFile.isEmpty()) {
        emit logUpdated("Please select a file first");
        return;
    }

    if (m_deleter->deleteFile(m_selectedFile)) {
        emit logUpdated("Operation completed");
    }
}

void FileHandler::getFileInfo()
{
    if (m_selectedFile.isEmpty()) {
        emit logUpdated("Please select a file first");
        return;
    }

    m_fileInfo->getFileInfo(m_selectedFile);
}

