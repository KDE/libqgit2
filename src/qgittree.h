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

#ifndef LIBQGIT2_TREE_H
#define LIBQGIT2_TREE_H

#include "libqgit2_export.h"

#include <QtCore/QString>

#include <git2/tree.h>

namespace LibQGit2
{
    class QGitRepository;
    class QGitOId;
    class QGitTreeEntry;
    class LIBQGIT2_TREE_EXPORT QGitTree
    {
        public:

            explicit QGitTree(git_tree *tree = 0);

            QGitTree( const QGitTree& other );
            ~QGitTree();

        public:
            /**
             * Lookup a tree object from the repository.
             * The generated tree object is owned by the revision
             * repo and shall not be freed by the user.
             *
             * @param repo the repo to use when locating the tree.
             * @param id identity of the tree to locate.
             * @return 0 on success; error code otherwise
             */
            int lookup(QGitRepository& repository, const QGitOId& id);

            /**
             * * Get the id of a tree.
             * * @return object identity for the tree.
             * */
            QGitOId id();

            /**
             * Get the number of entries listed in a tree
             * @return the number of entries in the tree
             */
            size_t entryCount();

            /**
             * Lookup a tree entry by its filename
             * @param filename the filename of the desired entry
             * @return the tree entry; NULL if not found
             */
            QGitTreeEntry entryByName(const QString& fileName);

            /**
             * Lookup a tree entry by its position in the tree
             * @param idx the position in the entry list
             * @return the tree entry; NULL if not found
             */
            QGitTreeEntry entryByIndex(int idx);

            git_tree* data() const;
            const git_tree* constData() const;

        private:
            git_tree *m_tree;
    };
}

#endif // LIBQGIT2_TREE_H
