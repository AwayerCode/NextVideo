#pragma once

#include <QObject>
#include "deleter/deleter.h"

class FileHandler : public QObject
{
    Q_OBJECT

public:
    explicit FileHandler(QObject *parent = nullptr);

public slots:
    void openFileDialog();
    void deleteFile();

signals:
    void fileSelected(const QString& filePath);
    void logUpdated(const QString& log);

private:
    QString m_selectedFile;
    Deleter m_deleter;
};