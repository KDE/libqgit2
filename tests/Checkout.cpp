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
#include <QFile>

using namespace LibQGit2;


class TestCheckout : public TestBase
{
    Q_OBJECT

private slots:
    void checkoutRemote();
    void checkoutRemoteKde();
    void checkoutCommitAsTree();
    void checkoutHead();
    void checkoutPaths();

private:
    void fetch(const QString& branch, const QString& repoPath, const QString& remote);
};


void TestCheckout::fetch(const QString& branch, const QString& repoPath, const QString& remote = "origin")
{
    try {
        Repository repo;
        repo.init(repoPath);
        repo.remoteAdd(remote, HttpRemoteUrl);
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
        repo.checkoutRemote("master", CheckoutOptions(), "kde");
    }
    catch (const Exception& ex) {
        QFAIL(ex.what());
    }
}


void TestCheckout::checkoutCommitAsTree()
{
    Repository repo;
    try {
        repo.clone(FileRepositoryUrl, testdir);
        OId id = OId::stringToOid("127c9e7d17");  // 127c9e7d17 is a commit where CMakeLists.txt was modified
        repo.checkoutTree(repo.lookupCommit(id), CheckoutOptions(CheckoutOptions::Safe));
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

void TestCheckout::checkoutHead()
{
    const QString fileName(testdir + "/CMakeLists.txt");

    Repository repo;
    try {
        repo.clone(FileRepositoryUrl, testdir);
        QVERIFY(QFile::remove(fileName));
        repo.checkoutHead(CheckoutOptions(CheckoutOptions::Force));
    } catch (const Exception& ex) {
        QFAIL(ex.what());
    }

    QVERIFY(QFile::exists(fileName));
}

void TestCheckout::checkoutPaths()
{
    const QStringList paths("CMakeLists.txt");
    Repository repo;
    try {
        repo.clone(FileRepositoryUrl, testdir);
        OId id = OId::stringToOid("7752cf5867");  // 7752cf5867 is a commit where many files were modified
        CheckoutOptions opts(CheckoutOptions::Safe);
        opts.setPaths(paths);
        repo.checkoutTree(repo.lookupCommit(id), opts);
    } catch (const Exception& ex) {
        QFAIL(ex.what());
    }

    StatusList status = repo.status(StatusOptions(StatusOptions::ShowOnlyIndex, StatusOptions::ExcludeSubmodules));
    QStringList checkedoutPaths;
    for (size_t i = 0; i < status.entryCount(); ++i) {
        const StatusEntry entry = status.entryByIndex(i);
        checkedoutPaths << entry.headToIndex().newFile().path();
    }

    QCOMPARE(checkedoutPaths, paths);
}


QTEST_MAIN(TestCheckout)

#include "Checkout.moc"

