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

#ifndef LIBQGIT2_BLOB_H
#define LIBQGIT2_BLOB_H

#include "qgitobject.h"

namespace LibQGit2
{
    /**
     * @brief Wrapper class for git_blob.
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_EXPORT Blob : public Object
    {
        public:

            /**
             * Create an Blob.
             *
             * Creates a Blob that points to the given git_blob.
             */
            explicit Blob(git_blob *blob = 0);

            /**
             * Copy constructor.
             */
            Blob(const Blob& other);

            /**
             * Destroy the object.
             */
            ~Blob();

            /**
             * Get the raw content of this blob.
             *
             * This method returns a read-only buffer with the raw content
             * of the blob. This pointer is automatically freed when there
             * are no more objects referencing it. The pointer may be
             * invalidated at a later time (e.g. when changing the contents
             * of the blob).
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
