/******************************************************************************
 * This file is part of the libqgit2 library
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

#ifndef LIBQGIT2_STATUS_H
#define LIBQGIT2_STATUS_H

#include "qgitobject.h"

namespace LibQGit2
{
/**
 * @brief Wrapper class for git_status.
 * Represents the status of a Git repository.
 *
 * @ingroup LibQGit2
 * @{
 */
class LIBQGIT2_STATUS_EXPORT QGitStatus
{
public:

    explicit QGitStatus();

    QGitStatus(const QGitStatus& other);

    /**
     * Destroys the object.
     */
    ~QGitStatus();
};

/**@}*/
}

#endif // LIBQGIT2_STATUS_H
