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
    QString openFileDialog();
    
signals:
    void fileSelected(const QString &filePath);
};

#endif