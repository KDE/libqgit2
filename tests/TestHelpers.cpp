#include "TestHelpers.h"

#include <QDir>

#include "qgitglobal.h"
#include "qgitrepository.h"

using namespace LibQGit2;

void sleep::ms(int msec)
{
    QThread::msleep(msec);
}


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

bool copyDir(QString srcPath, QString destPath)
{
    QDir srcDir(srcPath);
    if (!srcDir.exists()) {
        qDebug() << "Source directory does not exist:" << srcPath;
        return false;
    }

    foreach (QString dir, srcDir.entryList(QDir::Dirs | QDir::NoDotAndDotDot)) {
        QString subDestPath = destPath + QDir::separator() + dir;
        if (!srcDir.mkpath(subDestPath)) {
            qDebug() << "Could not create target directory:" << subDestPath;
            return false;
        }
        if (!copyDir(srcPath + QDir::separator() + dir, subDestPath)) {
            return false;
        }
    }

    foreach (QString file, srcDir.entryList(QDir::Files)) {
        if (!QFile::copy(srcPath + QDir::separator() + file, destPath + QDir::separator() + file)) {
            qDebug() << "Could not copy" << file << "from" << srcPath << "to" << destPath;
            return false;
        }
    }

    return true;
}

bool libgit2HasSSH() {
    return git_libgit2_features() & GIT_FEATURE_SSH;
}


void TestBase::init() {
    testdir = VALUE_TO_QSTR(TEST_DIR) + "/" + QFileInfo(QTest::currentAppName()).fileName() + "/" + QTest::currentTestFunction();
    QVERIFY(removeDir(testdir));
}

void TestBase::cleanup() {

}

void TestBase::initTestCase() {
    initLibQGit2();
}

void TestBase::cleanupTestCase() {
    shutdownLibQGit2();
}

void TestBase::initTestRepo()
{
    try {
        Repository repo;
        repo.clone(FileRepositoryUrl, testdir);
    } catch (const Exception& ex) {
        QFAIL(ex.what());
    }
}
