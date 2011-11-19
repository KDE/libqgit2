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

using namespace LibQGit2;

QGitCommit::QGitCommit(git_commit *commit)
    : m_commit(commit)
{
}

QGitCommit::QGitCommit( const QGitCommit& other )
{
    m_commit = other.m_commit;
}

QGitCommit::~QGitCommit()
{
}

void QGitCommit::reset(git_commit *commit)
{
    m_commit = commit;
}

int QGitCommit::lookup(const QGitRepository& repository, const QGitOId& oid)
{
    return git_commit_lookup(&m_commit, repository.data(), oid.data());
}

QGitOId QGitCommit::id() const
{
    return QGitOId(git_commit_id(m_commit));
}

QString QGitCommit::message() const
{
    return QString::fromUtf8(git_commit_message(m_commit));
}

QDateTime QGitCommit::dateTime() const
{
    QDateTime dateTime;
    dateTime.setTime_t(git_commit_time(m_commit));
    return dateTime;
}

int QGitCommit::timeOffset() const
{
    return git_commit_time_offset(m_commit);
}

QGitSignatureRef QGitCommit::committer() const
{
    return QGitSignatureRef(git_commit_committer(m_commit));
}

QGitSignatureRef QGitCommit::author() const
{
    return QGitSignatureRef(git_commit_author(m_commit));
}

QGitTree QGitCommit::tree() const
{
    git_tree *tree;
    git_commit_tree(&tree, m_commit);
    return QGitTree(tree);
}

unsigned int QGitCommit::parentCount() const
{
    return git_commit_parentcount(m_commit);
}

QGitCommit QGitCommit::parent(unsigned n) const
{
    git_commit *parent;
    git_commit_parent(&parent, m_commit, n);
    return QGitCommit(parent);
}

git_commit* QGitCommit::data() const
{
    return m_commit;
}

const git_commit* QGitCommit::constData() const
{
    return m_commit;
}

