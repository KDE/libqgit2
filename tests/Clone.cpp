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
#include "qgitcredentials.h"

#include "TestHelpers.h"

using namespace LibQGit2;


class TestClone : public TestBase
{
    Q_OBJECT

public:
    TestClone();

public slots:
    void cloneProgress(int p) 
    { 
        m_clone_progress = p;
        if (p % 20 == 0) {
            qDebug() << qPrintable(QString("Progress : %1%").arg(p));
        }
    }

private slots:
    void fileProtocol();
    void gitProtocol();
    void httpProtocol();
    void httpsProtocol();
    void sshProtocol();

private:
    int m_clone_progress;
    const QString testdir;

    void clone(const QString& url, const Credentials &credentials = Credentials());
};



TestClone::TestClone() : testdir(VALUE_TO_STR(TEST_DIR))
{
}

void TestClone::clone(const QString& url, const Credentials &credentials)
{
    Repository repo;
    repo.setRemoteCredentials("origin", credentials);
    connect(&repo, SIGNAL(cloneProgress(int)), this, SLOT(cloneProgress(int)));

    QString dirname = url;
    dirname.replace(":", "");
    dirname.replace("//", "/");
    dirname.replace("//", "/");
    dirname.replace("/", "_");
    dirname.replace(".", "_");
    const QString repoPath = testdir + "/clone_test/" + dirname;

    removeDir(repoPath);
    sleep::ms(500);
    m_clone_progress = 0;

    qDebug() << "Cloning " << url;
    try {
        repo.clone(url, repoPath);
    }
    catch (const Exception& ex) {
        QFAIL(ex.what());
    }

    QCOMPARE(m_clone_progress, 100);
}


void TestClone::fileProtocol()
{
    clone(FileRepositoryUrl);
}


void TestClone::gitProtocol()
{
    clone(GitRemoteUrl);
}


void TestClone::httpProtocol()
{
    clone(HttpRemoteUrl);
}


void TestClone::httpsProtocol()
{
    clone(HttpsRemoteUrl);
}


void TestClone::sshProtocol()
{
    if (!libgit2HasSSH()) {
        SKIPTEST("libgit2 is not compiled with SSH support. Skipping SSH clone test.");
    }

    clone(SshRemoteUrl, Credentials::ssh("libqgit2_id_rsa", "libqgit2_id_rsa.pub", "git"));
}


QTEST_MAIN(TestClone)

#include "Clone.moc"
