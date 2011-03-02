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

#include "oid.h"

using namespace LibQGit2;

OId::OId( const git_oid *oid, QObject* parent )
    : m_oid(const_cast<git_oid *>(oid))
{
}

OId::OId( const OId& other )
{
    m_oid = other.m_oid;
}

OId::~OId()
{
}

int OId::makeString(const QByteArray& string)
{
    return git_oid_mkstr(m_oid, string.constData());
}

void OId::makeRaw(const unsigned char *raw)
{
    return git_oid_mkraw(m_oid, raw);
}

void OId::format(char* string)
{
    return git_oid_fmt(string, m_oid);
}

void OId::pathFormat(char* string)
{
    return git_oid_pathfmt(string, m_oid);
}

char* OId::allocFormat()
{
    return git_oid_allocfmt(m_oid);
}

char* OId::toString(char* out, size_t n)
{
    return git_oid_to_string(out, n, m_oid);
}

int OId::compare(OId *oid)
{
    return git_oid_cmp(m_oid, oid->constData());
}

git_oid* OId::data() const
{
    return m_oid;
}

const git_oid* OId::constData() const
{
    return const_cast<const git_oid *>(m_oid);
}


