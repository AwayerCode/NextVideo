#include "Fileinfo.h"
#include <QFileInfo>
#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

FileInfo::FileInfo(QObject *parent)
    : QObject(parent)
    , m_fileSize(0)
    , m_isExecutable(false)
{
}

bool FileInfo::getFileInfo(const QString& filePath)
{
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists()) {
        emit logMessage(QString("File not found: %1").arg(filePath));
        return false;
    }

    m_fileName = fileInfo.fileName();
    m_filePath = fileInfo.absoluteFilePath();
    m_fileSize = fileInfo.size();
    m_creationTime = fileInfo.birthTime();
    m_lastModified = fileInfo.lastModified();
    m_isExecutable = fileInfo.suffix().toLower() == "exe";

    if (m_isExecutable) {
        m_fileVersion = getWindowsFileVersion(filePath);
    }

    emit logMessage(QString("File information loaded: %1").arg(m_fileName));
    return true;
}

QString FileInfo::getWindowsFileVersion(const QString& filePath)
{
#ifdef Q_OS_WIN
    DWORD handle;
    DWORD size = GetFileVersionInfoSizeW((LPCWSTR)filePath.utf16(), &handle);
    if (size == 0) {
        return QString();
    }

    QByteArray buffer(size, 0);
    if (!GetFileVersionInfoW((LPCWSTR)filePath.utf16(), handle, size, buffer.data())) {
        return QString();
    }

    VS_FIXEDFILEINFO* fileInfo;
    UINT len;
    if (!VerQueryValueW(buffer.data(), L"\\", (LPVOID*)&fileInfo, &len)) {
        return QString();
    }

    return QString("%1.%2.%3.%4")
        .arg(HIWORD(fileInfo->dwFileVersionMS))
        .arg(LOWORD(fileInfo->dwFileVersionMS))
        .arg(HIWORD(fileInfo->dwFileVersionLS))
        .arg(LOWORD(fileInfo->dwFileVersionLS));
#else
    return QString();
#endif
} 