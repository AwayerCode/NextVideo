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
        tr("选择进程文件"),
        defaultPath,
        tr("可执行文件 (*.exe);;所有文件 (*.*)"));

    if (!filePath.isEmpty()) {
        m_selectedFile = filePath;
        emit fileSelected(filePath);
    }
}

void FileHandler::deleteFile()
{
    if (m_selectedFile.isEmpty()) {
        emit logUpdated("请先选择文件");
        return;
    }

    if (m_deleter.deleteFile(m_selectedFile)) {
        emit logUpdated("操作完成");
    }
}
