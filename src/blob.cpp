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

#include "blob.h"

using namespace LibQGit2;

Blob::Blob(Repository* repository, QObject* parent )
{
    git_blob_new(&m_blob, repository->data());
}

Blob::Blob( const Blob& other )
{
    m_blob = other.m_blob;
}

Blob::~Blob()
{
}

int Blob::lookup(Repository* repository, const OId& oid)
{
    return git_blob_lookup(&m_blob, repository->data(), oid.constData());
}

const char* Blob::rawContent()
{
    return git_blob_rawcontent(m_blob);
}

int Blob::rawSize()
{
    return git_blob_rawsize(m_blob);
}

int Blob::setRawContentFromFile(const QString& fileName)
{
    return git_blob_set_rawcontent_fromfile(m_blob, fileName.toAscii().constData());
}

int Blob::setRawContent(const void* buffer, size_t len)
{
    return git_blob_set_rawcontent(m_blob, buffer, len);
}

int Blob::writeFile(OId *writtenId, Repository *repository, const QString& path)
{
    return git_blob_writefile(writtenId->data(), repository->data(), path.toAscii().constData());
}

git_blob* Blob::data() const
{
    return m_blob;
}

const git_blob* Blob::constData() const
{
    return const_cast<const git_blob *>(m_blob);
}

