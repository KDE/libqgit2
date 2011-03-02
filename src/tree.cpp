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

#include "tree.h"

#include "treeentry.h"
#include "repository.h"
#include "oid.h"

using namespace LibQGit2;

Tree::Tree(Repository *repository, QObject* parent)
{
    git_tree_new(&m_tree, repository->data());
}

Tree::Tree(const git_tree *tree, QObject* parent)
    : m_tree(const_cast<git_tree *>(tree))
{
}

Tree::Tree( const Tree& other )
{
    m_tree = other.m_tree;
}

int Tree::lookup(Repository& repository, const OId& id)
{
    return git_tree_lookup(&m_tree, (git_repository *)repository.data(), (git_oid *)id.data());
}

const OId* Tree::id()
{
    const OId *oid = new OId(git_tree_id(m_tree));
    return oid;
}

size_t Tree::entryCount()
{
    return git_tree_entrycount(m_tree);
}

TreeEntry* Tree::entryByName(const QString& fileName)
{
    TreeEntry *treeEntry = new TreeEntry(git_tree_entry_byname(m_tree, fileName.toAscii().constData()));
    return treeEntry;
}

TreeEntry* Tree::entryByIndex(int idx)
{
    TreeEntry *treeEntry = new TreeEntry(git_tree_entry_byindex(m_tree, idx));
    return treeEntry;
}

int Tree::removeEntryByIndex(int idx)
{
    return git_tree_remove_entry_byindex(m_tree, idx);
}

int Tree::removeEntryByName(const QString& fileName)
{
    return git_tree_remove_entry_byname(m_tree, fileName.toAscii().constData());
}

void Tree::clearEntries()
{
    return git_tree_clear_entries(m_tree);
}

void Tree::setEntryId(TreeEntry* treeEntry, const OId& oid)
{
    return git_tree_entry_set_id(treeEntry->data(), oid.constData());
}

void Tree::setEntryName(TreeEntry* treeEntry, const QString& fileName)
{
    return git_tree_entry_set_name(treeEntry->data(), fileName.toAscii().constData());
}

void Tree::setEntryAttributes(TreeEntry* treeEntry, int attribute)
{
    return git_tree_entry_set_attributes(treeEntry->data(), attribute);
}

git_tree* Tree::data() const
{
    return m_tree;
}

const git_tree* Tree::constData() const
{
    return const_cast<const git_tree*>(m_tree);
}

