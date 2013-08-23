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

Status::Status(const git_status_t status_flags)
    : d(status_flags)
{
}

Status::Status(const Status &other)
    : d(other.d)
{
}

Status::~Status()
{
}

bool Status::isCurrent() const
{
    return d == GIT_STATUS_CURRENT;
}

bool Status::isNewInIndex() const
{
    return d & GIT_STATUS_INDEX_NEW;
}

bool Status::isModifiedInIndex() const
{
    return d & GIT_STATUS_INDEX_MODIFIED;
}

bool Status::isDeletedInIndex() const
{
    return d & GIT_STATUS_INDEX_DELETED;
}

bool Status::isRenamedInIndex() const
{
    return d & GIT_STATUS_INDEX_RENAMED;
}

bool Status::isTypeChangedInIndex() const
{
    return d & GIT_STATUS_INDEX_TYPECHANGE;
}

bool Status::isNewInWorkdir() const
{
    return d & GIT_STATUS_WT_NEW;
}

bool Status::isModifiedInWorkdir() const
{
    return d & GIT_STATUS_WT_MODIFIED;
}

bool Status::isDeletedInWorkdir() const
{
    return d & GIT_STATUS_WT_DELETED;
}

bool Status::isRenamedInWorkdir() const
{
    return d & GIT_STATUS_WT_RENAMED;
}

bool Status::isTypeChangedInWorkdir() const
{
    return d & GIT_STATUS_WT_TYPECHANGE;
}

unsigned int Status::data() const
{
    return (unsigned int)d;
}


}
