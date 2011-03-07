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

#include "refs.h"

#include "oid.h"
#include "repository.h"

using namespace LibQGit2;

Refs::Refs(Repository* repository, QObject* parent)
{
}

Refs::Refs( const Refs& other )
{
    m_reference = other.m_reference;
}

Refs::~Refs()
{
}

const OId* Refs::oid() const
{
    const OId* oid = new OId(git_reference_oid(m_reference));
    return oid;
}

QString Refs::target() const
{
    return QString::fromUtf8(git_reference_target(m_reference));
}

git_rtype Refs::type() const
{
    return git_reference_type(m_reference);
}

QString Refs::name() const
{
    return QString::fromUtf8(git_reference_name(m_reference));
}

int Refs::resolve(Refs* resolvedRef)
{
    git_reference *reference = resolvedRef->data();
    return git_reference_resolve(&reference, m_reference);
}

Repository* Refs::owner()
{
    Repository* repository = new Repository(git_reference_owner(m_reference));
    return repository;
}

int Refs::setTarget(const QString& target)
{
    return git_reference_set_target(m_reference, target.toAscii().constData());
}

void Refs::setOId(const OId& oid)
{
    git_reference_set_oid(m_reference, oid.constData());
}

git_reference* Refs::data() const
{
    return m_reference;
}

const git_reference* Refs::constData() const
{
    return const_cast<const git_reference *>(m_reference);
}

