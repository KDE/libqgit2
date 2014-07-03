/******************************************************************************
 * This file is part of the libqgit2 library
 * Copyright (c) 2011 Laszlo Papp <djszapi@archlinux.us>
 * Copyright (C) 2013 Leonardo Giordani
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

#include <QtCore/QString>

#include "git2.h"

#include "libqgit2_config.h"

namespace LibQGit2
{
    /**
     * @brief Wrapper class for git_odb_backend.
     * Represents a Git object database backend.
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_EXPORT DatabaseBackend
    {
        public:
            DatabaseBackend();

            DatabaseBackend( const DatabaseBackend& other );

            ~DatabaseBackend();

        public:
            git_odb_backend* data() const;
            const git_odb_backend* constData() const;

        private:
            git_odb_backend *m_databaseBackend;
    };

    /**@}*/
}

#endif // LIBQGIT2_DATABASEBACKEND_H
