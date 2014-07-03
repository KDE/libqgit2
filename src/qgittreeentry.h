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

#ifndef LIBQGIT2_TREEENTRY_H
#define LIBQGIT2_TREEENTRY_H

#include "git2.h"

#include "qgitobject.h"
#include "libqgit2_config.h"

namespace LibQGit2
{
    class OId;
    class Repository;

    /**
     * @brief Wrapper class for git_tree_entry.
     * Represents a Git child tree entry, that can either point to another tree object or a blob.
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_EXPORT TreeEntry
    {
        public:
            explicit TreeEntry(const git_tree_entry* treeEntry);
            TreeEntry(const TreeEntry& other);
            ~TreeEntry();

        public:
            /**
              * @return true when internal pointer is 0; otherwise false
              */
            bool isNull() const;

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
            OId oid() const;

            /**
             * Get the type of the \c Object where this entry points to.
             * @note %Tree entries might not support all the possible object types.
             */
            Object::Type type() const;

            /**
             * Convert a tree entry to the Object it points too.
             *
             * @param object pointer to the converted object
             * @return a reference to the pointed object in the repository
             * @throws LibQGit2::Exception
             */
            Object toObject(const Repository& repo);

            const git_tree_entry* data() const;

        private:
            const git_tree_entry *d;
    };

    /**@}*/
}

#endif // LIBQGIT2_TREEENTRY_H
