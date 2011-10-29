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

#include "qgitrepository.h"

using namespace LibQGit2;

QGitRepository::QGitRepository(const QString& path, unsigned isBare, QObject* parent)
{
    git_repository_init(&m_repository, path.toAscii().constData(), isBare);
}

QGitRepository::QGitRepository(const git_repository *repository, QObject* parent)
    : m_repository(const_cast<git_repository *>(repository))
{
}

QGitRepository::QGitRepository( const QGitRepository& other )
{
    m_repository = other.m_repository;
}

QGitRepository::~QGitRepository()
{
    git_repository_free(m_repository);
}

int QGitRepository::open(const QString& path)
{
    return git_repository_open(&m_repository, path.toAscii().constData());
}

int QGitRepository::open2(const QString& gitDir,
                      const QString& gitObjectDirectory,
                      const QString& gitIndexFile,
                      const QString& gitWorkTree)
{
    return git_repository_open2(&m_repository, gitDir.toAscii().constData(),
                                gitObjectDirectory.toAscii().constData(), gitIndexFile.toAscii().constData(),
                                gitWorkTree.toAscii().constData());
}


int QGitRepository::open3(const QString& gitDir,
                      QGitDatabase *objectDatabase,
                      const QString& gitIndexFile,
                      const QString& gitWorkTree)
{
    return git_repository_open3(&m_repository, gitDir.toAscii().constData(), objectDatabase->data(),
                                gitIndexFile.toAscii().constData(), gitWorkTree.toAscii().constData());
}

QGitDatabase* QGitRepository::database() const
{
    QGitDatabase *database = new QGitDatabase(git_repository_database(m_repository));
    return database;
}

int QGitRepository::index(QGitIndex *index) const
{
    git_index *idx = index->data();
    return git_repository_index(&idx, m_repository);
}

git_repository* QGitRepository::data() const
{
    return m_repository;
}

const git_repository* QGitRepository::constData() const
{
    return const_cast<const git_repository *>(m_repository);
}

