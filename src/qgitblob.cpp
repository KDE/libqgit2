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

#include "qgitblob.h"
#include "qgitoid.h"
#include "qgitrepository.h"

#include <QtCore/QFile>

namespace LibQGit2
{

Blob::Blob(git_blob *blob)
    : Object(reinterpret_cast<git_object*>(blob))
{
}

Blob::Blob( const Blob& other )
    : Object(other)
{
}

Blob::~Blob()
{
}

const void* Blob::rawContent() const
{
    return git_blob_rawcontent(data());
}

QByteArray Blob::content() const
{
    return QByteArray::fromRawData( static_cast<const char *>(rawContent()), rawSize() );
}

int Blob::rawSize() const
{
    return git_blob_rawsize(data());
}

git_blob* Blob::data() const
{
    return reinterpret_cast<git_blob*>(Object::data());
}

const git_blob* Blob::constData() const
{
    return reinterpret_cast<git_blob*>(Object::data());
}

} // namespace LibQGit2
