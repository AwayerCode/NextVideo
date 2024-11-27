#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <QObject>
#include <QString>

class FileHandler : public QObject
{
    Q_OBJECT
public:
    explicit FileHandler(QObject *parent = nullptr);

public slots:
    void openFileDialog();
    void deleteFile();

signals:
    void fileSelected(const QString &filePath);
    void logUpdated(const QString &log);

private:
    QString filePath { "" };
};

#endif