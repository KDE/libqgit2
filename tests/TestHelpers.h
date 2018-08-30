#ifndef LIBQGIT2_TEST_HELPERS_H
#define LIBQGIT2_TEST_HELPERS_H

#include <QTest>
#include <QDebug>

#include <QThread>

#include "qgitexception.h"

#define TO_STR(s) #s
#define VALUE_TO_STR(s) TO_STR(s)
#define VALUE_TO_QSTR(s) QLatin1String(TO_STR(s))

const QString HttpRemoteUrl("http://anongit.kde.org/libqgit2");
const QString HttpsRemoteUrl("https://github.com/lgiordani/libqgit2.git");
const QString GitRemoteUrl("git://anongit.kde.org/libqgit2");
const QString ExistingRepository(VALUE_TO_QSTR(TEST_EXISTING_REPOSITORY));
const QString FileRepositoryUrl("file://" + ExistingRepository + "/.git");


struct sleep : QThread
{
    static void ms(int msec);
};


bool removeDir(const QString & dirName);

bool copyDir(QString srcPath, QString destPath);

bool libgit2HasSSH();


class TestBase : public QObject
{
    Q_OBJECT
protected slots:
    virtual void init();
    virtual void cleanup();
    virtual void initTestCase();
    virtual void cleanupTestCase();

protected:
    void initTestRepo();

    QString testdir;
};

#define EXPECT_THROW(statement, exClass) \
do {\
    try {\
        statement;\
        QTest::qFail(#statement " didn't throw", __FILE__, __LINE__);\
        return;\
    } catch (const exClass&) {\
    } catch (...) {\
        QTest::qFail(#statement " threw an unexpected kind of exception", __FILE__, __LINE__);\
        return;\
    }\
} while (0)


#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#define SKIPTEST(description) QSKIP(description, SkipSingle);
#else
#define SKIPTEST(description) QSKIP(description);
#endif


#endif  // LIBQGIT2_TEST_HELPERS_H
