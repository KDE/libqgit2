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

RevWalk::RevWalk(const Repository& repository)
{
    git_revwalk_new(&m_revWalk, repository.data());
    m_repository = &repository;
}

RevWalk::RevWalk( const RevWalk& other )
{
    m_revWalk = other.m_revWalk;
}

RevWalk::~RevWalk()
{
    git_revwalk_free(m_revWalk);
}

void RevWalk::reset() const
{
    git_revwalk_reset(m_revWalk);
}

void RevWalk::push(const OId& oid) const
{
    qGitThrow(git_revwalk_push(m_revWalk, oid.constData()));
}

void RevWalk::push(const Commit& commit) const
{
    qGitThrow(git_revwalk_push(m_revWalk, commit.oid().constData()));
}

void RevWalk::push(const Reference& reference) const
{
    qGitThrow(git_revwalk_push(m_revWalk, reference.target().constData()));
}

void RevWalk::push(const QString& glob) const
{
    qGitThrow(git_revwalk_push_glob(m_revWalk, glob.toUtf8().constData()));
}

void RevWalk::pushHead() const
{
    qGitThrow(git_revwalk_push_head(m_revWalk));
}

void RevWalk::pushRange(const QString& range) const
{
    qGitThrow(git_revwalk_push_range(m_revWalk,range.toUtf8().constData()));
}

void RevWalk::hide(const OId& oid) const
{
    qGitThrow(git_revwalk_hide(m_revWalk, oid.constData()));
}

void RevWalk::hide(const Commit& commit) const
{
    qGitThrow(git_revwalk_hide(m_revWalk, commit.oid().constData()));
}

void RevWalk::hide(const Reference& reference) const
{
    qGitThrow(git_revwalk_hide_glob(m_revWalk, reference.name().toUtf8().data()));
}

void RevWalk::hide(const QString& glob) const
{
    qGitThrow(git_revwalk_hide_glob(m_revWalk, glob.toUtf8().data()));
}

void RevWalk::hideHead() const
{
    qGitThrow(git_revwalk_hide_head(m_revWalk));
}

bool RevWalk::next(OId& oid) const
{
    int err = git_revwalk_next(oid.data(), m_revWalk);
    return (err == GIT_OK);
}

bool RevWalk::next(Commit& commit)
{
    OId oid;
    int err = git_revwalk_next(oid.data(), m_revWalk);

    if ( (err != GIT_OK) || !oid.isValid() )
        commit = Commit();
    else
        commit = constRepository()->lookupCommit(oid);

    return !commit.isNull();
}

void RevWalk::setSorting(SortModes sm)
{
    git_revwalk_sorting(m_revWalk, sm);
}

Repository* RevWalk::repository()
{
    Repository* repo = new Repository(git_revwalk_repository(m_revWalk));
    return repo;
}

const Repository* RevWalk::constRepository()
{
    return m_repository;
}

git_revwalk* RevWalk::data() const
{
    return m_revWalk;
}

const git_revwalk* RevWalk::constData() const
{
    return m_revWalk;
}

} // namespace LibQGit2
