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

#include "qgitcommit.h"

#include "qgitrepository.h"
#include "qgitsignature.h"
#include "qgittree.h"
#include "qgitexception.h"

namespace LibQGit2
{

Commit::Commit(git_commit *commit)
    : QGitObject(reinterpret_cast<git_object*>(commit))
{
}

Commit::Commit(const Commit& other)
    : QGitObject(other)
{
}

Commit::~Commit()
{
}

QGitOId Commit::oid() const
{
    return QGitOId(git_commit_id(data()));
}

QString Commit::message() const
{
    return QString::fromUtf8(git_commit_message(data()));
}

QString Commit::shortMessage(int maxLen) const
{
    return message().left(maxLen).split(QRegExp("(\\r|\\n)")).first();
}

QDateTime Commit::dateTime() const
{
    QDateTime dateTime;
    dateTime.setTime_t(git_commit_time(data()));
    return dateTime;
}

int Commit::timeOffset() const
{
    return git_commit_time_offset(data());
}

QGitSignature Commit::committer() const
{
    return QGitSignature(git_commit_committer(data()));
}

QGitSignature Commit::author() const
{
    return QGitSignature(git_commit_author(data()));
}

QGitTree Commit::tree() const
{
    git_tree *tree;
    qGitThrow(git_commit_tree(&tree, data()));
    return QGitTree(tree);
}

unsigned int Commit::parentCount() const
{
    return git_commit_parentcount(data());
}

Commit Commit::parent(unsigned n) const
{
    git_commit *parent;
    qGitThrow(git_commit_parent(&parent, data(), n));
    return Commit(parent);
}

QGitOId Commit::parentId(unsigned n) const
{
    return QGitOId(git_commit_parent_id(data(), n));
}

git_commit* Commit::data() const
{
    return reinterpret_cast<git_commit*>(QGitObject::data());
}

const git_commit* Commit::constData() const
{
    return reinterpret_cast<git_commit*>(QGitObject::data());
}

} // namespace LibQGit2
