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

#include "qgitrawobject.h"

#include "qgitdatabase.h"

using namespace LibQGit2;

QGitRawObject::QGitRawObject()
{
}

QGitRawObject::QGitRawObject(const QGitRawObject& other)
{
    m_rawObject = other.m_rawObject;
}

QGitRawObject::~QGitRawObject()
{
}

int QGitRawObject::read(QGitDatabase *db, const QGitOId& id)
{
    return git_odb_read(m_rawObject, db->data(), id.data());
}

int QGitRawObject::readHeader(QGitDatabase *db, const QGitOId& id)
{
    return git_odb_read_header(m_rawObject, db->data(), id.data());
}

int QGitRawObject::write(QGitOId &id, QGitDatabase *db)
{
    return git_odb_write(id.data(), db->data(), m_rawObject);
}

int QGitRawObject::hash(QGitOId &id)
{
    return git_rawobj_hash(id.data(), m_rawObject);
}

void QGitRawObject::close()
{
    return git_rawobj_close(m_rawObject);
}

git_rawobj* QGitRawObject::data() const
{
    return m_rawObject;
}

const git_rawobj* QGitRawObject::constData() const
{
    return m_rawObject;
}
