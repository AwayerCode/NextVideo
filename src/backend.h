#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>

class Backend : public QObject
{
    Q_OBJECT
public:
    explicit Backend(QObject *parent = nullptr);

public slots:
    QString getMessage() const;
    void setMessage(const QString &message);

signals:
    void messageChanged();

private:
    QString m_message;
};

#endif // BACKEND_H 