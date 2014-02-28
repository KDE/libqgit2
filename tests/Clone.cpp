#include "TestHelpers.h"

#include <QCoreApplication>
#include <QTimer>


#include <iostream>
#include <bitset>

#include "qgitcommit.h"
#include "qgitrepository.h"

using namespace LibQGit2;


class TestClone : public QObject
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

private:
    int m_clone_progress;
    const QString testdir;

    void clone(const QString& url);
};



TestClone::TestClone() : testdir(VALUE_TO_STR(TEST_DIR))
{
}


void TestClone::clone(const QString& url)
{
    LibQGit2::Repository repo;
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
    catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }

    QCOMPARE(m_clone_progress, 100);
}


void TestClone::fileProtocol()
{
    clone("file:///" + VALUE_TO_QSTR(TEST_EXISTING_REPOSITORY) + "/.git");
}


void TestClone::gitProtocol()
{
    clone("git://anongit.kde.org/libqgit2");
}


void TestClone::httpProtocol()
{
    clone("http://anongit.kde.org/libqgit2");
}


void TestClone::httpsProtocol()
{
    clone("https://github.com/lgiordani/libqgit2.git");
}


QTEST_MAIN(TestClone);

#include "Clone.moc"
