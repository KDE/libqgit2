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

#include "revwalk.h"

#include "commit.h"
#include "repository.h"

using namespace LibQGit2;

RevWalk::RevWalk(Repository* repository, QObject* parent)
{
    git_revwalk_new(&m_revWalk, repository->data());
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
    return git_revwalk_reset(m_revWalk);
}

int RevWalk::push(Commit *commit) const
{
    return git_revwalk_push(m_revWalk, commit->data());
}

int RevWalk::hide(Commit *commit) const
{
    return git_revwalk_hide(m_revWalk, commit->data());
}

int RevWalk::next(Commit *commit)
{
    git_commit *c = commit->data();
    return git_revwalk_next(&c, m_revWalk);
}

int RevWalk::sorting(unsigned int sortMode)
{
    return git_revwalk_sorting(m_revWalk, sortMode);
}

Repository* RevWalk::repository()
{
    Repository *repository = new Repository(git_revwalk_repository(m_revWalk));
    return repository;
}

git_revwalk* RevWalk::data() const
{
    return m_revWalk;
}

const git_revwalk* RevWalk::constData() const
{
    return const_cast<const git_revwalk *>(m_revWalk);
}

