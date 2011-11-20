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

#include "qgitref.h"

#include "qgitoid.h"
#include "qgitrepository.h"

#include <QtCore/QFile>

#include <git2/refs.h>

using namespace LibQGit2;

QGitRef::QGitRef(git_reference *ref)
    : m_reference(ref)
{
}

QGitRef::QGitRef(const QGitRef& other)
    : m_reference(other.m_reference)
{
}

QGitRef::~QGitRef()
{
}

QGitOId QGitRef::oid() const
{
    return QGitOId(git_reference_oid(m_reference));
}

QString QGitRef::target() const
{
    return QString::fromUtf8(git_reference_target(m_reference));
}

bool QGitRef::isDirect() const
{
    return git_reference_type(m_reference) == GIT_REF_OID;
}

bool QGitRef::isSymbolic() const
{
    return git_reference_type(m_reference) == GIT_REF_SYMBOLIC;
}

QString QGitRef::name() const
{
    return QString::fromUtf8(git_reference_name(m_reference));
}

QGitRef QGitRef::resolve() const
{
    git_reference *ref;
    git_reference_resolve(&ref, m_reference);
    return QGitRef(ref);
}

QGitRepository QGitRef::owner()
{
    return QGitRepository(git_reference_owner(m_reference));
}

int QGitRef::setTarget(const QString& target)
{
    return git_reference_set_target(m_reference, QFile::encodeName(target));
}

void QGitRef::setOId(const QGitOId& oid)
{
    git_reference_set_oid(m_reference, oid.data());
}

git_reference* QGitRef::data() const
{
    return m_reference;
}

const git_reference* QGitRef::constData() const
{
    return m_reference;
}

