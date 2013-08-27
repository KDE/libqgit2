// A try to port libgit2 status.c example to libqgit2

#include <QTest>

#include <QCoreApplication>
#include <QTimer>
#include <iostream>

#include <bitset>

#include "qgitcommit.h"
#include "qgitrepository.h"
#include "qgitrevwalk.h"

class TestRevision : public QObject
{

    Q_OBJECT

public:
    TestRevision();
    ~TestRevision();

private:
    LibQGit2::Repository *repo;
};


using namespace LibQGit2;

TestRevision::TestRevision()
{
    // Create a new repository object
    Repository * repo = new LibQGit2::Repository();

    // Open a local fixed path
    repo->open(QString("/home/leo/projects/libqgit2"));

    RevWalk * rw = new RevWalk(*repo);

    rw->setSorting(RevWalk::Topological);

    rw->pushHead();

    Commit commit;
    while(rw->next(commit)) {
        QByteArray qb = commit.oid().format();
        std::cout << qb.data() << std::endl;
    }

}

TestRevision::~TestRevision()
{}


QTEST_MAIN(TestRevision);

#include "Revision.moc"
