#include "annotatedcommit.h"
#include "git2.h"
#include "qgitexception.h"

namespace LibQGit2
{
namespace internal
{

inline static void no_op(git_annotated_commit *) {}

inline git_annotated_commit *gitAnnotatedCommitFromRef(Repository &repo, const Reference &ref)
{
    git_annotated_commit *commit = 0;
    if (!ref.isNull()) {
        qGitThrow(git_annotated_commit_from_ref(&commit, repo.data(), ref.constData()));
    }
    return commit;
}

AnnotatedCommit::AnnotatedCommit(git_annotated_commit *annotated_commit)
    : d(annotated_commit, git_annotated_commit_free)
{
}

AnnotatedCommit::AnnotatedCommit(Repository &repo, const Reference &ref)
    : d(gitAnnotatedCommitFromRef(repo, ref), ref.isNull() ? no_op : git_annotated_commit_free)
{
}

AnnotatedCommit::AnnotatedCommit(const AnnotatedCommit &other)
    : d(other.d)
{
}

const git_annotated_commit *AnnotatedCommit::constData() const
{
    return d.data();
}
}
}
