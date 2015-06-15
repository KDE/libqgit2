/******************************************************************************
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

#include "qgitrepository.h"
#include "qgitremote.h"

#include <QPointer>
#include <QDir>

using namespace LibQGit2;

class TestRepository : public TestBase
{
    Q_OBJECT
public:
    TestRepository() :
        branchName("new_branch")
    {}

private slots:
    virtual void init();
    virtual void cleanup();

    void testRemoteUrlChanging();
    void testLookingUpRevision();
    void testCreateBranch();
    void testDeleteBranch();
    void testShouldIgnore();

private:
    const QString branchName;
    QPointer<Repository> repo;
};

void TestRepository::init()
{
    TestBase::init();

    QVERIFY(!repo);
    repo = new Repository;
    QVERIFY(repo);
}

void TestRepository::cleanup()
{
    QVERIFY(repo);
    delete repo;
    QVERIFY(!repo);

    TestBase::cleanup();
}

void TestRepository::testRemoteUrlChanging()
{
    repo->init(testdir);

    const QString remoteName("origin");
    repo->remoteAdd(remoteName, HttpRemoteUrl);
    repo->remoteAdd(remoteName, GitRemoteUrl, true);

    QScopedPointer<Remote> remote(repo->remote(remoteName));
    QCOMPARE(remote->url(), GitRemoteUrl);
}

void TestRepository::testLookingUpRevision()
{
    repo->open(ExistingRepository);

    Object object = repo->lookupRevision("HEAD^{tree}");
    QCOMPARE(Object::TreeType, object.type());
}

void TestRepository::testCreateBranch()
{
    initTestRepo();
    repo->open(testdir);

    OId head = repo->head().target();
    QCOMPARE(repo->createBranch(branchName).target(), head);
    QCOMPARE(repo->lookupShorthandRef(branchName).target(), head);
}

void TestRepository::testDeleteBranch()
{
    initTestRepo();
    repo->open(testdir);
    repo->createBranch(branchName);

    repo->deleteBranch(branchName);
    EXPECT_THROW(repo->lookupShorthandRef(branchName), Exception);
}

void TestRepository::testShouldIgnore()
{
    initTestRepo();
    repo->open(testdir);

    const QString ignoredFileName("Makefile");
    const QString includedFileName("notignored.txt");

    // Relative paths
    QVERIFY(repo->shouldIgnore(ignoredFileName));
    QVERIFY(!repo->shouldIgnore(includedFileName));

    QDir dir(testdir);
    QString testDirName(dir.dirName());

    // Absolute paths
    QVERIFY(repo->shouldIgnore(dir.absoluteFilePath(ignoredFileName)));
    QVERIFY(!repo->shouldIgnore(dir.absoluteFilePath(includedFileName)));

    dir.cdUp();

    // Path containing .. but still leading up to the repository
    QVERIFY(!repo->shouldIgnore(dir.absoluteFilePath(dir.absolutePath() + "/../" + dir.dirName() + "/" + testDirName + "/" + includedFileName)));

    // Absolute path outside the repository
    EXPECT_THROW(repo->shouldIgnore(dir.absoluteFilePath(ignoredFileName)), Exception);
}

QTEST_MAIN(TestRepository)

#include "Repository.moc"
