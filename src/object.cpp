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

#include "object.h"

#include "oid.h"
#include "repository.h"

using namespace LibQGit2;

Object::Object( const git_object *object, QObject* parent )
    : m_object(const_cast<git_object *>(object))
{
}

Object::Object( const Object& other )
{
    m_object = other.m_object;
}

Object::~Object()
{
    git_object_free(m_object);
}

int Object::write()
{
    return git_object_write(m_object);
}

const OId* Object::id() const
{
    const OId *oid = new OId(git_object_id(m_object));
    return oid;
}

git_otype Object::type() const
{
    return git_object_type(m_object);
}

Repository* Object::owner() const
{
    Repository *repository = new Repository(git_object_owner(m_object));
    return repository;
}

QString Object::typeToString(git_otype type)
{
    return QString(git_object_type2string(type));
}

git_otype Object::stringToType(const QString& string)
{
    return git_object_string2type(string.toAscii().constData());
}

int Object::typeIsLoose(git_otype type)
{
    return git_object_typeisloose(type);
}

size_t Object::size(git_otype type)
{
    return git_object__size(type);
}

git_object* Object::data() const
{
    return m_object;
}

const git_object* Object::constData() const
{
    return const_cast<const git_object *>(m_object);
}




