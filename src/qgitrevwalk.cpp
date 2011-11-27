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
#include "qgitrepository.h"

#include <git2/revwalk.h>

using namespace LibQGit2;

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
    return git_revwalk_reset(m_revWalk);
}

int QGitRevWalk::push(const QGitOId& oid) const
{
    return git_revwalk_push(m_revWalk, oid.data());
}

int QGitRevWalk::hide(const QGitOId& oid) const
{
    return git_revwalk_hide(m_revWalk, oid.data());
}

QGitOId QGitRevWalk::next()
{
    QGitOId oid;
    git_revwalk_next(oid.data(), m_revWalk);
    return oid;
}

void QGitRevWalk::setSorting(SortModes sortMode)
{
    // wrap c defines
    unsigned int sort = GIT_SORT_NONE;
    if (sortMode | Time)
        sort |= GIT_SORT_TIME;
    if (sortMode | Topological)
        sort |= GIT_SORT_TOPOLOGICAL;
    if (sortMode | Reverse)
        sort |= GIT_SORT_REVERSE;

    git_revwalk_sorting(m_revWalk, sort);
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

