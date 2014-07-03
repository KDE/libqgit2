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

#ifndef LIBQGIT2_STATUS_OPTIONS_H
#define LIBQGIT2_STATUS_OPTIONS_H

#include "git2.h"

#include "libqgit2_config.h"

namespace LibQGit2
{

/**
 * @brief Wrapper class for status options.
 *
 * You will find a complete description of status flags and options in git2/status.h
 * in the libgit2 code tree. This object avoid the coder to use the libgit2 defines to
 * get and set states and show modes.
 *
 * @ingroup LibQGit2
 * @{
 */
class LIBQGIT2_EXPORT StatusOptions
{
public:

    enum ShowType {
        ShowIndexAndWorkdir = GIT_STATUS_SHOW_INDEX_AND_WORKDIR,
        ShowOnlyIndex = GIT_STATUS_SHOW_INDEX_ONLY,
        ShowOnlyWorkdir = GIT_STATUS_SHOW_WORKDIR_ONLY
    };

    enum StatusFlag {
        IncludeUntracked = GIT_STATUS_OPT_INCLUDE_UNTRACKED,
        IncludeIgnored = GIT_STATUS_OPT_INCLUDE_IGNORED,
        IncludeUnmodified = GIT_STATUS_OPT_INCLUDE_UNMODIFIED,
        ExcludeSubmodules = GIT_STATUS_OPT_EXCLUDE_SUBMODULES,
        RecurseUntrackedDirs = GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS,
        DisablePathspecMatch = GIT_STATUS_OPT_DISABLE_PATHSPEC_MATCH,
        RecurseIgnoredDirs = GIT_STATUS_OPT_RECURSE_IGNORED_DIRS,
        RenamesHeadToIndex = GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX,
        RenamesIndexToWorkdir = GIT_STATUS_OPT_RENAMES_INDEX_TO_WORKDIR,
        SortCaseSensitively = GIT_STATUS_OPT_SORT_CASE_SENSITIVELY,
        SortCaseInsensitively = GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY
    };

    Q_DECLARE_FLAGS(StatusFlags, StatusFlag)

    explicit StatusOptions(ShowType showType = ShowIndexAndWorkdir, StatusFlags statusFlags = StatusFlags());

    explicit StatusOptions(git_status_options status_options);

    StatusOptions(const StatusOptions& other);

    ~StatusOptions();

    ShowType showType() const;

    void setShowType(ShowType type);

    StatusFlags statusFlags() const;

    void setStatusFlags(StatusOptions::StatusFlags sf);

    git_status_options data() const;
    const git_status_options constData() const;

private:
    git_status_options d;

    ShowType show_type;
    StatusFlags status_flags;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(StatusOptions::StatusFlags)


/**@}*/
}

#endif // LIBQGIT2_STATUS_OPTIONS_H
