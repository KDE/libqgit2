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

#include "qgittree.h"

#include "qgittreeentry.h"
#include "qgitrepository.h"
#include "qgitoid.h"

#include <QtCore/QFile>

using namespace LibQGit2;

QGitTree::QGitTree(const QGitRepository& repository)
{
    git_tree_new(&m_tree, repository.data());
}

QGitTree::QGitTree(git_tree *tree)
    : m_tree(tree)
{
}

QGitTree::QGitTree( const QGitTree& other )
{
    m_tree = other.m_tree;
}

QGitTree::~QGitTree()
{
}

int QGitTree::lookup(QGitRepository& repository, const QGitOId& id)
{
    return git_tree_lookup(&m_tree, (git_repository *)repository.data(), (git_oid *)id.data());
}

QGitOId QGitTree::id()
{
    return QGitOId(git_tree_id(m_tree));
}

size_t QGitTree::entryCount()
{
    return git_tree_entrycount(m_tree);
}

QGitTreeEntry QGitTree::entryByName(const QString& fileName)
{
    return QGitTreeEntry(git_tree_entry_byname(m_tree, QFile::encodeName(fileName)));
}

QGitTreeEntry QGitTree::entryByIndex(int idx)
{
    return QGitTreeEntry(git_tree_entry_byindex(m_tree, idx));
}

int QGitTree::removeEntryByIndex(int idx)
{
    return git_tree_remove_entry_byindex(m_tree, idx);
}

int QGitTree::removeEntryByName(const QString& fileName)
{
    return git_tree_remove_entry_byname(m_tree, QFile::encodeName(fileName));
}

void QGitTree::clearEntries()
{
    return git_tree_clear_entries(m_tree);
}

void QGitTree::setEntryId(QGitTreeEntry& treeEntry, const QGitOId& oid)
{
    return git_tree_entry_set_id(treeEntry.data(), oid.data());
}

void QGitTree::setEntryName(QGitTreeEntry& treeEntry, const QString& fileName)
{
    return git_tree_entry_set_name(treeEntry.data(), QFile::encodeName(fileName));
}

int QGitTree::setEntryAttributes(QGitTreeEntry& treeEntry, int attribute)
{
    git_tree_entry_set_attributes(treeEntry.data(), attribute);

    //! @todo Find dependencies
    return 0;
}

git_tree* QGitTree::data() const
{
    return m_tree;
}

const git_tree* QGitTree::constData() const
{
    return m_tree;
}
