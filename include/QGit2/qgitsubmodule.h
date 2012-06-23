/******************************************************************************
 * This file is part of the Gluon Development Platform
 * Copyright (c) 2012 Laszlo Papp <djszapi@archlinux.us>
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

#ifndef QGITSUBMODULE_H
#define QGITSUBMODULE_H

#include <libqgit2_export.h>

#include <src/qgitrepository.h>

#include <git2/submodule.h>

#include <QtCore/QList>

class QString;
class QUrl;

namespace LibQGit2
{
    class QGitOId;
    class QGitRepository;

    class QGitSubmodule;
    typedef QList<QGitSubmodule> QGitSubmoduleList; //!< Synonym for QList<QGitSubmodule>

    /**
     * @brief Wrapper class for git_submodule.
     * Represents a Git submodule object.
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_EXPORT QGitSubmodule
    {
    public:
        explicit QGitSubmodule(const QGitRepository &owner = QGitRepository(), git_submodule *submodule = 0);

        /**
          * Checks the pointer to the assigned submodule.
          * @return true, when the submodule is 0, false otherwise
          */
        bool isNull() const;

        /**
          * @return the name of the submodule (usually the same as path)
          */
        QString name() const;

        /**
          * @return the relative path to the submodule's working directory (usually the same as name)
          */
        QString path() const;

        /**
          * @return the url to the linked repository
          */
        QUrl url() const;

        /**
          * @return the oid to the currently checked out commit
          */
        QGitOId oid() const;

        /**
          * Opens the local repository, if it exists.
          *
          * @return true on success; false on failure
          */
        bool open();

        /**
          * @return the submodule's owner repository
          */
        const QGitRepository &owner() const;

        /**
          * @return the submodule's local repository; may be empty, when submodule not initialized
          */
        const QGitRepository &repository() const;

        /**
          * @return the update enum
          */
        git_submodule_update_t update() const;

        /**
          * @return fetch submodules recursively
          */
        bool fetchRecurseSubmodules() const;

        /**
          * @return the ignore flags
          */
        git_submodule_ignore_t ignore() const;

    public:
        /**
          * Creates a list with all submodules in a repository.
          */
        static QGitSubmoduleList list(const QGitRepository &repo);

    private:
        git_submodule *     d;

        QGitRepository      _owner; //!< the submodules owner repository
        QGitRepository      _repo;  //!< the submodules repository

    };

    /**@}*/
}

#endif // QGITSUBMODULE_H
