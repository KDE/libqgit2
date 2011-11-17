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

#include "qgitrefs.h"

#include "qgitoid.h"
#include "qgitrepository.h"

#include <QtCore/QFile>

using namespace LibQGit2;

QGitRefs::QGitRefs(QGitRepository* repository)
{
}

QGitRefs::QGitRefs( const QGitRefs& other )
{
    m_reference = other.m_reference;
}

QGitRefs::~QGitRefs()
{
}

QGitOId QGitRefs::oid() const
{
    return QGitOId(git_reference_oid(m_reference));
}

QString QGitRefs::target() const
{
    return QString::fromUtf8(git_reference_target(m_reference));
}

git_rtype QGitRefs::type() const
{
    return git_reference_type(m_reference);
}

QString QGitRefs::name() const
{
    return QString::fromUtf8(git_reference_name(m_reference));
}

int QGitRefs::resolve(QGitRefs* resolvedRef)
{
    git_reference *reference = resolvedRef->data();
    return git_reference_resolve(&reference, m_reference);
}

QGitRepository* QGitRefs::owner()
{
    QGitRepository* repository = new QGitRepository(git_reference_owner(m_reference));
    return repository;
}

int QGitRefs::setTarget(const QString& target)
{
    return git_reference_set_target(m_reference, QFile::encodeName(target));
}

void QGitRefs::setOId(const QGitOId& oid)
{
    git_reference_set_oid(m_reference, oid.data());
}

git_reference* QGitRefs::data() const
{
    return m_reference;
}

const git_reference* QGitRefs::constData() const
{
    return const_cast<const git_reference *>(m_reference);
}

