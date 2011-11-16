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

#ifndef LIBQGIT2_OBJECT_H
#define LIBQGIT2_OBJECT_H

#include "libqgit2_export.h"

#include <QtCore/QString>
#include <QtCore/QDateTime>

#include <git2/object.h>

namespace LibQGit2
{
    class QGitRepository;
    class QGitOId;
    class LIBQGIT2_OBJECT_EXPORT QGitObject
    {
        public:
            explicit QGitObject(const git_object *object = 0);
            QGitObject( const QGitObject& other );
            ~QGitObject();

        public:
            /**
             * Write back an object to disk.
             *
             * The object will be written to its corresponding
             * repository.
             *
             * If the object has no changes since it was first
             * read from the repository, no actions will take place.
             *
             * If the object has been modified since it was read from
             * the repository, or it has been created from scratch
             * in memory, it will be written to the repository and
             * its SHA1 ID will be updated accordingly.
             *
             * @return 0 on success; otherwise an error code
             */
            int write();

            /**
             * Get the id (SHA1) of a repository object
             *
             * In-memory objects created by git_object_new() do not
             * have a SHA1 ID until they are written on a repository.
             *
             * @return the SHA1 id
             */
            QGitOId id() const;

            /**
             * * Get the object type of an object
             * *
             * * @return the object's type
             * */
            git_otype type() const;

            /**
             * Get the repository that owns this object
             *
             * @param obj the object
             * @return the repository who owns this object
             */
            QGitRepository* owner() const;

            /**
             * Convert an object type to it's string representation.
             *
             * The result is a string in static memory and
             * should not be free()'ed.
             *
             * @param type object type to convert.
             * @return the corresponding string representation.
             */
            static QString typeToString(git_otype type);

            /**
             * Convert a string object type representation to it's git_otype.
             *
             * @param string the string to convert.
             * @return the corresponding git_otype.
             */
            static git_otype stringToType(const QString& string);

            /**
             * Determine if the given git_otype is a valid loose object type.
             *
             * @param type object type to test.
             * @return true if the type represents a valid loose object type,
             * false otherwise.
             */
            static int typeIsLoose(git_otype type);

            /**
             * Get the size in bytes for the structure which
             * acts as an in-memory representation of any given
             * object type.
             *
             * For all the core types, this would the equivalent
             * of calling `sizeof(git_commit)` if the core types
             * were not opaque on the external API.
             *
             * @param type object type to get its size
             * @return size in bytes of the object
             */
            static size_t size(git_otype type);

            git_object* data() const;
            const git_object* constData() const;

        private:
            git_object *m_object;
    };
}

#endif // LIBQGIT2_OBJECT_H
