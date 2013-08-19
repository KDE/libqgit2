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

#ifndef LIBQGIT2_INDEX_H
#define LIBQGIT2_INDEX_H

#include <QtCore/QSharedPointer>

#include "git2.h"

#include "libqgit2_export.h"

namespace LibQGit2
{
    class OId;
    class Repository;
    class IndexEntry;

    /**
     * @brief Wrapper class for git_index.
     * Represents a Git index a.k.a "the stage".
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_INDEX_EXPORT Index
    {
        public:

            /**
             * Creates a Index that points to 'index'. The pointer 'index' becomes managed by
             * this Index, and must not be passed to another Index or freed outside this
             * object.
             */
            explicit Index(git_index *index = 0);

            /**
             * Copy constructor; creates a copy of the object, sharing the same underlaying data
             * structure.
             */
            Index(const Index& other);

            /**
             * Destruct an existing index object.
             */
            ~Index();

            /**
             * Create a index object as a memory representation
             * of the Git index file in 'indexPath', without a repository
             * to back it.
             *
             * Since there is no ODB behind this index, any Index methods
             * which rely on the ODB (e.g. index_add) will fail with the
             * GIT_EBAREINDEX error code.
             *
             * @param index_path the path to the index file in disk
             * @throws LibQGit2::Exception
             */
            void open(const QString& indexPath);

            /**
             * Create a new tree object from the index
             *
             * @throws LibQGit2::Exception
             */
            OId createTree();

            /**
             * Clear the contents (all the entries) of an index object.
             * This clears the index object in memory; changes must be manually
             * written to disk for them to take effect.
             */
            void clear();

            /**
             * Update the contents of an existing index object in memory
             * by reading from the hard disk.
             *
             * @throws LibQGit2::Exception
             */
            void read() const;

            /**
             * Write an existing index object from memory back to disk
             * using an atomic file lock.
             *
             * @throws LibQGit2::Exception
             */
            void write();

            /**
             * Find the first index of any entires which point to given
             * path in the Git index.
             *
             * @param path path to search
             * @return an index >= 0 if found, -1 otherwise
             */
            int find(const QString& path);

            /**
             * Add or update an index entry from a file in disk.
             *
             * @param path filename to add
             * @throws LibQGit2::Exception
             */
            void addByPath(const QString& path);

            /**
             * Remove an entry from the index given the path
             *
             * @param stage stage of the entry to remove
             * @throws LibQGit2::Exception
             */
            void remove(const QString& path, int stage = GIT_INDEX_STAGE_ANY);

            /**
             * Insert an entry into the index.
             * A full copy (including the 'path' string) of the given
             * 'source_entry' will be inserted on the index; if the index
             * already contains an entry for the same path, the entry
             * will be updated.
             *
             * @param source_entry new entry object
             * @throws LibQGit2::Exception
             */
            void add(const IndexEntry& source_entry);

            /**
             * Get a pointer to one of the entries in the index
             *
             * This entry can be modified, and the changes will be written
             * back to disk on the next write() call.
             *
             * @param n the position of the entry
             * @return a pointer to the entry; NULL if out of bounds
             */
            IndexEntry getByIndex(int n) const;

            /**
             * Get the count of entries currently in the index
             *
             * @return integer of count of current entries
             */
            unsigned int entryCount() const;

            git_index* data() const;
            const git_index* constData() const;

        private:
            typedef QSharedPointer<git_index> ptr_type;
            ptr_type d;
    };

    /**@}*/
}

#endif // LIBQGIT2_INDEX_H
