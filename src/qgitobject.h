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

#ifndef LIBQGIT2_OBJECT_H
#define LIBQGIT2_OBJECT_H

#include <QtCore/QString>
#include <QtCore/QDateTime>
#include <QtCore/QSharedPointer>

#include "libqgit2_config.h"

#include "git2.h"

namespace LibQGit2
{
    class Repository;
    class OId;
    class Commit;
    class Tag;
    class Tree;
    class Blob;

    /**
     * @brief Wrapper class for git_object.
     *
     * This is the base class for every repository object, i.e. blob, commit,
     * tag and tree. Every object is identified with it's LibQGit2::OId.
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_EXPORT Object
    {
        public:
            /**
             * @brief The type of an \c Object.
             */
            enum Type {
                BadType,  ///< Not a valid Object type
                CommitType,
                TreeType,
                BlobType,
                TagType
            };

            /**
             * Create an Object.
             *
             * Creates an Object that points to the given object.
             * The pointer to the underlaying git_object is managed by this
             * Object, and is automatically freed when no more referenced.
             */
            explicit Object(git_object *object = 0);

            /**
             * Copy constructor.
             */
            Object(const Object& other);

            /**
             * Destroy the object.
             */
            ~Object();

            /**
             * Gets the type of this \c Object.
             */
            Type type() const;

            /**
             * Convert into a commit object.
             *
             * If the underlaying git_object is a commit this returns a valid
             * Commit object, otherwise it returns an empty one.
             */
            Commit toCommit() const;

            /**
             * Convert into a tag object.
             *
             * If the underlaying git_object is a tag this returns a valid
             * Tag object, otherwise it returns an empty one.
             */
            Tag toTag() const;

            /**
             * Convert into a tree object.
             *
             * If the underlaying git_object is a tree this returns a valid
             * Tag object, otherwise it returns an empty one.
             */
            Tree toTree() const;

            /**
             * Convert into a blob object.
             *
             * If the underlaying git_object is a blob this returns a valid
             * Tag object, otherwise it returns an empty one.
             */
            Blob toBlob() const;

            /**
             * Check if the pointer is null.
             *
             * Returns true if the git_object pointer owned by this
             * instance is null.
             */
            bool isNull() const;

            /**
             * Get the OId (SHA1) of a repository object.
             *
             * This returns the OId of the object.
             * Remember that in-memory objects created by git_object_new()
             * do not have a SHA1 id until they are written on a repository.
             *
             * @return the OId of the object
             */
            OId oid() const;

            /**
             * Check if this is a commit.
             *
             * Returns true if the object represents a commit; false otherwise.
             */
            bool isCommit() const;

            /**
             * Check if this is a tag.
             *
             * Returns true if the object represents a tag; false otherwise.
             */
            bool isTag() const;

            /**
             * Check if this is a tree.
             *
             * Returns true if the object represents a tree; false otherwise.
             */
            bool isTree() const;

            /**
             * Check if this is a blob.
             *
             * Returns true if the object represents a blob; false otherwise.
             */
            bool isBlob() const;

            /**
             * Get the object type as a string.
             */
            QString typeString() const;

            /**
             * Get the repository that owns this object.
             */
            Repository owner() const;

            git_object* data() const;
            const git_object* constData() const;

        private:
            QSharedPointer<git_object> d;

            static Type resolveType(git_otype);

            friend class TreeEntry;
    };

    /**
     * Compares two Objects. Objects are equal when their oid are equal.
     */
    bool operator ==(const Object &o1, const Object &o2);

    /**
     * Compares two Objects. Objects are different when their oid are different.
     */
    bool operator !=(const Object &o1, const Object &o2);

    /**@}*/
}

#endif // LIBQGIT2_OBJECT_H
