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

Commit Object::toCommit() const
{
    Commit commit;
    if (isCommit()) {
        commit.d = d;
    }
    return commit;
}

QGitTag Object::toTag() const
{
    QGitTag tag;
    if (isTag()) {
        tag.d = d;
    }
    return tag;
}

QGitTree Object::toTree() const
{
    QGitTree tree;
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
    return OId(git_object_id(data()));
}

bool Object::isCommit() const
{
    return git_object_type(data()) == GIT_OBJ_COMMIT;
}

bool Object::isTag() const
{
    return git_object_type(data()) == GIT_OBJ_TAG;
}

bool Object::isTree() const
{
    return git_object_type(data()) == GIT_OBJ_TREE;
}

bool Object::isBlob() const
{
    return git_object_type(data()) == GIT_OBJ_BLOB;
}

QString Object::typeString() const
{
    return QString(git_object_type2string(git_object_type(data())));
}

QGitRepository Object::owner() const
{
    return QGitRepository(git_object_owner(data()));
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
