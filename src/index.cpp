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

#include "index.h"

#include "repository.h"

using namespace LibQGit2;

Index::Index( const QString& indexPath, QObject* parent )
{
    git_index_open_bare(&m_index, indexPath.toAscii().data());
}

Index::Index(Repository* repository, QObject* parent )
{
    git_index_open_inrepo(&m_index, repository->data());
}

Index::Index( const Index& other )
{
    m_index = other.m_index;
}

Index::~Index()
{
    git_index_free(m_index);
}

void Index::clear()
{
    return git_index_clear(m_index);
}

int Index::read() const
{
    return git_index_read(m_index);
}

int Index::write()
{
    return git_index_write(m_index);
}

int Index::find(const QString& path)
{
    return git_index_find(m_index, path.toAscii().constData());
}

int Index::add(const QString& path, int stage)
{
    return git_index_add(m_index, path.toAscii().constData(), stage);
}

int Index::remove(int position)
{
    return git_index_remove(m_index, position);
}

int Index::insert(const git_index_entry *source_entry)
{
    return git_index_insert(m_index, source_entry);
}

git_index_entry* Index::get(int n) const
{
    return git_index_get(m_index, n);
}

unsigned int Index::entryCount() const
{
    return git_index_entrycount(m_index);
}

git_index* Index::data() const
{
    return m_index;
}

const git_index* Index::constData() const
{
    return const_cast<const git_index *>(m_index);
}
