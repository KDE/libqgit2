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

#ifndef LIBQGIT2_DATABASEBACKEND_H
#define LIBQGIT2_DATABASEBACKEND_H

#include "libqgit2_export.h"

#include <QtCore/QString>

#include <git2/odb_backend.h>

namespace LibQGit2
{
    class LIBQGIT2_DATABASEBACKEND_EXPORT QGitDatabaseBackend
    {
        public:
            QGitDatabaseBackend( QObject* parent = 0 );

            QGitDatabaseBackend( const QGitDatabaseBackend& other );

            ~QGitDatabaseBackend();

        public:
            int pack(const QString& objectsDir);

            int loose(const QString& objectsDir);

            git_odb_backend* data() const;
            const git_odb_backend* constData() const;

        private:
            git_odb_backend *m_databaseBackend;
    };
}

#endif // LIBQGIT2_DATABASEBACKEND_H
