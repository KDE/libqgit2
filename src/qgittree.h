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

#ifndef LIBQGIT2_TREE_H
#define LIBQGIT2_TREE_H

#include "qgitobject.h"

#include <QtCore/QString>

namespace LibQGit2
{
    class Repository;
    class OId;
    class TreeEntry;

    /**
     * @brief Wrapper class for git_tree.
     * Represents a Git tree object.
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_EXPORT Tree : public Object
    {
        public:

            /**
             * Creates a Tree that points to tree. The pointer object becomes managed by
             * this Tree, and must not be passed to another Tree or closed outside this
             * object.
             */
            explicit Tree(git_tree *tree = 0);

            /**
             * Copy constructor; creates a copy of the object, sharing the same underlaying data
             * structure.
             */
            Tree(const Tree& other);

            /**
             * Destroys the object.
             */
            ~Tree();

            /**
             * * Get the id of a tree.
             * * @return object identity for the tree.
             * */
            OId oid();

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
            TreeEntry entryByName(const QString& fileName) const;

            /**
             * Lookup a tree entry by its position in the tree
             * @param idx the position in the entry list
             * @return the tree entry; NULL if not found
             */
            TreeEntry entryByIndex(int idx) const;

            git_tree* data() const;
            const git_tree* constData() const;
    };

    /**@}*/
}

#endif // LIBQGIT2_TREE_H
