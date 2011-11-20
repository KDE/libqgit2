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

#include "qgitoid.h"

using namespace LibQGit2;

QGitOId::QGitOId(const git_oid *oid)
    : d(GIT_OID_RAWSZ, 0)
{
    if (oid != 0) {
        git_oid_cpy(data(), oid);
    }
}

QGitOId::QGitOId(const QGitOId& other)
    : d(other.d)
{
}

QGitOId::~QGitOId()
{
}

bool LibQGit2::QGitOId::isValid() const
{
    return ( !d.isEmpty() && (d != QByteArray(GIT_OID_RAWSZ, '\0')) );
}

QGitOId QGitOId::fromString(const QByteArray& string)
{
    QGitOId oid;
    git_oid_fromstr(oid.data(), string.constData());
    return oid;
}

QGitOId QGitOId::fromRawData(const QByteArray& raw)
{
    Q_ASSERT(raw.size() == GIT_OID_RAWSZ);
    QGitOId oid;
    oid.d = raw;
    return oid;
}

QByteArray QGitOId::format() const
{
    QByteArray ba(GIT_OID_HEXSZ, Qt::Uninitialized);
    git_oid_fmt(ba.data(), data());
    return ba;
}

QByteArray QGitOId::pathFormat() const
{
    QByteArray ba(GIT_OID_HEXSZ+1, Qt::Uninitialized);
    git_oid_pathfmt(ba.data(), data());
    return ba;
}

git_oid* QGitOId::data()
{
    return reinterpret_cast<git_oid*>(d.data());
}

const git_oid* QGitOId::data() const
{
    return reinterpret_cast<const git_oid*>(d.constData());
}

const git_oid* QGitOId::constData() const
{
    return reinterpret_cast<const git_oid*>(d.constData());
}

bool operator==(const QGitOId& oid1, const QGitOId& oid2)
{
    return git_oid_cmp(oid1.data(), oid2.data()) == 0;
}
