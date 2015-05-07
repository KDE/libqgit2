/******************************************************************************
 * This file is part of the libqgit2 library
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

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
