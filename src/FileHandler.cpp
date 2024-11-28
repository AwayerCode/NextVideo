#include "FileHandler.h"
#include <QFileDialog>
#include <QStandardPaths>
#include <QDir>

FileHandler::FileHandler(QObject *parent)
    : QObject(parent)
{
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

void FileHandler::deleteFile()
{
    if (m_selectedFile.isEmpty()) {
        emit logUpdated("Please select a file first");
        return;
    }

    if (m_deleter.deleteFile(m_selectedFile)) {
        emit logUpdated("Operation completed");
    }
}
