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

#include "git2.h"

#include "libqgit2_config.h"

namespace LibQGit2
{
/**
 * @brief Wrapper class for status flags.
 *
 * You will find a complete description of libgit status values in git2/status.h
 * in the libgit2 code tree.
 * Remember that Git status refers both to index and workdir, i.e. to modifications
 * you already staged (index) and to modifications made in the local workdir and not yet staged (workdir).
 *
 * @ingroup LibQGit2
 * @{
 */
class LIBQGIT2_EXPORT Status
{
public:
    explicit Status(const git_status_t status_flags);

    Status(const Status& other);

    ~Status();

    /**
     * Returns true if no changes are pending
     */
    bool isCurrent() const;

    /**
     * Returns true if the file is new in the index
     */
    bool isNewInIndex() const;

    /**
     * Returns true if the file has been modified in the index
     */
    bool isModifiedInIndex() const;

    /**
     * Returns true if the file has been deleted in the index
     */
    bool isDeletedInIndex() const;

    /**
     * Returns true if the file has been renamed in the index
     */
    bool isRenamedInIndex() const;

    /**
     * Returns true if the file type has been changed in the index
     */
    bool isTypeChangedInIndex() const;

    /**
     * Returns true if the file is new in the workdir
     */
    bool isNewInWorkdir() const;

    /**
     * Returns true if the file has been modified in the workdir
     */
    bool isModifiedInWorkdir() const;

    /**
     * Returns true if the file has been deleted in the workdir
     */
    bool isDeletedInWorkdir() const;

    /**
     * Returns true if the file has been renamed in the workdir
     */
    bool isRenamedInWorkdir() const;

    /**
     * Returns true if the file type has been changed in the workdir
     */
    bool isTypeChangedInWorkdir() const;

    unsigned int data() const;

private:
    git_status_t d;
};


/**@}*/
}

#endif // LIBQGIT2_STATUS_H
