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
    : d(submodule, free)
{
}

QGitSubmodule::QGitSubmodule(const QGitSubmodule &other)
    : d(other.d)
{
}

QGitSubmodule::~QGitSubmodule()
{
}

QString QGitSubmodule::getName() const
{
    if (d.isNull())
        return QString();

    return QString::fromUtf8(d->name);
}

QString QGitSubmodule::getPath() const
{
    if (d.isNull())
        return QString();

    QString tmpPath = QString::fromUtf8(d->path);
    if (tmpPath.isEmpty())
        return getName();

    return tmpPath;
}

QUrl QGitSubmodule::getUrl() const
{
    if (d.isNull())
        return QUrl();

    return QUrl::fromEncoded(d->url);
}

QGitOId QGitSubmodule::getOid() const
{
    if (d.isNull())
        return QGitOId();

    return QGitOId(&d->oid);
}

git_submodule_update_t QGitSubmodule::getUpdate() const
{
    if (d.isNull())
        return GIT_SUBMODULE_UPDATE_CHECKOUT;

    return d->update;
}

bool QGitSubmodule::getFetchRecurseSubmodules() const
{
    if (d.isNull())
        return false;

    //! @todo fetch_recurse is originally an int
    return d->fetch_recurse;
}

git_submodule_ignore_t QGitSubmodule::getIgnore() const
{
    if (d.isNull())
        return GIT_SUBMODULE_IGNORE_NONE;

    return d->ignore;
}

} // namespace LibQGit2
