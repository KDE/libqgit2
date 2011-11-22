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

#include "qgitobject.h"

#include <QtCore/QDateTime>

struct git_commit;

namespace LibQGit2
{
    class QGitOId;
    class QGitSignature;
    class QGitTree;
    class QGitRepository;

    class LIBQGIT2_COMMIT_EXPORT QGitCommit : public QGitObject
    {
        public:

            /**
             * Creates a QGitCommit that points to commit. The pointer object becomes managed by
             * this QGitCommit, and must not be passed to another QGitCommit or closed outside this
             * object.
             */
            explicit QGitCommit(git_commit *commit = 0);

            /**
             * Copy constructor; creates a copy of the object, sharing the same underlaying data
             * structure.
             */
            QGitCommit(const QGitCommit& other);

            /**
             * Destroys the object.
             */
            ~QGitCommit();

            /**
            * Get the id of a commit.
            * @return object identity for the commit.
            */
            QGitOId oid() const;

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
            QGitSignature committer() const;


            /**
            * Get the committer of a commit.
            * @return the committer of a commit
            */
            QGitSignature author() const;

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
    };
}

#endif // LIBQGIT2_HASHTABLE_H
