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

#include "qgitobject.h"

#include "qgitoid.h"
#include "qgitrepository.h"
#include "qgitcommit.h"
#include "qgittag.h"
#include "qgittree.h"
#include "qgitblob.h"

namespace LibQGit2
{

// git_object_free() is automatically invoked on the pointer when
// it is no more referenced.
Object::Object(git_object *object)
    : d(object, git_object_free)
{
}

Object::Object(const Object& other)
    : d(other.d)
{
}

Object::~Object()
{
}

Object::Type Object::type() const
{
    Type t = BadType;
    if (!isNull()) {
        t = resolveType(git_object_type(d.data()));
    }

    return t;
}

Object::Type Object::resolveType(git_otype rawType)
{
    switch (rawType) {
    case GIT_OBJ_COMMIT:
        return CommitType;
    case GIT_OBJ_TAG:
        return TagType;
    case GIT_OBJ_TREE:
        return TreeType;
    case GIT_OBJ_BLOB:
        return BlobType;
    default:
        return BadType;
    }
}

Commit Object::toCommit() const
{
    Commit commit;
    if (isCommit()) {
        commit.d = d;
    }
    return commit;
}

Tag Object::toTag() const
{
    Tag tag;
    if (isTag()) {
        tag.d = d;
    }
    return tag;
}

Tree Object::toTree() const
{
    Tree tree;
    if (isTree()) {
        tree.d = d;
    }
    return tree;
}

Blob Object::toBlob() const
{
    Blob blob;
    if (isBlob()) {
        blob.d = d;
    }
    return blob;
}

bool Object::isNull() const
{
    return d.isNull();
}

OId Object::oid() const
{
    return OId(git_object_id(d.data()));
}

bool Object::isCommit() const
{
    return type() == CommitType;
}

bool Object::isTag() const
{
    return type() == TagType;
}

bool Object::isTree() const
{
    return type() == TreeType;
}

bool Object::isBlob() const
{
    return type() == BlobType;
}

QString Object::typeString() const
{
    return QString(git_object_type2string(git_object_type(d.data())));
}

Repository Object::owner() const
{
    return Repository(git_object_owner(d.data()));
}

git_object* Object::data() const
{
    return d.data();
}

const git_object* Object::constData() const
{
    return d.data();
}

bool operator ==(const Object &o1, const Object &o2)
{
    return (o1.oid() == o2.oid());
}

bool operator !=(const Object &o1, const Object &o2)
{
    return !(operator ==(o1, o2));
}

} // namespace LibQGit2
