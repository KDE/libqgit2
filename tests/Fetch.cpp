/******************************************************************************
* Copyright (C) 2014 Peter Kümmel <syntheticpp@gmx.net>
*
* Permission to use, copy, modify, and distribute the software
* and its documentation for any purpose and without fee is hereby
* granted, provided that the above copyright notice appear in all
* copies and that both that the copyright notice and this
* permission notice and warranty disclaimer appear in supporting
* documentation, and that the name of the author not be used in
* advertising or publicity pertaining to distribution of the
* software without specific, written prior permission.
*
* The author disclaim all warranties with regard to this
* software, including all implied warranties of merchantability
* and fitness.  In no event shall the author be liable for any
* special, indirect or consequential damages or any damages
* whatsoever resulting from loss of use, data or profits, whether
* in an action of contract, negligence or other tortious action,
* arising out of or in connection with the use or performance of
* this software.
*/
#include "TestHelpers.h"

#include <QCoreApplication>
#include <QTimer>


#include <iostream>
#include <bitset>

#include "qgitcommit.h"
#include "qgitrepository.h"

using namespace LibQGit2;


class TestFetch : public QObject
{
    Q_OBJECT

public:
    TestFetch() : testdir(VALUE_TO_STR(TEST_DIR)) {}

public slots:
    void fetchProgress(int p) 
    { 
        m_progress = p;
        if (p % 20 == 0) {
            qDebug() << qPrintable(QString("Progress : %1%").arg(p));
        }
    }

private slots:
    void remoteBranches();
    void remoteAdd();
    void remoteAddExisiting();
    void remoteAddExisitingDifferentUrl();
    void fetchMaster();
    void fetchAdditionalBranch();
    void fetchAll();

private:
    int m_progress;
    const QString testdir;
};


void TestFetch::remoteAdd()
{
    LibQGit2::Repository repo;

    const QString repoPath = testdir + "/fetch_test";

    QVERIFY(removeDir(repoPath));
    sleep::ms(500);
    m_progress = 0;

    try {
        repo.init(repoPath);
        repo.remoteAdd("kde", "http://anongit.kde.org/libqgit2");
    }
    catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }
}


void TestFetch::remoteAddExisiting()
{
    LibQGit2::Repository repo;

    const QString repoPath = testdir + "/fetch_test";

    QVERIFY(removeDir(repoPath));
    sleep::ms(500);

    try {
        repo.init(repoPath);
        repo.remoteAdd("kde", "http://anongit.kde.org/libqgit2");
        repo.remoteAdd("kde", "http://anongit.kde.org/libqgit2");
    }
    catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }

    // TODO verify remote branches with unit test
}



void TestFetch::remoteAddExisitingDifferentUrl()
{
    LibQGit2::Repository repo;

    const QString repoPath = testdir + "/fetch_test";

    QVERIFY(removeDir(repoPath));
    sleep::ms(500);

    try {
        repo.init(repoPath);
        repo.remoteAdd("kde", "http://anongit.kde.org/libqgit2");
    }
    catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }

    try {
        repo.remoteAdd("kde", "XYZ");
    }
    catch (const LibQGit2::Exception&) {
        return;
    }

    QFAIL("Could add invalid remote URL");
}


void TestFetch::fetchMaster()
{
    LibQGit2::Repository repo;

    const QString repoPath = testdir + "/fetch_test";

    QVERIFY(removeDir(repoPath));
    sleep::ms(500);

    try {
        repo.init(repoPath);
        repo.remoteAdd("kde", "http://anongit.kde.org/libqgit2");
        repo.fetch("kde", "master");
    }
    catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }

    // TODO verify remote branches with unit test
}


void TestFetch::fetchAdditionalBranch()
{
    LibQGit2::Repository repo;

    const QString repoPath = testdir + "/fetch_test";

    try {
        repo.open(repoPath);
        repo.fetch("kde", "develop");
    }
    catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }

    // TODO verify remote branches with unit test
}


void TestFetch::fetchAll()
{
    LibQGit2::Repository repo;

    const QString repoPath = testdir + "/fetch_test";

    try {
        repo.open(repoPath);
        repo.fetch("kde");
    }
    catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }

    // TODO verify remote branches with unit test
}


void TestFetch::remoteBranches()
{
    LibQGit2::Repository repo;

    const QString repoPath = testdir + "/remote_ls";

    QVERIFY(removeDir(repoPath));
    sleep::ms(500);

    QStringList heads;
    try {
        repo.init(repoPath);
        repo.remoteAdd("kde", "http://anongit.kde.org/libqgit2");
        heads = repo.remoteBranches("kde");
    }
    catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }

    qDebug() << "heads" << heads;

    QVERIFY(!heads.isEmpty());
    QVERIFY(heads.size() > 1);
    QVERIFY(heads.contains("master"));
}


QTEST_MAIN(TestFetch);

#include "Fetch.moc"
