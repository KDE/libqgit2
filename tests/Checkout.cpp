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


using namespace LibQGit2;


class TestCheckout : public QObject
{
    Q_OBJECT

public:
    TestCheckout() : testdir(VALUE_TO_STR(TEST_DIR)) {}

public slots:

private slots:
    void checkoutRemote();
    void checkoutRemoteKde();

private:
    const QString testdir;

    void fetch(const QString& branch, const QString& repoPath, const QString& remote);
};


void TestCheckout::fetch(const QString& branch, const QString& repoPath, const QString& remote = "origin")
{
    QVERIFY(removeDir(repoPath));
    sleep::ms(500);

    try {
        LibQGit2::Repository repo;
        repo.init(repoPath);
        repo.remoteAdd(remote, "http://anongit.kde.org/libqgit2");
        repo.fetch(remote, branch);
    }
    catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }
}


void TestCheckout::checkoutRemote()
{
    const QString repoPath = testdir + "/checkout_test/checkout_remote";

    fetch("master", repoPath);

    try {
        LibQGit2::Repository repo;
        repo.open(repoPath);
        repo.checkoutRemote("master");
    }
    catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }
}


void TestCheckout::checkoutRemoteKde()
{
    const QString repoPath = testdir + "/checkout_test/checkout_remote_kde";

    fetch("master", repoPath, "kde");

    try {
        LibQGit2::Repository repo;
        repo.open(repoPath);
        repo.checkoutRemote("master", false, "kde");
    }
    catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }
}

QTEST_MAIN(TestCheckout);

#include "Checkout.moc"

