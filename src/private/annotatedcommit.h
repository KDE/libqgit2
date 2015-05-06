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

#ifndef LIBQGIT2_ANNOTATEDCOMMIT_H
#define LIBQGIT2_ANNOTATEDCOMMIT_H

#include "qgitref.h"
#include "qgitrepository.h"
#include "git2.h"
#include <QSharedPointer>

namespace LibQGit2
{
namespace internal
{
class AnnotatedCommit
{
public:
    explicit AnnotatedCommit(git_annotated_commit *annotated_commit);
    explicit AnnotatedCommit(Repository &repo, const Reference &ref);
    AnnotatedCommit(const AnnotatedCommit &other);

    const git_annotated_commit *constData() const;
private:
    QSharedPointer<git_annotated_commit> d;
};
}
}
#endif // LIBQGIT2_ANNOTATEDCOMMIT_H
