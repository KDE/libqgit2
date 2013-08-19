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

#include <iostream>

#include "qgitrevwalk.h"
#include "qgitcommit.h"
#include "qgitref.h"
#include "qgitexception.h"
#include "qgitrepository.h"


namespace LibQGit2
{

QGitRevWalk::QGitRevWalk(const QGitRepository& repository)
{
    git_revwalk_new(&m_revWalk, repository.data());
    m_repository = &repository;
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

void QGitRevWalk::push(const QGitOId& oid) const
{
    qGitThrow(git_revwalk_push(m_revWalk, oid.constData()));
}

void QGitRevWalk::push(const QGitCommit& commit) const
{
    qGitThrow(git_revwalk_push(m_revWalk, commit.oid().constData()));
}

void QGitRevWalk::push(const Reference& reference) const
{
    qGitThrow(git_revwalk_push_glob(m_revWalk, reference.name().toUtf8().constData()));
}

void QGitRevWalk::push(const QString& glob) const
{
    qGitThrow(git_revwalk_push_glob(m_revWalk, glob.toUtf8().constData()));
}

void QGitRevWalk::pushHead() const
{
    qGitThrow(git_revwalk_push_head(m_revWalk));
}

void QGitRevWalk::pushRange(const QString& range) const
{
    qGitThrow(git_revwalk_push_range(m_revWalk,range.toUtf8().constData()));
}

void QGitRevWalk::hide(const QGitOId& oid) const
{
    qGitThrow(git_revwalk_hide(m_revWalk, oid.constData()));
}

void QGitRevWalk::hide(const QGitCommit& commit) const
{
    qGitThrow(git_revwalk_hide(m_revWalk, commit.oid().constData()));
}

void QGitRevWalk::hide(const Reference& reference) const
{
    qGitThrow(git_revwalk_hide_glob(m_revWalk, reference.name().toUtf8().data()));
}

void QGitRevWalk::hide(const QString& glob) const
{
    qGitThrow(git_revwalk_hide_glob(m_revWalk, glob.toUtf8().data()));
}

void QGitRevWalk::hideHead() const
{
    qGitThrow(git_revwalk_hide_head(m_revWalk));
}

bool QGitRevWalk::next(QGitOId& oid) const
{
    int err = git_revwalk_next(oid.data(), m_revWalk);
    return (err == GIT_OK);
}

bool QGitRevWalk::next(QGitCommit& commit)
{
    QGitOId oid;
    int err = git_revwalk_next(oid.data(), m_revWalk);

    if ( (err != GIT_OK) || !oid.isValid() )
        commit = QGitCommit();
    else
        commit = constRepository()->lookupCommit(oid);

    return !commit.isNull();
}

void QGitRevWalk::setSorting(SortModes sm)
{
    git_revwalk_sorting(m_revWalk, sm);
}

QGitRepository* QGitRevWalk::repository()
{
    QGitRepository* repo = new QGitRepository(git_revwalk_repository(m_revWalk));
    return repo;
}

const QGitRepository* QGitRevWalk::constRepository()
{
    return m_repository;
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
