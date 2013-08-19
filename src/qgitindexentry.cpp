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

#include "qgitindexentry.h"
#include "qgitoid.h"

#include <QtCore/QFile>

namespace LibQGit2
{

IndexEntry::IndexEntry(const git_index_entry *data)
    : d(data)
{
}

IndexEntry::IndexEntry(const IndexEntry& other)
    : d(other.d)
{
}

IndexEntry::~IndexEntry()
{
}

QGitOId IndexEntry::id() const
{
    return QGitOId(&d->oid);
}

QString IndexEntry::path() const
{
    return QFile::decodeName(d->path);
}

qint64 IndexEntry::fileSize() const
{
    return d->file_size;
}

const git_index_entry *IndexEntry::data() const
{
    return d;
}

} // namespace LibQGit2
