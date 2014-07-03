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

#ifndef LIBQGIT2_STATUS_LIST_H
#define LIBQGIT2_STATUS_LIST_H

#include <QtCore/QSharedPointer>

#include "git2.h"

#include "libqgit2_config.h"
#include "qgitstatusentry.h"

namespace LibQGit2
{
/**
 * @brief Wrapper class for git_status_list.
 *
 * Represents a list of status entries in a Git repository. This is not a simple QList of StatusEntry,
 * it wraps the underlying libgit2 functions.
 *
 * @ingroup LibQGit2
 * @{
 */
class LIBQGIT2_EXPORT StatusList
{
public:
    explicit StatusList(git_status_list *status_list = 0);

    StatusList(const StatusList& other);

    ~StatusList();

    /**
     * Returns the number of entries in the status list.
     */
    size_t entryCount();

    /**
     * Returns the entry with the given index.
     */
    const StatusEntry entryByIndex(size_t idx);

    git_status_list* data() const;
    const git_status_list* constData() const;

private:
    typedef QSharedPointer<git_status_list> ptr_type;
    ptr_type d;

};


/**@}*/
}

#endif // LIBQGIT2_STATUS_LIST_H
