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

struct git_revwalk;

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
    class LIBQGIT2_EXPORT QGitRevWalk
    {
        public:
            /**
             * Defines the sortmode when walking revisions.
             */
            enum SortMode
            {
                None            = 0x0   //!< GIT_SORT_NONE
                , Time          = 0x1   //!< GIT_SORT_TIME
                , Topological   = 0x2   //!< GIT_SORT_TOPOLOGICAL
                , Reverse       = 0x4   //!< GIT_SORT_REVERSE
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

        public:

            /**
             * Reset the walking machinery for reuse.
             */
            void reset() const;

            /**
             * Push a commit to start traversal from.
             * The commit object must belong to the repo which is being walked through.
             *
             * @param commit the commit to start from.
             *
             * @return the libgit2 error code
             *
             * @see git_revwalk_push
             */
            int push(const QGitCommit& commit) const;

            /**
              * Pushes all commit id's matched by the glob pattern for traversal.
              * The glob pattern is assumed to start with "refs/".
              *
              * @param The glob pattern according to @see git_revwalk_push_glob
              *
              * @return the libgit2 error code
              *
              * @see git_revwalk_push_glob
              */
            int pushGlob(const QString &glob) const;

            /**
              * Pushes the repository's head commit id for traversal.
              *
              * @return the libgit2 error code
              *
              * @see git_revwalk_push_head
              */
            int pushHead() const;

            /**
              * Pushes the commit id of the matched reference for traversal.
              *
              * @param the reference name
              *
              * @return the libgit2 error code
              *
              * @see git_revwalk_push_ref
              */
            int pushRef(const QString &ref) const;

            /**
             * Mark a commit (and its ancestors) uninteresting for the output.
             *
             * @param commit the commit that will be ignored during the traversal
             *
             * @return the libgit2 error code
             */
            int hide(const QGitOId& commit) const;

            /**
              * Hides all commit id's and their parents matched by the glob pattern during traversal.
              * The glob pattern is assumed to start with "refs/".
              *
              * @param The glob pattern according to @see git_revwalk_hide_glob
              *
              * @return the libgit2 error code
              *
              * @see git_revwalk_hide_glob
              */
            int hideGlob(const QString &glob) const;

            /**
              * Hides the repository's head commit and its parents during traversal.
              *
              * @return the libgit2 error code
              *
              * @see git_revwalk_hide_head
              */
            int hideHead() const;

            /**
              * Hides the commit and its parents of the matched reference during traversal.
              *
              * @param the reference name
              *
              * @return the libgit2 error code
              *
              * @see git_revwalk_hide_ref
              */
            int hideRef(const QString &ref) const;

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
