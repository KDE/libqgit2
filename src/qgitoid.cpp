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

QGitOId::QGitOId( const git_oid *oid, QObject* parent )
    : m_oid(const_cast<git_oid *>(oid))
{
}

QGitOId::QGitOId( const QGitOId& other )
{
    m_oid = other.m_oid;
}

QGitOId::~QGitOId()
{
}

int QGitOId::makeString(const QByteArray& string)
{
    return git_oid_mkstr(m_oid, string.constData());
}

void QGitOId::makeRaw(const unsigned char *raw)
{
    return git_oid_mkraw(m_oid, raw);
}

void QGitOId::format(char* string)
{
    return git_oid_fmt(string, m_oid);
}

void QGitOId::pathFormat(char* string)
{
    return git_oid_pathfmt(string, m_oid);
}

char* QGitOId::allocFormat()
{
    return git_oid_allocfmt(m_oid);
}

char* QGitOId::toString(char* out, size_t n)
{
    return git_oid_to_string(out, n, m_oid);
}

int QGitOId::compare(QGitOId *oid)
{
    return git_oid_cmp(m_oid, oid->constData());
}

git_oid* QGitOId::data() const
{
    return m_oid;
}

const git_oid* QGitOId::constData() const
{
    return const_cast<const git_oid *>(m_oid);
}


