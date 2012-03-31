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
#include "qgitexception.h"

#include <QtCore/QFile>

#include <git2/tree.h>

namespace LibQGit2
{

QGitTreeEntry::QGitTreeEntry(const git_tree_entry *treeEntry)
    : d(treeEntry)
{
}

QGitTreeEntry::QGitTreeEntry(const QGitTreeEntry& other)
    : d(other.d)
{
}

QGitTreeEntry::~QGitTreeEntry()
{
}

bool QGitTreeEntry::isNull() const
{
    return constData() == 0;
}

unsigned int QGitTreeEntry::attributes() const
{
    return git_tree_entry_attributes(d);
}

const QString QGitTreeEntry::name() const
{
    return QFile::decodeName( git_tree_entry_name(d) );
}

QGitOId QGitTreeEntry::oid() const
{
    return QGitOId( git_tree_entry_id(d) );
}

QGitObject QGitTreeEntry::toObject(const QGitRepository& repo)
{
    git_object *obj;
    qGitThrow(git_tree_entry_2object(&obj, repo.data(), d));
    return QGitObject(obj);
}

const git_tree_entry* QGitTreeEntry::constData() const
{
    return d;
}

} // namespace LibQGit2
