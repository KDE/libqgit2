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
#include "qgitoid.h"
#include "qgitrepository.h"

#include <QtCore/QFile>

using namespace LibQGit2;

QGitBlob::QGitBlob(git_blob *blob)
    : QGitObject(reinterpret_cast<git_object*>(blob))
{
}

QGitBlob::QGitBlob( const QGitBlob& other )
    : QGitObject(other)
{
}

QGitBlob::~QGitBlob()
{
}

const void* QGitBlob::rawContent() const
{
    return git_blob_rawcontent(data());
}

int QGitBlob::rawSize() const
{
    return git_blob_rawsize(data());
}

int QGitBlob::writeFile(QGitOId& writtenId, const QGitRepository& repository, const QString& path)
{
    return git_blob_create_fromfile(writtenId.data(), repository.data(), QFile::encodeName(path));
}

git_blob* QGitBlob::data() const
{
    return reinterpret_cast<git_blob*>(QGitObject::data());
}

const git_blob* QGitBlob::constData() const
{
    return reinterpret_cast<git_blob*>(QGitObject::data());
}

