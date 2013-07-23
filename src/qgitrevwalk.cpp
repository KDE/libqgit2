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

#include "qgitrevwalk.h"

#include "qgitcommit.h"
#include "qgitexception.h"
#include "qgitrepository.h"
#include <iostream>

namespace LibQGit2
{

QGitRevWalk::QGitRevWalk(const QGitRepository& repository)
{
    git_revwalk_new(&m_revWalk, repository.data());
}

QGitRevWalk::QGitRevWalk( const QGitRevWalk& other )
{
    m_revWalk = other.m_revWalk;
}

QGitRevWalk::~QGitRevWalk()
{
    git_revwalk_free(m_revWalk);
}

void QGitRevWalk::reset() const
{
    git_revwalk_reset(m_revWalk);
}

void QGitRevWalk::push(const LibQGit2::QGitCommit& commit) const
{
    qGitThrow(git_revwalk_push(m_revWalk, commit.oid().data()));
}

void QGitRevWalk::pushHead() const
{
    qGitThrow(git_revwalk_push_head(m_revWalk));
}

void QGitRevWalk::hide(const QGitOId& oid) const
{
    qGitThrow(git_revwalk_hide(m_revWalk, oid.constData()));
}

QGitOId QGitRevWalk::next() const
{
    QGitOId oid;
    qGitThrow(git_revwalk_next(oid.data(), m_revWalk));
    return oid;
}

bool QGitRevWalk::next(QGitCommit & commit)
{
    QGitOId oid;
    int err = git_revwalk_next(oid.data(), m_revWalk);

    if ( (err != GIT_OK) || !oid.isValid() )
        commit = QGitCommit();
    else
        commit = repository().lookupCommit(oid);

    return !commit.isNull();
}

void QGitRevWalk::setSorting(SortModes sm)
{
    git_revwalk_sorting(m_revWalk, sm);
}

QGitRepository QGitRevWalk::repository()
{
    return QGitRepository(git_revwalk_repository(m_revWalk));
}

git_revwalk* QGitRevWalk::data() const
{
    return m_revWalk;
}

const git_revwalk* QGitRevWalk::constData() const
{
    return m_revWalk;
}

} // namespace LibQGit2
