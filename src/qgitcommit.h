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

#ifndef LIBQGIT2_COMMIT_H
#define LIBQGIT2_COMMIT_H

#include "qgitobject.h"
#include "qgittree.h"
#include "qgitsignature.h"

#include <QtCore/QDateTime>

namespace LibQGit2
{
    class OId;
    class Repository;

    /**
     * @brief Wrapper class for git_commit.
     *
     * This class represents a Git commit object.
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_EXPORT Commit : public Object
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
            */
            OId oid() const;

            /**
             * Get the full message of a commit.
             */
            QString message() const;

            /**
             * Get the short commit message.
             *
             * Get the first part of the commit message (similar to
             * git log --oneline). The string is further cut when a
             * linebreak is found.
             *
             * @param maxLen maximal length of the resulting string.
             * Default is 80 characters.
             *
             * @return the short message
             */
            QString shortMessage(int maxLen = 80) const;

            /**
             * Get the commit time (i.e. committer time) of this commit.
             */
            QDateTime dateTime() const;

            /**
             * Get the timezone offset.
             *
             * Get the timezone offset (i.e. committer's preferred timezone)
             * of this commit.
             *
             * @return positive or negative timezone offset, in minutes from UTC
             */
            int timeOffset() const;

            /**
             * Get the committer signature of this commit.
             */
            Signature committer() const;

            /**
             * Get the author signature of this commit.
             */
            Signature author() const;

            /**
             * Get the tree pointed to by this commit.
             *
             * @throws Exception
             */
            Tree tree() const;

            /**
             * Get the number of parents of this commit
             */
            unsigned int parentCount() const;

            /**
             * Get the specified parent of the commit.
             *
             * This method returns the nth parent of this commit or, if no
             * parent can be found, an empty commit.
             *
             * @param n the position of the parent
             * @return the parent commit or an empty commit
             * @throws Exception
             */
            Commit parent(unsigned n) const;

            /**
             * Get the OId of the specified parent of the commit.
             *
             * This method returns the Oid of the nth parent of this commit or,
             * if no parent can be found, an empty OId.
             *
             * @param n the position of the parent
             * @return the OId of the parent commit or an empty OId
             * @throws Exception
             */
            OId parentId(unsigned n) const;

            /**
             * Amends an existing commit.
             *
             * Only the arguments that are provided are used. Arguments left to their default values
             * mean that the property in the Commit is not changed.
             *
             * @param tree The Tree to be used to amend the Commit.
             * @param ref Name of the reference that will be updated to point to the amended Commit.
             *        See Repository::createCommit.
             * @param message The message for the amended Commit.
             * @param author Author signature.
             * @param committer Committer signature.
             * @return The OId of the amended Commit.
             *
             * @see Repository::createCommit
             */
            OId amend(const Tree& tree = Tree(), const QString& ref = QString(), const QString& message = QString(), const Signature& author = Signature(), const Signature& committer = Signature());

            git_commit* data() const;
            const git_commit* constData() const;
    };

    /**@}*/
}

#endif // LIBQGIT2_COMMIT_H
