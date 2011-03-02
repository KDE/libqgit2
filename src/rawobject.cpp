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

#include "rawobject.h"

#include "database.h"

using namespace LibQGit2;

RawObject::RawObject(QObject* parent)
{
}

RawObject::RawObject(const RawObject& other)
{
    m_rawObject = other.m_rawObject;
}

RawObject::~RawObject()
{
}

int RawObject::read(Database *db, const OId *id)
{
    return git_odb_read(m_rawObject, db->data(), id->constData());
}

int RawObject::readHeader(Database *db, const OId *id)
{
    return git_odb_read_header(m_rawObject, db->data(), id->constData());
}

int RawObject::write(OId *id, Database *db)
{
    return git_odb_write(id->data(), db->data(), m_rawObject);
}

int RawObject::hash(OId *id)
{
    return git_rawobj_hash(id->data(), m_rawObject);
}

void RawObject::close()
{
    return git_rawobj_close(m_rawObject);
}

git_rawobj* RawObject::data() const
{
    return m_rawObject;
}

const git_rawobj* RawObject::constData() const
{
    return const_cast<const git_rawobj *>(m_rawObject);
}
