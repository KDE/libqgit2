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

using namespace LibQGit2;

class TestRepository : public QObject
{
    Q_OBJECT

private slots:
    void init();

    void testRemoteUrlChanging();
    void testLookingUpRevision();

private:
    QString testdir;
    Repository repo;
};

void TestRepository::init()
{
    testdir = VALUE_TO_QSTR(TEST_DIR) + "/repository_test/" + QTest::currentTestFunction();
    QVERIFY(removeDir(testdir));
    sleep::ms(500);
}


void TestRepository::testRemoteUrlChanging()
{
    repo.init(testdir);

    const QString remoteName("origin");
    repo.remoteAdd(remoteName, HttpRemoteUrl);
    repo.remoteAdd(remoteName, GitRemoteUrl, true);

    QScopedPointer<Remote> remote(repo.remote(remoteName));
    QCOMPARE(remote->url(), GitRemoteUrl);
}

void TestRepository::testLookingUpRevision()
{
    repo.open(ExistingRepository);

    Object object = repo.lookupRevision("HEAD^{tree}");
    QCOMPARE(Object::TreeType, object.type());
}

QTEST_MAIN(TestRepository)

#include "Repository.moc"
