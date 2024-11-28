#pragma once

#include <QString>
#include <QObject>

class Deleter : public QObject
{
    Q_OBJECT

public:
    explicit Deleter(QObject *parent = nullptr);
    
    bool deleteFile(const QString& filePath);
    bool isProcessRunning(const QString& processName);
    bool killProcess(const QString& processName);

signals:
    void logMessage(const QString& message);
}; 