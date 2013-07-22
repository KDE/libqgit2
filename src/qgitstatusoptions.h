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

#include "libqgit2_export.h"

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
class LIBQGIT2_STATUS_EXPORT QGitStatusOptions
{
public:
    explicit QGitStatusOptions();

    explicit QGitStatusOptions(git_status_options status_options);

    QGitStatusOptions(const QGitStatusOptions& other);

    ~QGitStatusOptions();

    /**
     * True if show option is GIT_STATUS_SHOW_INDEX_AND_WORKDIR
     */
    bool showsIndexAndWorkdir() const;

    /**
     * True if show option is GIT_STATUS_SHOW_INDEX_ONLY
     */
    bool showsOnlyIndex() const;

    /**
     * True if show option is GIT_STATUS_SHOW_WORKDIR_ONLY
     */
    bool showsOnlyWorkdir() const;

    /**
     * Set the show option to GIT_STATUS_SHOW_INDEX_AND_WORKDIR
     */
    void setShowIndexAndWorkdir();

    /**
     * Set the show option to GIT_STATUS_SHOW_INDEX_ONLY
     */
    void setShowOnlyIndex();

    /**
     * Set the show option to GIT_STATUS_SHOW_WORKDIR_ONLY
     */
    void setShowOnlyWorkdir();
    
    /**
     * Get the GIT_STATUS_OPT_INCLUDE_UNTRACKED option
     */
    bool untrackedIncluded() const;

    /**
     * Set the GIT_STATUS_OPT_INCLUDE_UNTRACKED option
     */
    void setUntrackedIncluded(bool value);

    /**
     * Get the GIT_STATUS_OPT_INCLUDE_IGNORED option
     */
    bool ignoredIncluded() const;

    /**
     * Set the GIT_STATUS_OPT_INCLUDE_IGNORED option
     */
    void setIgnoredIncluded(bool value);

    /**
     * Get the GIT_STATUS_OPT_INCLUDE_UNMODIFIED option
     */
    bool unmodifiedIncluded() const;

    /**
     * Set the GIT_STATUS_OPT_INCLUDE_UNMODIFIED option
     */
    void setUnmodifiedIncluded(bool value);

    /**
     * Get the GIT_EXCLUDE_SUBMODULES option
     */
    bool submodulesExcluded() const;

    /**
     * Set the GIT_EXCLUDE_SUBMODULES option
     */
    void setSubmodulesExcluded(bool value);

    /**
     * Get the GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS option
     */
    bool untrackedDirsRecursed() const;

    /**
     * Set the GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS option
     */
    void setUntrackedDirsRecursed(bool value);

    /**
     * Get the GIT_STATUS_OPT_DISABLE_PATHSPEC_MATCH option
     */
    bool isPathspecMatchDisabled() const;

    /**
     * Set the GIT_STATUS_OPT_DISABLE_PATHSPEC_MATCH option
     */
    void setPathspecMatchDisabled(bool value);

    /**
     * Get the GIT_STATUS_OPT_RECURSE_IGNORED_DIRS option
     */
    bool ignoredDirsRecursed() const;

    /**
     * Set the GIT_STATUS_OPT_RECURSE_IGNORED_DIRS option
     */
    void setIgnoredDirsRecursed(bool value);

    /**
     * Get the GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX option
     */
    bool headToIndexRenames() const;

    /**
     * Set the GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX option
     */
    void setHeadtoIndexRenames(bool value);

    /**
     * Get the GIT_STATUS_OPT_RENAMES_INDEX_TO_WORKDIR option
     */
    bool indexToWorkdirRenames() const;

    /**
     * Set the GIT_STATUS_OPT_RENAMES_INDEX_TO_WORKDIR option
     */
    void setIndexToWorkdirRenames(bool value);

    /**
     * Get the GIT_STATUS_OPT_SORT_CASE_SENSITIVELY option
     */
    bool isSortCaseSensitively() const;

    /**
     * Set the GIT_STATUS_OPT_SORT_CASE_SENSITIVELY option
     */
    void setSortCaseSensitively(bool value);

    /**
     * Get the GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY option
     */
    bool isSortCaseInsensitively() const;

    /**
     * Set the GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY option
     */
    void setSortCaseInsensitively(bool value);

    git_status_options data() const;
    const git_status_options constData() const;
    
private:
    git_status_options d;
};


/**@}*/
}

#endif // LIBQGIT2_STATUS_OPTIONS_H
