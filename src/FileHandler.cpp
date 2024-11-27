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
    
    qDebug() << "Selected file: " << filePath;

    if (!filePath.isEmpty()) {
        emit fileSelected(filePath);
    }
}