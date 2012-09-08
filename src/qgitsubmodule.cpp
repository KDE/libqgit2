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
int addToSubmoduleList(git_submodule *sm, const char *name, void *payload)
{
    Q_ASSERT(payload != 0);

    QGitPrivateSubmoduleLookupInfo * submoduleInfo = static_cast<QGitPrivateSubmoduleLookupInfo *>(payload);

    git_submodule *submodule;
    int err = git_submodule_lookup(&submodule, submoduleInfo->repo.data(), name);
    if ( err == GIT_OK )
        submoduleInfo->submodules.append(LibQGit2::QGitSubmodule(submodule));

    return err;
}

} // internal namespace


namespace LibQGit2
{

QGitSubmodule::QGitSubmodule(git_submodule *submodule)
    : d(submodule)
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
    return QString::fromUtf8( git_submodule_name(d) );
}

QString QGitSubmodule::path() const
{
    return QString::fromUtf8( git_submodule_path(d) );
}

QUrl QGitSubmodule::url() const
{
    return QUrl::fromEncoded( git_submodule_url(d) );
}

QGitOId QGitSubmodule::oid() const
{
    return QGitOId( git_submodule_head_oid(d) );
}

QGitRepository QGitSubmodule::owner() const
{
    return QGitRepository( git_submodule_owner(d) );
}

const QGitRepository &QGitSubmodule::repository() const
{
    return _repo;
}

bool QGitSubmodule::fetchRecurseSubmodules() const
{
    return git_submodule_fetch_recurse_submodules(d);
}

void QGitSubmodule::setFetchRecurseSubmodules(bool yes)
{
    git_submodule_set_fetch_recurse_submodules(d, yes);
}

git_submodule_update_t QGitSubmodule::update() const
{
    return git_submodule_update(d);
}

git_submodule_update_t QGitSubmodule::setUpdate(git_submodule_update_t updateStrategy)
{
    return git_submodule_set_update(d, updateStrategy);
}

git_submodule_ignore_t QGitSubmodule::ignore() const
{
    return git_submodule_ignore(d);
}

git_submodule_ignore_t QGitSubmodule::setIgnore(git_submodule_ignore_t ignoreStrategy)
{
    return git_submodule_set_ignore(d, ignoreStrategy);
}

QGitSubmoduleList QGitSubmodule::list(const QGitRepository &repo)
{
    QGitPrivateSubmoduleLookupInfo submoduleInfo( repo );
    git_submodule_foreach(repo.data(), &addToSubmoduleList, &submoduleInfo);
    return submoduleInfo.submodules;
}

bool QGitSubmodule::open()
{
    // already open?
    if (!_repo.isNull())
        return true;

    git_repository *submodule_repo = 0;
    int ret = git_submodule_open(&submodule_repo, d);
    if (ret != GIT_OK)
        return false;

    _repo = QGitRepository(submodule_repo, true);
    return true;
}

} // namespace LibQGit2
