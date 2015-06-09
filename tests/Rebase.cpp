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

#include "qgitindex.h"
#include "qgitrebase.h"
#include "qgitremote.h"
#include "qgitrepository.h"
#include "qgitrevwalk.h"
#include "qgitdiff.h"
#include "qgitdiffdelta.h"
#include "qgittree.h"

#include <QFile>
#include <QFileInfo>
#include <QPointer>

using namespace LibQGit2;

class TestRebase : public TestBase
{
    Q_OBJECT
public:
    TestRebase();

private slots:
    void init();
    void cleanup();

    void TestRebasingMasterOntoAnotherBranchProducesCorrectTopology();

private:
    QSharedPointer<Repository> repo;
    Signature sig;

    OId commitIndexToRef(const QString &refSpec);
    void writeToIndex(const QString &path, const QString &text);
};

TestRebase::TestRebase()
    : TestBase(),
      sig("unknown", "unknown")
{
}

void TestRebase::init()
{
    TestBase::init();
    initTestRepo();
    repo = QSharedPointer<Repository>(new Repository);
    repo->open(testdir + "/.git");
}

void TestRebase::cleanup()
{
    repo.clear();
    TestBase::cleanup();
}

OId TestRebase::commitIndexToRef(const QString &refSpec)
{
    Tree index = repo->lookupTree(repo->index().createTree());
    QList<Commit> parents;
    parents.append(repo->lookupCommit(repo->head().target()));
    return repo->createCommit(index, parents, sig, sig, "commit", refSpec);
}

void TestRebase::writeToIndex(const QString &path, const QString &text)
{
    QFile file(path);
    file.open(QFile::ReadWrite);
    file.write(text.toUtf8());
    file.close();
    repo->index().addByPath(QFileInfo(path).fileName());
    repo->index().write();
}

void TestRebase::TestRebasingMasterOntoAnotherBranchProducesCorrectTopology()
{
    // set up the repository
    QString refSpecOntoBranch("refs/heads/onto");
    repo->createBranch("onto");
    QString pathOntoBranchFile(testdir + "/onto.txt");
    writeToIndex(pathOntoBranchFile, "testing");
    OId oidOntoCommit = commitIndexToRef(refSpecOntoBranch);

    repo->checkoutHead(CheckoutOptions(CheckoutOptions::Force));

    QString pathMasterBranchFile(testdir + "/master.txt");
    writeToIndex(pathMasterBranchFile, "testing testing");
    commitIndexToRef("HEAD");

    // rebase master onto another branch
    Reference refOnto = repo->lookupRef(refSpecOntoBranch);
    Reference refMaster = repo->lookupRef("refs/heads/master");
    Reference refUpstream = repo->lookupRef("refs/remotes/origin/master");
    Rebase rebase = repo->rebase(refMaster, refUpstream, refOnto, RebaseOptions(CheckoutOptions(CheckoutOptions::SafeCreate)), sig);
    rebase.next();
    OId oidRebasedMaster = rebase.commit(sig, sig, QString());
    rebase.finish(sig);

    // check results
    RevWalk walk(*repo);
    walk.setSorting(RevWalk::Topological);
    walk.pushHead();
    OId oid;
    QVERIFY(walk.next(oid));
    QCOMPARE(oidRebasedMaster, oid);
    QVERIFY(walk.next(oid));
    QCOMPARE(oidOntoCommit, oid);
    QVERIFY(walk.next(oid));
    QCOMPARE(refUpstream.target(), oid);
}


QTEST_MAIN(TestRebase);

#include "Rebase.moc"
