#pragma once

#include <QString>
#include <QObject>
#include <QDateTime>

class FileInfo : public QObject
{
    Q_OBJECT

public:
    explicit FileInfo(QObject *parent = nullptr);
    
    bool getFileInfo(const QString& filePath);
    QString getFileName() const { return m_fileName; }
    QString getFilePath() const { return m_filePath; }
    qint64 getFileSize() const { return m_fileSize; }
    QDateTime getCreationTime() const { return m_creationTime; }
    QDateTime getLastModified() const { return m_lastModified; }
    QString getFileVersion() const { return m_fileVersion; }
    bool isExecutable() const { return m_isExecutable; }

signals:
    void logMessage(const QString& message);

private:
    QString m_fileName;
    QString m_filePath;
    qint64 m_fileSize;
    QDateTime m_creationTime;
    QDateTime m_lastModified;
    QString m_fileVersion;
    bool m_isExecutable;
    
    QString getWindowsFileVersion(const QString& filePath);
}; 