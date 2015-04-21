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

#include "qgitdatabase.h"

#include "private/pathcodec.h"

namespace LibQGit2
{

Database::Database(git_odb *odb)
    : m_database(odb)
{
}

Database::Database( const Database& other )
{
    m_database = other.m_database;
}

Database::~Database()
{
}

int Database::open(const QString& objectsDir)
{
    return git_odb_open(&m_database, PathCodec::toLibGit2(objectsDir));
}

void Database::close()
{
    return git_odb_free(m_database);
}

int Database::addBackend(DatabaseBackend *backend, int priority)
{
    return git_odb_add_backend(m_database, (git_odb_backend *)backend, priority);
}

int Database::addAlternate(DatabaseBackend *backend, int priority)
{
    return git_odb_add_alternate(m_database, (git_odb_backend *)backend, priority);
}

int Database::exists(Database *db, const OId& id)
{
    return git_odb_exists(db->data(), id.constData());
}

git_odb* Database::data() const
{
    return m_database;
}

const git_odb* Database::constData() const
{
    return m_database;
}

} // namespace LibQGit2
