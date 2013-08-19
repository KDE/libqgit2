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

#include <QtCore/QFile>

#include "qgitref.h"

#include "qgitoid.h"
#include "qgitrepository.h"
#include "qgitexception.h"

namespace LibQGit2
{

Reference::Reference(git_reference *ref)
    : d(ref, git_reference_free)
{
}

Reference::Reference(const Reference& other)
    : d(other.d)
{
}

Reference::~Reference()
{
}

QGitOId Reference::target() const
{
    return QGitOId(git_reference_target(d.data()));
}

QString Reference::symbolicTarget() const
{
    return QString::fromUtf8(git_reference_symbolic_target(d.data()));
}

bool Reference::isDirect() const
{
    return git_reference_type(d.data()) == GIT_REF_OID;
}

bool Reference::isSymbolic() const
{
    return git_reference_type(d.data()) == GIT_REF_SYMBOLIC;
}

QString Reference::name() const
{
    return QString::fromUtf8(git_reference_name(d.data()));
}

Reference Reference::resolve() const
{
    git_reference *ref;
    qGitThrow(git_reference_resolve(&ref, d.data()));
    return Reference(ref);
}

QGitRepository Reference::owner() const
{
    return QGitRepository(git_reference_owner(d.data()));
}

void Reference::setSymbolicTarget(const QString& target)
{
    git_reference* rp;
    qGitThrow(git_reference_symbolic_set_target(&rp, data(), QFile::encodeName(target)));
    d = ptr_type(rp);
}

void Reference::setTarget(const QGitOId& oid)
{
    git_reference* rp;
    qGitThrow(git_reference_set_target(&rp, data(), oid.constData()));
    d = ptr_type(rp);
}

bool Reference::isNull() const
{
    return data() == 0;
}

git_reference* Reference::data() const
{
    return d.data();
}

const git_reference* Reference::constData() const
{
    return d.data();
}

} // namespace LibQGit2
