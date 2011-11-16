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

#include "qgitobject.h"

#include "qgitoid.h"
#include "qgitrepository.h"

using namespace LibQGit2;

QGitObject::QGitObject(const git_object *object)
    : m_object(const_cast<git_object *>(object))
{
}

QGitObject::QGitObject( const QGitObject& other )
{
    m_object = other.m_object;
}

QGitObject::~QGitObject()
{
}

int QGitObject::write()
{
    return git_object_write(m_object);
}

QGitOId QGitObject::id() const
{
    return QGitOId(git_object_id(m_object));
}

git_otype QGitObject::type() const
{
    return git_object_type(m_object);
}

QGitRepository* QGitObject::owner() const
{
    QGitRepository *repository = new QGitRepository(git_object_owner(m_object));
    return repository;
}

QString QGitObject::typeToString(git_otype type)
{
    return QString(git_object_type2string(type));
}

git_otype QGitObject::stringToType(const QString& string)
{
    return git_object_string2type(string.toAscii().constData());
}

int QGitObject::typeIsLoose(git_otype type)
{
    return git_object_typeisloose(type);
}

size_t QGitObject::size(git_otype type)
{
    return git_object__size(type);
}

git_object* QGitObject::data() const
{
    return m_object;
}

const git_object* QGitObject::constData() const
{
    return const_cast<const git_object *>(m_object);
}




