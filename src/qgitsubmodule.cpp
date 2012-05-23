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

#include "qgitsubmodule.h"

#include <QtCore/QString>
#include <QtCore/QUrl>

#include <qgitoid.h>

namespace LibQGit2
{

QGitSubmodule::QGitSubmodule(git_submodule *submodule)
    : d(submodule)
{
}

QGitSubmodule::QGitSubmodule(const QGitSubmodule &other)
    : d(other.d)
{
}

QGitSubmodule::~QGitSubmodule()
{
}

bool QGitSubmodule::isNull() const
{
    return d == 0;
}

QString QGitSubmodule::name() const
{
    return QString::fromUtf8(d->name);
}

QString QGitSubmodule::path() const
{
    QString tmpPath = QString::fromUtf8(d->path);
    if (tmpPath.isEmpty())
        return name();

    return tmpPath;
}

QUrl QGitSubmodule::url() const
{
    return QUrl::fromEncoded(d->url);
}

QGitOId QGitSubmodule::oid() const
{
    return QGitOId(&d->oid);
}

git_submodule_update_t QGitSubmodule::update() const
{
    return d->update;
}

bool QGitSubmodule::fetchRecurseSubmodules() const
{
    return d->fetch_recurse;
}

git_submodule_ignore_t QGitSubmodule::ignore() const
{
    return d->ignore;
}

} // namespace LibQGit2
