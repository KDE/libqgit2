/******************************************************************************
 * This file is part of the Gluon Development Platform
 * Copyright (c) 2011 Laszlo Papp <djszapi@archlinux.us>
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

#ifndef LIBQGIT2_REVWALK_H
#define LIBQGIT2_REVWALK_H

#include "libqgit2_export.h"

#include <git2/revwalk.h>

namespace LibQGit2
{
    class QGitRepository;
    class QGitCommit;
    class LIBQGIT2_REVWALK_EXPORT QGitRevWalk
    {
        public:
            /**
             * Allocate a new revision walker to iterate through a repo.
             *
             * @param walker pointer to the new revision walker
             * @param repo the repo to walk through
             * @return 0 on success; error code otherwise
             */
            explicit QGitRevWalk(const QGitRepository& repository);

            QGitRevWalk( const QGitRevWalk& other );

            /**
             * Delete a revwalk previously allocated.
             */
            ~QGitRevWalk();

        public:

            /**
             * Reset the walking machinery for reuse.
             * @param walker handle to reset.
             */
            void reset() const;

            /**
             * Mark a commit to start traversal from.
             * The commit object must belong to the repo which is being walked through.
             *
             * @param commit the commit to start from.
             */
            int push(const QGitCommit& commit) const;

            /**
             * Mark a commit (and its ancestors) uninteresting for the output.
             * @param commit the commit that will be ignored during the traversal
             */
            int hide(const QGitCommit& commit) const;

            /**
             * Get the next commit from the revision traversal.
             *
             * @param commit Pointer where to store the next commit
             * @return GIT_SUCCESS if the next commit was found;
             * GIT_EREVWALKOVER if there are no commits left to iterate
             */
            int next(QGitCommit& commit);

            /**
             * Change the sorting mode when iterating through the
             * repository's contents.
             * Changing the sorting mode resets the walker.
             * @param sortMode combination of GIT_RPSORT_XXX flags
             */
            int sorting(unsigned int sortMode);

            /**
             * Return the repository on which this walker
             * is operating.
             *
             * @return the repository being walked
             */
            QGitRepository repository();

            git_revwalk* data() const;
            const git_revwalk* constData() const;

        private:
            git_revwalk *m_revWalk;
    };
}

#endif // LIBQGIT2_REVWALK_H
