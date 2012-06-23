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
#include <qgitexception.h>
#include <qgiterror.h>

namespace
{

/**
  * Private struct to lookup submodule information.
  */
struct QGitPrivateSubmoduleLookupInfo
{
    const LibQGit2::QGitRepository &    repo;       //!< the owner repository to lookup submodules; must be open
    LibQGit2::QGitSubmoduleList         submodules; //!< the resulting list of submodules

    QGitPrivateSubmoduleLookupInfo(const LibQGit2::QGitRepository &owner) : repo(owner)
    {
    }
};

/**
  * Internal callback for git_submodule_foreach to retreive a list of submodules.
  */
int addToSubmoduleList(const char *name, void *payload)
{
    Q_ASSERT(payload != 0);

    QGitPrivateSubmoduleLookupInfo * submoduleInfo = static_cast<QGitPrivateSubmoduleLookupInfo *>(payload);

    git_submodule *submodule;
    int err = git_submodule_lookup(&submodule, submoduleInfo->repo.data(), name);
    if ( err == GIT_OK )
        submoduleInfo->submodules.append(LibQGit2::QGitSubmodule(submoduleInfo->repo, submodule));

    return err;
}

} // internal namespace


namespace LibQGit2
{

QGitSubmodule::QGitSubmodule(const QGitRepository &owner, git_submodule *submodule)
    : d(submodule)
    , _owner(owner)
{
    if ( !isNull() )
        open();
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

const QGitRepository &QGitSubmodule::owner() const
{
    return _owner;
}

const QGitRepository &QGitSubmodule::repository() const
{
    return _repo;
}

git_submodule_update_t QGitSubmodule::update() const
{
    return d->update;
}

bool QGitSubmodule::fetchRecurseSubmodules() const
{
    return d->fetch_recurse == 1;
}

git_submodule_ignore_t QGitSubmodule::ignore() const
{
    return d->ignore;
}

QGitSubmoduleList QGitSubmodule::list(const QGitRepository &repo)
{
    QGitPrivateSubmoduleLookupInfo submoduleInfo( repo );
    git_submodule_foreach(repo.data(), &addToSubmoduleList, &submoduleInfo);
    return submoduleInfo.submodules;
}

bool QGitSubmodule::open()
{
    if ( _owner.isBare() || _owner.isEmpty() )
        return false;

    // already open?
    if (!_repo.isNull())
        return true;

    const QString absPath = QString("%1/%2").arg(_owner.workDirPath()).arg(path());
    return _repo.discoverAndOpen(absPath, false, QStringList() << absPath);
}

} // namespace LibQGit2
