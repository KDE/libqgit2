/******************************************************************************
 * This file is part of the libqgit2 library
 * Copyright (c) 2011 Laszlo Papp <djszapi@archlinux.us>
 * Copyright (C) 2013 Leonardo Giordani
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

namespace LibQGit2
{
    class QGitOId;
    class QGitSignature;
    class QGitTree;
    class QGitRepository;

    /**
     * @brief Wrapper class for git_commit.
     * Represents a Git commit object.
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_COMMIT_EXPORT Commit : public Object
    {
        public:

            /**
             * Creates a Commit that points to commit. The pointer object becomes managed by
             * this Commit, and must not be passed to another Commit or closed outside this
             * object.
             */
            explicit Commit(git_commit *commit = 0);

            /**
             * Copy constructor; creates a copy of the object, sharing the same underlaying data
             * structure.
             */
            Commit(const Commit& other);

            /**
             * Destroys the object.
             */
            ~Commit();

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
             * Get the first part of the commit message (similar to git log --oneline).
             * The string is further cut when a linebreak is found.
             * @param maxLen maximal length of the resulting string. Default is 80 characters.
             * @return the short message
             */
            QString shortMessage(int maxLen = 80) const;

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
             * @throws QGitException
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
             * @return the parent commit or an empty commit, when there is no parent
             * @throws QGitException
             */
            Commit parent(unsigned n) const;

            /**
             * Get the object id of the specified parent of the commit.
             * @param n the position of the entry
             * @return the parent's object id or an empty id, when there is no parent
             */
            QGitOId parentId(unsigned n) const;

            git_commit* data() const;
            const git_commit* constData() const;
    };

    /**@}*/
}

#endif // LIBQGIT2_HASHTABLE_H
