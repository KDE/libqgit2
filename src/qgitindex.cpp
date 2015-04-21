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

#include "qgitindex.h"
#include "qgitindexentry.h"
#include "qgittree.h"
#include "qgitexception.h"

#include "qgitrepository.h"

#include "private/pathcodec.h"

namespace LibQGit2
{

Index::Index(git_index *index)
    : d(index, git_index_free)
{
}

Index::Index(const Index& other)
    : d(other.d)
{
}

Index::~Index()
{
}

void Index::open(const QString& indexPath)
{
    d.clear();
    git_index *index = 0;
    qGitThrow(git_index_open(&index, PathCodec::toLibGit2(indexPath)));
    d = ptr_type(index, git_index_free);
}

OId Index::createTree()
{
    OId oid;
    qGitThrow(git_index_write_tree(oid.data(), data()));
    return oid;
}

void Index::clear()
{
    qGitThrow(git_index_clear(data()));
}

void Index::read(bool force) const
{
    qGitThrow(git_index_read(data(), force ? 1 : 0));
}

void Index::write()
{
    qGitThrow(git_index_write(data()));
}

int Index::find(const QString& path)
{
    return git_index_find(NULL, data(), PathCodec::toLibGit2(path));
}

void Index::addByPath(const QString& path)
{
    qGitThrow(git_index_add_bypath(data(), PathCodec::toLibGit2(path)));
}

void Index::remove(const QString& path, int stage)
{
    qGitThrow(git_index_remove(data(), PathCodec::toLibGit2(path), stage));
}

void Index::add(const IndexEntry &source_entry)
{
    qGitThrow(git_index_add(data(), source_entry.data()));
}

void Index::updateAll()
{
    qGitThrow(git_index_update_all(data(), NULL, NULL, NULL));
}

IndexEntry Index::getByIndex(int n) const
{
    return IndexEntry(git_index_get_byindex(data(), n));
}

unsigned int Index::entryCount() const
{
    return git_index_entrycount(data());
}

bool Index::hasConflicts() const
{
    return !d.isNull() && git_index_has_conflicts(d.data());
}

git_index* Index::data() const
{
    return d.data();
}

const git_index* Index::constData() const
{
    return d.data();
}

} // namespace LibQGit2
