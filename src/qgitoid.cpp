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
{
    reset(oid);
}

QGitOId::QGitOId(const QGitOId& other)
{
    reset(other.data());
}

QGitOId::~QGitOId()
{
}

void QGitOId::reset(const git_oid *oid)
{
    git_oid_cpy(&m_oid, oid);
}

QGitOId QGitOId::fromString(const QByteArray& string)
{
    git_oid oid;
    git_oid_mkstr(&oid, string.constData());
    return QGitOId(&oid);
}

//void QGitOId::makeRaw(const unsigned char *raw)
//{
//    return git_oid_mkraw(m_oid, raw);
//}

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

const git_oid* QGitOId::data() const
{
    return &m_oid;
}

bool operator==(const QGitOId& oid1, const QGitOId& oid2)
{
    return git_oid_cmp(oid1.data(), oid2.data()) == 0;
}
