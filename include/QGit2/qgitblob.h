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

#ifndef LIBQGIT2_BLOB_H
#define LIBQGIT2_BLOB_H

#include "qgitobject.h"

struct git_blob;

namespace LibQGit2
{
    /**
     * @brief Wrapper class for git_blob.
     * Represents a Git blob (binary large object).
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_EXPORT QGitBlob : public QGitObject
    {
        public:

            /**
             * Creates a QGitBlob that points to blob. The pointer object becomes managed by
             * this QGitBlob, and must not be passed to another QGitBlob or closed outside this
             * object.
             */
            explicit QGitBlob(git_blob *blob = 0);

            /**
             * Copy constructor; creates a copy of the object, sharing the same underlaying data
             * structure.
             */
            QGitBlob(const QGitBlob& other);

            /**
             * Destroys the object.
             */
            ~QGitBlob();

            /**
             * Get a read-only buffer with the raw content of a blob.
             *
             * A pointer to the raw content of a blob is returned;
             * this pointer is owned internally by the object and shall
             * not be free'd. The pointer may be invalidated at a later
             * time (e.g. when changing the contents of the blob).
             *
             * @return the pointer; NULL if the blob has no contents
             */
            const void* rawContent() const;

            /**
              * @return The blob content as QByteArray.
              */
            QByteArray content() const;

            /**
             * Get the size in bytes of the contents of a blob
             *
             * @return size on bytes
             */
            int rawSize() const;

            git_blob* data() const;
            const git_blob* constData() const;
    };

    /**@}*/
}

#endif // LIBQGIT2_BLOB_H
