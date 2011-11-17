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

#include "qgitblob.h"

#include <QtCore/QFile>

using namespace LibQGit2;

QGitBlob::QGitBlob(QGitRepository* repository)
{
    git_blob_new(&m_blob, repository->data());
}

QGitBlob::QGitBlob( const QGitBlob& other )
{
    m_blob = other.m_blob;
}

QGitBlob::~QGitBlob()
{
}

int QGitBlob::lookup(QGitRepository* repository, const QGitOId& oid)
{
    return git_blob_lookup(&m_blob, repository->data(), oid.data());
}

const void* QGitBlob::rawContent()
{
    return git_blob_rawcontent(m_blob);
}

int QGitBlob::rawSize()
{
    return git_blob_rawsize(m_blob);
}

int QGitBlob::setRawContentFromFile(const QString& fileName)
{
    return git_blob_set_rawcontent_fromfile(m_blob, QFile::encodeName(fileName));
}

int QGitBlob::setRawContent(const void* buffer, size_t len)
{
    return git_blob_set_rawcontent(m_blob, buffer, len);
}

int QGitBlob::writeFile(QGitOId &writtenId, QGitRepository *repository, const QString& path)
{
    git_oid oid;
    int ret = git_blob_writefile(&oid, repository->data(), QFile::encodeName(path));
    writtenId.reset(&oid);
    return ret;
}

git_blob* QGitBlob::data() const
{
    return m_blob;
}

const git_blob* QGitBlob::constData() const
{
    return const_cast<const git_blob *>(m_blob);
}

