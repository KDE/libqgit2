#ifndef LIBQGIT2_TEST_HELPERS_H
#define LIBQGIT2_TEST_HELPERS_H

#include <QTest>

#include <QDir>
#include <QDebug>
#include <QTimer>
#include <QThread>

#include "git2.h"
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

bool libgit2HasSSH() {
#if LIBGIT2_VER_MAJOR <= 0 && LIBGIT2_VER_MINOR <= 20
	bool hasSSH = git_libgit2_capabilities() & GIT_CAP_SSH;
#else
	bool hasSSH = git_libgit2_features() & GIT_FEATURE_SSH;
#endif

	return hasSSH;
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#define SKIPTEST(description) QSKIP(description, QTest::SkipSingle);
#else
#define SKIPTEST(description) QSKIP(description);
#endif


#endif  // LIBQGIT2_TEST_HELPERS_H
