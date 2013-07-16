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

#ifndef LIBQGIT2_INDEX_ENTRY_H
#define LIBQGIT2_INDEX_ENTRY_H

#include "libqgit2_export.h"

struct git_index_entry;

namespace LibQGit2
{
    class QGitOId;

    /**
     * @brief Wrapper class for git_index_entry.
     * Represents a Git index/stage entry.
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_INDEX_EXPORT QGitIndexEntry
    {
        public:
            /**
             * Create a new Git index entry object
             */
            explicit QGitIndexEntry(const git_index_entry *data);

            /**
             * Copy constructor
             */
            QGitIndexEntry(const QGitIndexEntry& other);

            /**
             * Destruct an existing index object.
             */
            ~QGitIndexEntry();

            /**
             * Get the id of an index entry.
             */
            QGitOId id() const;

            /**
             * Get the path of the index entry, represented by a string
             */
            QString path() const;

            /**
             * Get the size of the file
             */
            qint64 fileSize() const;

            const git_index_entry *data() const;

        private:
            const git_index_entry *d;
    };

    /**@}*/
}

#endif // LIBQGIT2_INDEX_ENTRY_H
