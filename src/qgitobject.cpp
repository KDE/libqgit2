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

#include "qgitobject.h"

#include "qgitoid.h"
#include "qgitrepository.h"
#include "qgitcommit.h"
#include "qgittag.h"
#include "qgittree.h"
#include "qgitblob.h"

using namespace LibQGit2;

QGitObject::QGitObject(git_object *object)
    : d(object, git_object_close)
{
}

QGitObject::QGitObject(const QGitObject& other)
    : d(other.d)
{
}

QGitObject::~QGitObject()
{
}

QGitCommit QGitObject::toCommit() const
{
    QGitCommit commit;
    if (type() == GIT_OBJ_COMMIT) {
        commit.d = d;
    }
    return commit;
}

QGitTag QGitObject::toTag() const
{
    QGitTag tag;
    if (type() == GIT_OBJ_TAG) {
        tag.d = d;
    }
    return tag;
}

QGitTree QGitObject::toTree() const
{
    QGitTree tree;
    if (type() == GIT_OBJ_TREE) {
        tree.d = d;
    }
    return tree;
}

QGitBlob QGitObject::toBlob() const
{
    QGitBlob blob;
    if (type() == GIT_OBJ_BLOB) {
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

git_otype QGitObject::type() const
{
    return git_object_type(data());
}

QString QGitObject::typeString() const
{
    return QString(git_object_type2string(type()));
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
