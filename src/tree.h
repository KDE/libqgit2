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
    class Repository;
    class OId;
    class TreeEntry;
    class LIBQGIT2_TREE_EXPORT Tree
    {
        public:
            /**
             * Construct a new in-memory tree.
             *
             * The tree object must be manually filled using
             * setter methods before it can be written to its
             * repository.
             *
             * @param repo The repository where the object will reside
             * @return 0 on success; error code otherwise
             */
            Tree(Repository *repository, QObject* parent = 0 );
            Tree(const git_tree *tree = 0, QObject* parent = 0 );

            Tree( const Tree& other );
            ~Tree();

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
            int lookup(Repository& repository, const OId& id);

            /**
             * * Get the id of a tree.
             * * @return object identity for the tree.
             * */
            const OId* id();

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
            TreeEntry* entryByName(const QString& fileName);

            /**
             * Lookup a tree entry by its position in the tree
             * @param idx the position in the entry list
             * @return the tree entry; NULL if not found
             */
            TreeEntry* entryByIndex(int idx);

            /**
             * Add a new entry to the tree and return the new entry.
             *
             * This will mark the tree as modified; the new entry will
             * be written back to disk on the next write() call of the Object class
             *
             * @param entryOut Pointer to the entry that just got
             * created. May be NULL if you are not interested on
             * getting the new entry
             * @iparam id OID for the tree entry
             * @param filename Filename for the tree entry
             * @param attributes UNIX file attributes for the entry
             * @return 0 on success; otherwise error code
             */
            int addEntry(TreeEntry *entryOut, const OId& id, const QString& fileName, int attributes);

            /**
             * Remove an entry by its index.
             *
             * Index must be >= 0 and < than entryCount().
             *
             * This will mark the tree as modified; the modified entry will
             * be written back to disk on the next write() call of the Object class
             *
             * @param idx index of the entry
             * @return 0 on successful removal; GIT_ENOTFOUND if the entry wasn't found
             */
            int removeEntryByIndex(int idx);

            /**
             * Remove an entry by its filename.
             *
             * This will mark the tree as modified; the modified entry will
             * be written back to disk on the next write() call of the Object class
             *
             * @param fileName File name of the entry
             * @return 0 on successful removal; GIT_ENOTFOUND if the entry wasn't found
             */
            int removeEntryByName(const QString& fileName);

            /**
             * Clear all the entries in a tree.
             *
             * This will mark the tree as modified; the modified entry will
             * be written back to disk on the next write() call of the Object class
             *
             */
            void clearEntries();

            /**
             * Change the SHA1 id of a tree entry.
             *
             * This will mark the tree that contains the entry as modified;
             * the modified entry will be written back to disk on the next write() call of the Object class
             *
             * @param entry Entry object which will be modified
             * @param oid new SHA1 oid for the entry
             */
            void setEntryId(TreeEntry* treeEntry, const OId& oid);

            /**
             * Change the filename of a tree entry.
             *
             * This will mark the tree that contains the entry as modified;
             * the modified entry will be written back to disk on the next write() call of the Object class
             *
             * @param entry Entry object which will be modified
             * @param fileName new filename for the entry
             */
            void setEntryName(TreeEntry* treeEntry, const QString& fileName);

            /**
             * Change the attributes of a tree entry.
             *
             * This will mark the tree that contains the entry as modified;
             * the modified entry will be written back to disk on the next git_object_write()
             *
             * @param attribute new attributes for the entry
             */
            void setEntryAttributes(TreeEntry* treeEntry, int attribute);

            git_tree* data() const;
            const git_tree* constData() const;

        private:
            git_tree *m_tree;
    };
}

#endif // LIBQGIT2_TREE_H
