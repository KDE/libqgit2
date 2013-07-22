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

#include "git2.h"

#include "libqgit2_export.h"

namespace LibQGit2
{
    class QGitRepository;
    class QGitOId;
    class QGitCommit;

    /**
      * @brief Wrapper class for git_revwalk.
      * The revision walker can be used to traverse Git commit history. It features sorting abilities and more.
      *
      * @ingroup LibQGit2
      * @{
      */
    class LIBQGIT2_REVWALK_EXPORT QGitRevWalk
    {
        public:
            /**
             * Defines the sort mode when walking revisions.
             */
            enum SortMode
            {
                None = GIT_SORT_NONE,
                Time = GIT_SORT_TIME,
                Topological = GIT_SORT_TOPOLOGICAL,
                Reverse = GIT_SORT_REVERSE
            };
            
            Q_DECLARE_FLAGS(SortModes, SortMode) //!< Combination of SortMode

            /**
             * Allocate a new revision walker to iterate through a repo.
             *
             * @param repo the repo to walk through
             */
            explicit QGitRevWalk(const QGitRepository& repository);

            QGitRevWalk( const QGitRevWalk& other );

            /**
             * Delete a revwalk previously allocated.
             */
            ~QGitRevWalk();

            /**
             * Reset the walking machinery for reuse.
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
            int hide(const QGitOId& commit) const;

            /**
             * Get the oid of the next commit from the revision traversal.
             */
            LibQGit2::QGitOId next() const;

            /**
             * Get the next commit from the revision traversal and look it up in the owner repository.
             * @param commit The next commit within the set repository, when it was found;
             * Otherwise an empty QGitCommit will be set.
             * @return True when the commit was found.
             */
            bool next(QGitCommit & commit);

            /**
             * Change the sorting mode when iterating through the
             * repository's contents.
             * Changing the sorting mode resets the walker.
             * @param sortMode The sorting mode @see SortModes.
             */
            void setSorting(SortModes sortMode);

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

    Q_DECLARE_OPERATORS_FOR_FLAGS(QGitRevWalk::SortModes)

    /**@}*/
}

#endif // LIBQGIT2_REVWALK_H
