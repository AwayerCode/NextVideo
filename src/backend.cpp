#include "backend.h"

Backend::Backend(QObject *parent)
    : QObject(parent)
    , m_message("Hello from C++!")
{
}

QString Backend::getMessage() const
{
    return m_message;
}

void Backend::setMessage(const QString &message)
{
    if (m_message != message) {
        m_message = message;
        emit messageChanged();
    }
} 