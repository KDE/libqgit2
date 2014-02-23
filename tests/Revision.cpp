// A try to port libgit2 status.c example to libqgit2


#include "TestHelpers.h"

#include <QCoreApplication>
#include <QTimer>
#include <iostream>

#include <bitset>

#include "qgitcommit.h"
#include "qgitrepository.h"
#include "qgitrevwalk.h"


using namespace LibQGit2;


class TestRevision : public QObject
{
    Q_OBJECT

public:
    TestRevision();
    ~TestRevision();

private slots:

    void initTestCase() {}

    void create();
    void open();
    void revwalk();

private:
    LibQGit2::Repository *repo;
};



TestRevision::TestRevision() : repo(0)
{
}


void TestRevision::create()
{
    QVERIFY(!repo);

    // Create a new repository object
    repo = new LibQGit2::Repository();

    QVERIFY(repo);
}


void TestRevision::open()
{
    try {
        // Open a local fixed path
        repo->open(QString(VALUE_TO_STR(TEST_EXISTING_REPOSITORY)));
    } catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }
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

    } catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }
}


TestRevision::~TestRevision()
{
    delete repo;
}


QTEST_MAIN(TestRevision);

#include "Revision.moc"
