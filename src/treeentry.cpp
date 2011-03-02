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

#include "treeentry.h"

#include "repository.h"

using namespace LibQGit2;

TreeEntry::TreeEntry(const git_tree_entry *treeEntry, QObject* parent)
    : m_treeEntry(const_cast<git_tree_entry *>(treeEntry))
{
}

TreeEntry::TreeEntry( const TreeEntry& other )
{
}

TreeEntry::~TreeEntry()
{
}

unsigned int TreeEntry::attributes() const
{
    return git_tree_entry_attributes(m_treeEntry);
}

const QString TreeEntry::name() const
{
    return QString::fromUtf8(git_tree_entry_name(m_treeEntry));
}

const OId* TreeEntry::id() const
{
    const OId *oid = new OId(git_tree_entry_id(m_treeEntry));
    return oid;
}

int TreeEntry::toObject(Object& object)
{
    git_object *obj = object.data();
    return git_tree_entry_2object(&obj, m_treeEntry);
}

void TreeEntry::setAttributes(int attributes)
{
    return git_tree_entry_set_attributes(m_treeEntry, attributes);
}

void TreeEntry::setName(const QString& name)
{
    return git_tree_entry_set_name(m_treeEntry, name.toAscii().constData());
}

void TreeEntry::setId(const OId& oid)
{
    return git_tree_entry_set_id(m_treeEntry, oid.constData());
}

git_tree_entry* TreeEntry::data() const
{
    return m_treeEntry;
}

const git_tree_entry* TreeEntry::constData() const
{
    return const_cast<const git_tree_entry *>(m_treeEntry);
}

