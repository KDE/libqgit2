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

#ifndef LIBQGIT2_INDEX_H
#define LIBQGIT2_INDEX_H

#include "libqgit2_export.h"

#include <QtCore/QSharedPointer>

struct git_index;

namespace LibQGit2
{
    class QGitOId;
    class QGitRepository;
    class QGitIndexEntry;

    class LIBQGIT2_INDEX_EXPORT QGitIndex
    {
        public:

            /**
             * Creates a QGitIndex that points to 'index'. The pointer 'index' becomes managed by
             * this QGitIndex, and must not be passed to another QGitIndex or freed outside this
             * object.
             */
            explicit QGitIndex(git_index *index = 0);

            /**
             * Copy constructor; creates a copy of the object, sharing the same underlaying data
             * structure.
             */
            QGitIndex(const QGitIndex& other);

            /**
             * Destruct an existing index object.
             */
            ~QGitIndex();

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
             * @throws QGitException
             */
            void open(const QString& indexPath);

            /**
             * Create a new tree object from the index
             *
             * @throws QGitException
             */
            QGitOId createTree();

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
             * @throws QGitException
             */
            void read() const;

            /**
             * Write an existing index object from memory back to disk
             * using an atomic file lock.
             *
             * @throws QGitException
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
             * @param stage stage for the entry
             * @throws QGitException
             */
            void add(const QString& path, int stage = 0);

            /**
             * Remove an entry from the index
             *
             * @param position position of the entry to remove
             * @throws QGitException
             */
            void remove(int position);

            /**
             * Insert an entry into the index.
             * A full copy (including the 'path' string) of the given
             * 'source_entry' will be inserted on the index; if the index
             * already contains an entry for the same path, the entry
             * will be updated.
             *
             * @param source_entry new entry object
             * @throws QGitException
             */
            void insert(const QGitIndexEntry& source_entry);

            /**
             * Get a pointer to one of the entries in the index
             *
             * This entry can be modified, and the changes will be written
             * back to disk on the next write() call.
             *
             * @param n the position of the entry
             * @return a pointer to the entry; NULL if out of bounds
             */
            QGitIndexEntry get(int n) const;

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
}

#endif // LIBQGIT2_INDEX_H
