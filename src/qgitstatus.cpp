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


#include <QtCore/QFile>

#include "qgitstatus.h"

namespace LibQGit2
{

QGitStatus::QGitStatus(const git_status_t status_flags)
    : d(status_flags)
{
}

QGitStatus::QGitStatus(const QGitStatus &other)
    : d(other.d)
{
}

QGitStatus::~QGitStatus()
{
}

bool QGitStatus::isCurrent() const
{
    return d == GIT_STATUS_CURRENT;
}

bool QGitStatus::isNewInIndex() const
{
    return d & GIT_STATUS_INDEX_NEW;
}

bool QGitStatus::isModifiedInIndex() const
{
    return d & GIT_STATUS_INDEX_MODIFIED;
}

bool QGitStatus::isDeletedInIndex() const
{
    return d & GIT_STATUS_INDEX_DELETED;
}

bool QGitStatus::isRenamedInIndex() const
{
    return d & GIT_STATUS_INDEX_RENAMED;
}

bool QGitStatus::isTypeChangedInIndex() const
{
    return d & GIT_STATUS_INDEX_TYPECHANGE;
}

bool QGitStatus::isNewInWorkdir() const
{
    return d & GIT_STATUS_WT_NEW;
}

bool QGitStatus::isModifiedInWorkdir() const
{
    return d & GIT_STATUS_WT_MODIFIED;
}

bool QGitStatus::isDeletedInWorkdir() const
{
    return d & GIT_STATUS_WT_DELETED;
}

bool QGitStatus::isRenamedInWorkdir() const
{
    return d & GIT_STATUS_WT_RENAMED;
}

bool QGitStatus::isTypeChangedInWorkdir() const
{
    return d & GIT_STATUS_WT_TYPECHANGE;
}

unsigned int QGitStatus::data() const
{
    return (unsigned int)d;
}


}
