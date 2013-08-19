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

QGitObject::QGitObject(git_object *object)
    : d(object, git_object_free)
{
}

QGitObject::QGitObject(const QGitObject& other)
    : d(other.d)
{
}

QGitObject::~QGitObject()
{
}

Commit QGitObject::toCommit() const
{
    Commit commit;
    if (isCommit()) {
        commit.d = d;
    }
    return commit;
}

QGitTag QGitObject::toTag() const
{
    QGitTag tag;
    if (isTag()) {
        tag.d = d;
    }
    return tag;
}

QGitTree QGitObject::toTree() const
{
    QGitTree tree;
    if (isTree()) {
        tree.d = d;
    }
    return tree;
}

Blob QGitObject::toBlob() const
{
    Blob blob;
    if (isBlob()) {
        blob.d = d;
    }
    return blob;
}

bool QGitObject::isNull() const
{
    return d.isNull();
}

QGitOId QGitObject::oid() const
{
    return QGitOId(git_object_id(data()));
}

bool QGitObject::isCommit() const
{
    return git_object_type(data()) == GIT_OBJ_COMMIT;
}

bool QGitObject::isTag() const
{
    return git_object_type(data()) == GIT_OBJ_TAG;
}

bool QGitObject::isTree() const
{
    return git_object_type(data()) == GIT_OBJ_TREE;
}

bool QGitObject::isBlob() const
{
    return git_object_type(data()) == GIT_OBJ_BLOB;
}

QString QGitObject::typeString() const
{
    return QString(git_object_type2string(git_object_type(data())));
}

QGitRepository QGitObject::owner() const
{
    return QGitRepository(git_object_owner(data()));
}

git_object* QGitObject::data() const
{
    return d.data();
}

const git_object* QGitObject::constData() const
{
    return d.data();
}

bool operator ==(const QGitObject &o1, const QGitObject &o2)
{
    return (o1.oid() == o2.oid());
}

bool operator !=(const QGitObject &o1, const QGitObject &o2)
{
    return !(operator ==(o1, o2));
}

} // namespace LibQGit2
