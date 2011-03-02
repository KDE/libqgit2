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

#ifndef LIBQGIT2_RAWOBJECT_H
#define LIBQGIT2_RAWOBJECT_H

#include "libqgit2_export.h"

#include <git2/odb.h>

namespace LibQGit2
{
    class Database;
    class OId;
    class LIBQGIT2_RAWOBJECT_EXPORT RawObject
    {
            Q_OBJECT
        public:
            /**
             * Create a new object database with no backends.
             *
             * Before the ODB can be used for read/writing, a custom database
             * backend must be manually added using `addBackend()`
             *
             */
            RawObject( QObject* parent = 0 );

            RawObject( const RawObject& other );

            ~RawObject();

        public:
            /**
             * Read an object from the database.
             *
             * If GIT_ENOTFOUND then out->data is set to NULL.
             *
             * @param db database to search for the object in.
             * @param id identity of the object to read.
             * @return
             * - GIT_SUCCESS if the object was read;
             * - GIT_ENOTFOUND if the object is not in the database.
             */
            int read(Database *db, const OId *id);

            /**
             * Read the header of an object from the database, without
             * reading its full contents.
             *
             * Only the 'type' and 'len' fields of the git_rawobj structure
             * are filled. The 'data' pointer will always be NULL.
             *
             * The raw object pointed by 'out' doesn't need to be manually
             * closed with git_rawobj_close().
             *
             * @param db database to search for the object in.
             * @param id identity of the object to read.
             * @return
             * - GIT_SUCCESS if the object was read;
             * - GIT_ENOTFOUND if the object is not in the database.
             */
            int readHeader(Database *db, const OId *id);

            /**
             * Write an object to the database.
             *
             * @param id identity of the object written.
             * @param db database to which the object should be written.
             * @param obj object descriptor for the object to write.
             * @return
             * - GIT_SUCCESS if the object was written;
             * - GIT_ERROR otherwise.
             */
            int write(OId *id, Database *db);

            /**
             * Determine the object-ID (sha1 hash) of the given git_rawobj.
             *
             * The input obj must be a valid loose object type and the data
             * pointer must not be NULL, unless the len field is also zero.
             *
             * @param id the resulting object-ID.
             * @return
             * - GIT_SUCCESS if the object-ID was correctly determined.
             * - GIT_ERROR if the given object is malformed.
             */
            int hash(OId *id);

            /**
             * Release all memory used by the obj structure.
             *
             * As a result of this call, obj->data will be set to NULL.
             *
             * If obj->data is already NULL, nothing happens.
             *
             */
            void close();

            git_rawobj* data() const;
            const git_rawobj* constData() const;

        private:
            git_rawobj *m_rawObject;
    };
}

#endif // LIBQGIT2_RAWOBJECT_H
