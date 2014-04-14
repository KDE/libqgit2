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

#include "qgitrepository.h"
#include "qgitcommit.h"
#include "qgitdiffdelta.h"
#include "qgitdifffile.h"

using namespace LibQGit2;


class TestCheckout : public QObject
{
    Q_OBJECT

private slots:
    void init();

    void checkoutRemote();
    void checkoutRemoteKde();
    void checkoutCommitAsTree();

private:
    QString testdir;

    void fetch(const QString& branch, const QString& repoPath, const QString& remote);
};


void TestCheckout::init()
{
    testdir = VALUE_TO_QSTR(TEST_DIR) + "/checkout_test/" + QTest::currentTestFunction();
    QVERIFY(removeDir(testdir));
    sleep::ms(500);
}

void TestCheckout::fetch(const QString& branch, const QString& repoPath, const QString& remote = "origin")
{
    try {
        Repository repo;
        repo.init(repoPath);
        repo.remoteAdd(remote, "http://anongit.kde.org/libqgit2");
        repo.fetch(remote, branch);
    }
    catch (const Exception& ex) {
        QFAIL(ex.what());
    }
}


void TestCheckout::checkoutRemote()
{
    fetch("master", testdir);

    try {
        Repository repo;
        repo.open(testdir);
        repo.checkoutRemote("master");
    }
    catch (const Exception& ex) {
        QFAIL(ex.what());
    }
}


void TestCheckout::checkoutRemoteKde()
{
    fetch("master", testdir, "kde");

    try {
        Repository repo;
        repo.open(testdir);
        repo.checkoutRemote("master", false, "kde");
    }
    catch (const Exception& ex) {
        QFAIL(ex.what());
    }
}


void TestCheckout::checkoutCommitAsTree()
{
    const QString existingRepoPath = "file://" + VALUE_TO_QSTR(TEST_EXISTING_REPOSITORY) + "/.git";

    Repository repo;
    try {
        repo.clone(existingRepoPath, testdir);
        OId id = OId::stringToOid("127c9e7d17");  // 127c9e7d17 is a commit where CMakeLists.txt was modified
        repo.checkoutTree(repo.lookupCommit(id));
    } catch (const Exception& ex) {
        QFAIL(ex.what());
    }

    StatusList status = repo.status(StatusOptions(StatusOptions::ShowOnlyIndex, StatusOptions::ExcludeSubmodules));
    bool found = false;
    for (size_t i = 0; i < status.entryCount(); ++i) {
        const StatusEntry entry = status.entryByIndex(i);
        if ((found = (entry.headToIndex().newFile().path() == "CMakeLists.txt"))) {
            break;
        }
    }
    QVERIFY2(found, "Expected path was not part of the checked out commit");
}


QTEST_MAIN(TestCheckout)

#include "Checkout.moc"

