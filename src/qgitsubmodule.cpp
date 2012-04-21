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

QGitSubmodule::QGitSubmodule()
{
}

QString QGitSubmodule::getName() const
{
    return QString::fromUtf8(name);
}

QString QGitSubmodule::getPath() const
{
    QString tmpPath = QString::fromUtf8(path);
    if (tmpPath.isEmpty())
        return getName();

    return tmpPath;
}

QUrl QGitSubmodule::getUrl() const
{
    return QUrl::fromEncoded(url);
}

QGitOId QGitSubmodule::getOid() const
{
    return QGitOId(&oid);
}

bool QGitSubmodule::getFetchRecurseSubmodules() const
{
    //! @todo fetch_recurse is originally an int
    return fetch_recurse;
}

git_submodule_ignore_t QGitSubmodule::getIgnore() const
{
    return ignore;
}

} // namespace LibQGit2
