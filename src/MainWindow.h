#pragma once

#include <QObject>

class MainWindow : public QObject
{
    Q_OBJECT
    
public:
    explicit MainWindow(QObject *parent = nullptr);
}; 