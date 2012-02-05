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

#include "qgitdatabase.h"

#include <QtCore/QFile>

#include <git2/odb.h>

namespace LibQGit2
{

QGitDatabase::QGitDatabase(git_odb *odb)
    : m_database(odb)
{
}

QGitDatabase::QGitDatabase( const QGitDatabase& other )
{
    m_database = other.m_database;
}

QGitDatabase::~QGitDatabase()
{
}

int QGitDatabase::open(const QString& objectsDir)
{
    return git_odb_open(&m_database, QFile::encodeName(objectsDir));
}

void QGitDatabase::close()
{
    return git_odb_free(m_database);
}

int QGitDatabase::addBackend(QGitDatabaseBackend *backend, int priority)
{
    return git_odb_add_backend(m_database, (git_odb_backend *)backend, priority);
}

int QGitDatabase::addAlternate(QGitDatabaseBackend *backend, int priority)
{
    return git_odb_add_alternate(m_database, (git_odb_backend *)backend, priority);
}

int QGitDatabase::exists(QGitDatabase *db, const QGitOId& id)
{
    return git_odb_exists(db->data(), id.constData());
}

git_odb* QGitDatabase::data() const
{
    return m_database;
}

const git_odb* QGitDatabase::constData() const
{
    return m_database;
}

} // namespace LibQGit2
