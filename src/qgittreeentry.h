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

#ifndef LIBQGIT2_TREEENTRY_H
#define LIBQGIT2_TREEENTRY_H

#include "libqgit2_export.h"

struct git_tree_entry;

namespace LibQGit2
{
    class QGitOId;
    class QGitObject;
    class QGitRepository;

    class LIBQGIT2_TREEENTRY_EXPORT QGitTreeEntry
    {
        public:
            explicit QGitTreeEntry(const git_tree_entry *treeEntry = 0);
            QGitTreeEntry(const QGitTreeEntry& other);
            ~QGitTreeEntry();

        public:
            /**
             * Get the UNIX file attributes of a tree entry
             * @return attributes as an integer
             */
            unsigned int attributes() const;

            /**
             * Get the filename of a tree entry
             * @return the name of the file
             */
            const QString name() const;


            /**
             * Get the id of the object pointed by the entry
             * @return the oid of the object
             */
            QGitOId id() const;

            /**
             * Convert a tree entry to the Object it points too.
             *
             * @param object pointer to the converted object
             * @return a reference to the pointed object in the repository
             */
            QGitObject toObject(const QGitRepository& repo);

            const git_tree_entry* data() const;
            const git_tree_entry* constData() const;

        private:
            const git_tree_entry *m_treeEntry;
    };
}

#endif // LIBQGIT2_TREEENTRY_H
