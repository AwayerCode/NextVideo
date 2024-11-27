#include "filehandler.h"
#include <QFileDialog>
#include <QStandardPaths>

FileHandler::FileHandler(QObject *parent) : QObject(parent)
{
}

void FileHandler::openFileDialog()
{
    QString filePath = QFileDialog::getOpenFileName(
        nullptr,
        tr("Select Process File"),
        QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation),
        tr("All Files (*.*)"));

    if (!filePath.isEmpty()) {
        this->filePath = filePath;
        emit fileSelected(filePath);
        emit logUpdated("File selected: " + filePath);
    }
    else {
        emit logUpdated("No file selected");
    }
}

void FileHandler::deleteFile()
{
    emit logUpdated("Deleting file: " + filePath);

    bool success = QFile(filePath).remove();
    if (success) {
        emit logUpdated("File deleted successfully: " + filePath);
    }
    else {
        emit logUpdated("File deleted failed: " + filePath);
    }
}
