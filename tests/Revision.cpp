// A try to port libgit2 status.c example to libqgit2


#include "TestHelpers.h"

#include <QCoreApplication>
#include <QPointer>
#include <QTimer>
#include <iostream>

#include <bitset>

#include "qgitcommit.h"
#include "qgitrepository.h"
#include "qgitrevwalk.h"


using namespace LibQGit2;


class TestRevision : public TestBase
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();

    void revwalk();

private:
    QPointer<Repository> repo;
};

void TestRevision::init()
{
    TestBase::init();

    QVERIFY(!repo);

    // Create a new repository object
    repo = new Repository();

    QVERIFY(repo);

    try {
        // Open a local fixed path
        repo->open(ExistingRepository);
    } catch (const Exception& ex) {
        QFAIL(ex.what());
    }
}

void TestRevision::cleanup()
{
    QVERIFY(repo);
    delete repo;
    QVERIFY(!repo);

    TestBase::cleanup();
}

void TestRevision::revwalk()
{
    try {

        RevWalk rw(*repo);

        rw.setSorting(RevWalk::Topological);

        rw.pushHead();

        Commit commit;
        while(rw.next(commit)) {
            QByteArray qb = commit.oid().format();
            std::cout << qb.data() << std::endl;
        }

    } catch (const Exception& ex) {
        QFAIL(ex.what());
    }
}

QTEST_MAIN(TestRevision);

#include "Revision.moc"
