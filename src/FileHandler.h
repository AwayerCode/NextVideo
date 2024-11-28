#pragma once

#include <QObject>
#include "deleter/deleter.h"
#include "fileinfo/fileinfo.h"

class FileHandler : public QObject
{
    Q_OBJECT

public:
    explicit FileHandler(QObject *parent = nullptr);

    void init();
    void initData();
    void initConnections();

public slots:
    void openFileDialog();
    void startOperation(const QString& operation);

signals:
    void fileSelected(const QString& filePath);
    void logUpdated(const QString& log);

private:    
    void deleteFile();
    void getFileInfo();

private:
    QString m_selectedFile;

    std::unique_ptr<Deleter> m_deleter;
    std::unique_ptr<FileInfo> m_fileInfo;
};