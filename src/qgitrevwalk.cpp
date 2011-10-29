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

using namespace LibQGit2;

QGitRevWalk::QGitRevWalk(QGitRepository* repository, QObject* parent)
{
    git_revwalk_new(&m_revWalk, repository->data());
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

int QGitRevWalk::push(QGitCommit *commit) const
{
    return git_revwalk_push(m_revWalk, commit->data());
}

int QGitRevWalk::hide(QGitCommit *commit) const
{
    return git_revwalk_hide(m_revWalk, commit->data());
}

int QGitRevWalk::next(QGitCommit *commit)
{
    git_commit *c = commit->data();
    return git_revwalk_next(&c, m_revWalk);
}

int QGitRevWalk::sorting(unsigned int sortMode)
{
    return git_revwalk_sorting(m_revWalk, sortMode);
}

QGitRepository* QGitRevWalk::repository()
{
   QGitRepository *repository = new QGitRepository(git_revwalk_repository(m_revWalk));
    return repository;
}

git_revwalk* QGitRevWalk::data() const
{
    return m_revWalk;
}

const git_revwalk* QGitRevWalk::constData() const
{
    return const_cast<const git_revwalk *>(m_revWalk);
}

