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

#ifndef LIBQGIT2_HASHTABLE_H
#define LIBQGIT2_HASHTABLE_H

#include "libqgit2_export.h"

#include <QtCore/QDateTime>

#include <git2/commit.h>

namespace LibQGit2
{
    class QGitOId;
    class QGitSignature;
    class QGitSignatureRef;
    class QGitTree;
    class QGitRepository;

    class LIBQGIT2_COMMIT_EXPORT QGitCommit
    {
        public:

            explicit QGitCommit(git_commit *commit = 0);

            QGitCommit( const QGitCommit& other );

            ~QGitCommit();

            void reset(git_commit *commit = 0);

        public:

            /**
             * Lookup a commit object from a repository.
             * The generated commit object is owned by the revision
             * repo and shall not be freed by the user.
             *
             * @param repo the repo to use when locating the commit.
             * @param id identity of the commit to locate. If the object is
             * an annotated tag it will be peeled back to the commit.
             * @return 0 on success; error code otherwise
             */
            int lookup(const QGitRepository& repository, const QGitOId& oid);

            /**
            * Get the id of a commit.
            * @return object identity for the commit.
            */
            QGitOId id() const;

            /**
             * Get the full message of a commit.
             * @return the message of a commit
             */
            QString message() const;

            /**
             * Get the commit time (i.e. committer time) of a commit.
             * @return the time of a commit
             */
            QDateTime dateTime() const;

            /**
            * Get the commit timezone offset (i.e. committer's preferred timezone) of a commit.
            * @return positive or negative timezone offset, in minutes from UTC
            */
            int timeOffset() const;


            /**
            * Get the committer of a commit.
            * @return the committer of a commit
            */
            QGitSignatureRef committer() const;


            /**
            * Get the committer of a commit.
            * @return the committer of a commit
            */
            QGitSignatureRef author() const;

            /**
             * Get the tree pointed to by a commit.
             * @return the tree of a commit
             */
            QGitTree tree() const;

            /**
             * Get the number of parents of this commit
             *
             * @return integer of count of parents
             */
            unsigned int parentCount() const;

            /**
             * Get the specified parent of the commit.
             * @param n the position of the entry
             * @return a pointer to the commit; NULL if out of bounds
             */
            QGitCommit parent(unsigned n) const;

            git_commit* data() const;
            const git_commit* constData() const;

        private:
            git_commit *m_commit;
    };
}

#endif // LIBQGIT2_HASHTABLE_H
