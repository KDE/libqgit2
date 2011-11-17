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

#include "qgittreeentry.h"

#include "qgitrepository.h"

#include <QtCore/QFile>

using namespace LibQGit2;

QGitTreeEntry::QGitTreeEntry(git_tree_entry *treeEntry)
    : m_treeEntry(treeEntry)
{
}

QGitTreeEntry::QGitTreeEntry(const QGitTreeEntry& other)
    : m_treeEntry(other.m_treeEntry)
{
}

QGitTreeEntry::~QGitTreeEntry()
{
}

unsigned int QGitTreeEntry::attributes() const
{
    return git_tree_entry_attributes(m_treeEntry);
}

const QString QGitTreeEntry::name() const
{
    return QString::fromUtf8(git_tree_entry_name(m_treeEntry));
}

QGitOId QGitTreeEntry::id() const
{
    return QGitOId(git_tree_entry_id(m_treeEntry));
}

int QGitTreeEntry::toObject(QGitObject& object)
{
    git_object *obj = object.data();
    return git_tree_entry_2object(&obj, m_treeEntry);
}

int QGitTreeEntry::setAttributes(unsigned int attributes)
{
    git_tree_entry_set_attributes(m_treeEntry, attributes);

    //! @todo Find dependencies
    return 0;
}

void QGitTreeEntry::setName(const QString& name)
{
    return git_tree_entry_set_name(m_treeEntry, QFile::encodeName(name));
}

void QGitTreeEntry::setId(const QGitOId& oid)
{
    return git_tree_entry_set_id(m_treeEntry, oid.data());
}

git_tree_entry* QGitTreeEntry::data() const
{
    return m_treeEntry;
}

const git_tree_entry* QGitTreeEntry::constData() const
{
    return m_treeEntry;
}

