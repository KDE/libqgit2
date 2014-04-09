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

#include "qgitrepository.h"
#include "qgitconfig.h"
#include "qgitpush.h"

using namespace LibQGit2;


class TestPush : public QObject
{
    Q_OBJECT

public:
    TestPush() :
        testdir(VALUE_TO_QSTR(TEST_DIR) + "/push_test/"),
        existingRepo(VALUE_TO_QSTR(TEST_EXISTING_REPOSITORY) + "/.git"),
        existingBareRepo(VALUE_TO_QSTR(TEST_EXISTING_REPOSITORY) + "_bare")
    {}

private slots:
    void initTestCase();
    void pushToNewTargetBranch();

private:
    const QString testdir;
    const QString existingRepo;
    const QString existingBareRepo;

    void initBareLocalRepo();
};


void TestPush::initTestCase()
{
    initBareLocalRepo();
}

void TestPush::initBareLocalRepo()
{
    removeDir(existingBareRepo);
    copyDir(existingRepo, existingBareRepo);

    Repository repo;
    repo.open(existingBareRepo);
    Config cfg = repo.configuration();
    cfg.setValue("core.bare", "true");
}


void TestPush::pushToNewTargetBranch()
{
    const QString repoPath = testdir + "push_new_target_branch";
    QVERIFY(removeDir(repoPath));

    const QString targetBranch("push_new_target_branch");

    Repository repo;
    try {
        repo.clone(existingBareRepo, repoPath);
        Push push = repo.push("origin");
        push.addRefSpec("refs/heads/master:refs/heads/" + targetBranch);
        push.execute();
    }
    catch (const LibQGit2::Exception& ex) {
        QFAIL(ex.what());
    }

    QVERIFY(repo.remoteBranches("origin").contains(targetBranch));
}

QTEST_MAIN(TestPush)

#include "Push.moc"
