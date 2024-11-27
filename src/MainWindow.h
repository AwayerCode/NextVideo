#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QObject>
#include <QString>

class MainWindow : public QObject
{
    Q_OBJECT
public:
    explicit MainWindow(QObject *parent = nullptr);
    ~MainWindow();

public slots:

signals:

private:

};

#endif // MAINWINDOW_H  