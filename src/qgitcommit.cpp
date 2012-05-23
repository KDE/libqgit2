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

#include "qgitcommit.h"

#include "qgitrepository.h"
#include "qgitsignature.h"
#include "qgittree.h"
#include "qgitexception.h"

#include <git2/commit.h>

#include <QtCore/QTextCodec>

namespace LibQGit2
{

QGitCommit::QGitCommit(git_commit *commit)
    : QGitObject(reinterpret_cast<git_object*>(commit))
{
}

QGitCommit::QGitCommit(const QGitCommit& other)
    : QGitObject(other)
{
}

QGitCommit::~QGitCommit()
{
}

QGitOId QGitCommit::oid() const
{
    return QGitOId(git_commit_id(data()));
}

QString QGitCommit::message() const
{
    QTextCodec *codec = QTextCodec::codecForName( git_commit_message_encoding(data()) );

    if (codec != 0)
        return codec->toUnicode( git_commit_message(data()) );

    return QString( git_commit_message(data()) );
}

QString QGitCommit::shortMessage(int maxLen) const
{
    return message().left(maxLen).split(QRegExp("(\\r|\\n)")).first();
}

QDateTime QGitCommit::dateTime() const
{
    QDateTime dateTime;
    dateTime.setTime_t(git_commit_time(data()));
    return dateTime;
}

int QGitCommit::timeOffset() const
{
    return git_commit_time_offset(data());
}

QGitSignature QGitCommit::committer() const
{
    return QGitSignature(git_commit_committer(data()));
}

QGitSignature QGitCommit::author() const
{
    return QGitSignature(git_commit_author(data()));
}

QGitTree QGitCommit::tree() const
{
    git_tree *tree;
    qGitThrow(git_commit_tree(&tree, data()));
    return QGitTree(tree);
}

unsigned int QGitCommit::parentCount() const
{
    return git_commit_parentcount(data());
}

QGitCommit QGitCommit::parent(unsigned n) const
{
    git_commit *parent;
    qGitThrow(git_commit_parent(&parent, data(), n));
    return QGitCommit(parent);
}

QGitOId QGitCommit::parentId(unsigned n) const
{
    return QGitOId( git_commit_parent_oid(data(), n) );
}

git_commit* QGitCommit::data() const
{
    return reinterpret_cast<git_commit*>(QGitObject::data());
}

const git_commit* QGitCommit::constData() const
{
    return reinterpret_cast<git_commit*>(QGitObject::data());
}

} // namespace LibQGit2
