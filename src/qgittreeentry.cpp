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

#include "qgittreeentry.h"
#include "qgitrepository.h"
#include "qgitexception.h"

#include "private/pathcodec.h"

namespace LibQGit2
{

TreeEntry::TreeEntry(const git_tree_entry* treeEntry)
    : d(treeEntry)
{
}

TreeEntry::TreeEntry(const TreeEntry& other)
    : d(other.d)
{
}

TreeEntry::~TreeEntry()
{
}

bool TreeEntry::isNull() const
{
    return d == 0;
}

unsigned int TreeEntry::attributes() const
{
    return git_tree_entry_filemode(d);
}

const QString TreeEntry::name() const
{
    return PathCodec::fromLibGit2(git_tree_entry_name(d));
}

OId TreeEntry::oid() const
{
    return OId(git_tree_entry_id(d));
}

Object::Type TreeEntry::type() const
{
    if (isNull()) {
        throw Exception("TreeEntry::type(): unknown type since this tree entry is null.");
    }

    return Object::resolveType(git_tree_entry_type(d));
}

Object TreeEntry::toObject(const Repository& repo)
{
    git_object *obj;
    qGitThrow(git_tree_entry_to_object(&obj, repo.data(), d));
    return Object(obj);
}

const git_tree_entry* TreeEntry::data() const
{
    return d;
}

} // namespace LibQGit2
