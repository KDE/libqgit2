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

#include "qgitdatabasebackend.h"

#include <QtCore/QFile>

using namespace LibQGit2;

QGitDatabaseBackend::QGitDatabaseBackend()
{
}

QGitDatabaseBackend::QGitDatabaseBackend( const QGitDatabaseBackend& other )
{
}

QGitDatabaseBackend::~QGitDatabaseBackend()
{
}

int QGitDatabaseBackend::pack(const QString& objectsDir)
{
    return git_odb_backend_pack(&m_databaseBackend, QFile::encodeName(objectsDir));
}

int QGitDatabaseBackend::loose(const QString& objectsDir)
{
    return git_odb_backend_loose(&m_databaseBackend, QFile::encodeName(objectsDir));
}

git_odb_backend* QGitDatabaseBackend::data() const
{
    return m_databaseBackend;
}

const git_odb_backend* QGitDatabaseBackend::constData() const
{
    return m_databaseBackend;
}
