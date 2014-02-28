#pragma once

#include <QTest>

#include <QDir>
#include <QDebug>
#include <QTimer>
#include <QThread>

#include "qgitexception.h"

#define TO_STR(s) #s
#define VALUE_TO_STR(s) TO_STR(s)
#define VALUE_TO_QSTR(s) QLatin1String(TO_STR(s))


struct sleep : QThread
{
    static void ms(int msec) { QThread::msleep(msec); }
};


bool removeDir(const QString & dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
                if (!result) {
                    QFile(info.absoluteFilePath()).setPermissions(QFile::WriteOwner);
                    result = QFile::remove(info.absoluteFilePath());
                }
                if (!result) {
                    qDebug() << "Could not remove " << info.absoluteFilePath();
                }
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
        if (!result) {
            qDebug() << "Could not remove " << dirName;
        }
    }
    return result;
}