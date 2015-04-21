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

#include "qgittree.h"

#include "qgittreeentry.h"
#include "qgitrepository.h"
#include "qgitoid.h"

#include "private/pathcodec.h"

namespace LibQGit2
{

Tree::Tree(git_tree *tree)
    : Object(reinterpret_cast<git_object*>(tree))
{
}

Tree::Tree( const Tree& other )
    : Object(other)
{
}

Tree::~Tree()
{
}

OId Tree::oid()
{
    return OId(git_tree_id(data()));
}

size_t Tree::entryCount()
{
    return git_tree_entrycount(data());
}

TreeEntry Tree::entryByName(const QString& fileName) const
{
    return TreeEntry(git_tree_entry_byname(constData(), PathCodec::toLibGit2(fileName)));
}

TreeEntry Tree::entryByIndex(int idx) const
{
    return TreeEntry(git_tree_entry_byindex(data(), idx));
}

git_tree* Tree::data() const
{
    return reinterpret_cast<git_tree*>(Object::data());
}

const git_tree* Tree::constData() const
{
    return reinterpret_cast<git_tree*>(Object::data());
}

} // namespace LibQGit2
