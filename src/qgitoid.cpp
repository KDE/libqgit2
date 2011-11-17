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

QGitOId::QGitOId( const QGitOId& other )
{
    m_oid = other.m_oid;
}

QGitOId::~QGitOId()
{
}

void QGitOId::reset(const git_oid *oid)
{
    if (oid != 0) {
        QSharedPointer<git_oid> dup(new git_oid);
        git_oid_cpy(dup.data(), oid);
        m_oid = dup;
    } else {
        m_oid.clear();
    }
}

QGitOId QGitOId::fromString(const QByteArray& string)
{
    git_oid *oid = new git_oid;
    git_oid_mkstr(oid, string.constData());
    return QGitOId(oid);
}

//void QGitOId::makeRaw(const unsigned char *raw)
//{
//    return git_oid_mkraw(m_oid, raw);
//}

QByteArray QGitOId::format() const
{
    if (isNull())
        return QByteArray();

    QByteArray ba(40, Qt::Uninitialized);
    git_oid_fmt(ba.data(), m_oid.data());
    return ba;
}

void QGitOId::pathFormat(char* string)
{
    return git_oid_pathfmt(string, data());
}

int QGitOId::compare(const QGitOId& oid)
{
    return git_oid_cmp(data(), oid.data());
}

const git_oid* QGitOId::data() const
{
    return m_oid.data();
}

//const git_oid* QGitOId::constData() const
//{
//    return m_oid.data();
//}

bool QGitOId::isNull() const
{
    return m_oid.isNull();
}
