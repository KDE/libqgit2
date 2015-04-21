
#include "TestHelpers.h"

#include "qgitrepository.h"
#include <QDir>
#include <QPointer>


using namespace LibQGit2;


class TestInit : public TestBase
{
    Q_OBJECT

public:
    TestInit();
    ~TestInit();

private slots:

    void initTestCase();
    void cleanupTestCase();

    void init();
    void initBare();

private:
    QPointer<Repository> repo;

    const QString repoPath;
};



TestInit::TestInit() :
    repo(0),
    repoPath(QString(VALUE_TO_STR(TEST_DIR)) + "/repo")
{
}


void TestInit::initTestCase()
{
    TestBase::initTestCase();

    QVERIFY(!repo);

    // Create a new repository object
    repo = new LibQGit2::Repository();

    QVERIFY(repo);
}

void TestInit::cleanupTestCase()
{
    QVERIFY(repo);
    delete repo;
    QVERIFY(!repo);

    TestBase::cleanupTestCase();
}

void TestInit::init()
{
    QVERIFY(removeDir(repoPath));
    QVERIFY(!QDir(repoPath).exists());

    QDir().mkdir(repoPath);
    QVERIFY(QDir(repoPath).exists());

    try {
        repo->init(repoPath, false);
    } catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }

    QVERIFY(QDir(repoPath).exists());
    QVERIFY(QFile(repoPath + "/.git").exists());
    QVERIFY(QFile(repoPath + "/.git/HEAD").exists());
}


void TestInit::initBare()
{
    QVERIFY(removeDir(repoPath));
    QVERIFY(!QDir(repoPath).exists());

    QDir().mkdir(repoPath);
    QVERIFY(QDir(repoPath).exists());

    try {
        repo->init(repoPath, true);
    } catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }

    QVERIFY(QDir(repoPath).exists());
    QVERIFY(!QFile(repoPath + "/.git").exists());
    QVERIFY(QFile(repoPath + "/HEAD").exists());
}


TestInit::~TestInit()
{
    delete repo;
}


QTEST_MAIN(TestInit);

#include "Init.moc"
