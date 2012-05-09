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

#include <git2/submodule.h>

#include <QtCore/QList>
#include <QtCore/QSharedPointer>

class QString;
class QUrl;


namespace LibQGit2
{
    class QGitSubmodule;
    typedef QList<QGitSubmodule> QGitSubmoduleList; //!< Defines a List of submodules

    class QGitOId;
    class QGitRepository;

    /**
     * @brief Wrapper class for git_submodule.
     * Represents a Git submodule object.
     *
     * @ingroup LibQGit2
     * @{
     */
    class QGitSubmodule
    {
    public:
        explicit QGitSubmodule(git_submodule *submodule = 0);
        QGitSubmodule(const QGitSubmodule &other);
        virtual ~QGitSubmodule();

        /**
          * @return the name of the submodule (usually the same as path)
          */
        QString getName() const;

        /**
          * @return the relative path to the submodule's working directory (usually the same as name)
          */
        QString getPath() const;

        /**
          * @return the url to the linked repository
          */
        QUrl getUrl() const;

        /**
          * @return the oid to the currently checked out commit
          */
        QGitOId getOid() const;

        /**
          * @return the update enum
          */
        git_submodule_update_t getUpdate() const;

        bool getFetchRecurseSubmodules() const;
        git_submodule_ignore_t getIgnore() const;

    private:
        QSharedPointer<git_submodule>     d;

    };

    /**@}*/
}

#endif // QGITSUBMODULE_H
