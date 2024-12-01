#include "deleter.h"
#include <QFile>
#include <QProcess>
#include <QDebug>
#include <QFileInfo>

#ifdef Q_OS_WIN
#include <windows.h>
#include <tlhelp32.h>
#endif

Deleter::Deleter(QObject *parent)
    : QObject(parent)
{
}

bool Deleter::deleteFile(const QString& filePath)
{
    QFile file(filePath);
    if (!file.exists()) {
        emit logMessage(QString("File not found: %1").arg(filePath));
        return false;
    }

    QString fileName = QFileInfo(filePath).fileName();
    if (isProcessRunning(fileName)) {
        emit logMessage(QString("Terminating process: %1").arg(fileName));
        if (!killProcess(fileName)) {
            emit logMessage(QString("Failed to terminate process: %1").arg(fileName));
            return false;
        }
    }

    if (file.remove()) {
        emit logMessage(QString("Successfully deleted file: %1").arg(filePath));
        return true;
    } else {
        emit logMessage(QString("Failed to delete file: %1").arg(filePath));
        return false;
    }
}

bool Deleter::isProcessRunning(const QString& processName)
{
#ifdef Q_OS_WIN
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32W processEntry;
    processEntry.dwSize = sizeof(processEntry);

    if (!Process32FirstW(snapshot, &processEntry)) {
        CloseHandle(snapshot);
        return false;
    }

    do {
        if (QString::fromWCharArray(processEntry.szExeFile) == processName) {
            CloseHandle(snapshot);
            return true;
        }
    } while (Process32NextW(snapshot, &processEntry));

    CloseHandle(snapshot);
#endif
    return false;
}

bool Deleter::killProcess(const QString& processName)
{
#ifdef Q_OS_WIN
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        return false;
    }

    PROCESSENTRY32W processEntry;
    processEntry.dwSize = sizeof(processEntry);

    if (!Process32FirstW(snapshot, &processEntry)) {
        CloseHandle(snapshot);
        return false;
    }

    bool success = false;
    do {
        if (QString::fromWCharArray(processEntry.szExeFile) == processName) {
            HANDLE processHandle = OpenProcess(PROCESS_TERMINATE, FALSE, processEntry.th32ProcessID);
            if (processHandle != NULL) {
                if (TerminateProcess(processHandle, 0)) {
                    success = true;
                }
                CloseHandle(processHandle);
            }
        }
    } while (Process32NextW(snapshot, &processEntry));

    CloseHandle(snapshot);
    return success;
#else
    return false;
#endif
} 