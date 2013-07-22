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

#include "qgitstatusoptions.h"

namespace LibQGit2
{

QGitStatusOptions::QGitStatusOptions()
    : d(GIT_STATUS_OPTIONS_INIT)
{
}

QGitStatusOptions::QGitStatusOptions(git_status_options status_options)
    : d(status_options)
{
}

QGitStatusOptions::QGitStatusOptions(const QGitStatusOptions &other)
    : d(other.d)
{
}

QGitStatusOptions::~QGitStatusOptions()
{
}

bool QGitStatusOptions::showsIndexAndWorkdir() const
{
    return d.show & GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
}

bool QGitStatusOptions::showsOnlyIndex() const
{
    return d.show & GIT_STATUS_SHOW_INDEX_ONLY;
}

bool QGitStatusOptions::showsOnlyWorkdir() const
{
    return d.show & GIT_STATUS_SHOW_WORKDIR_ONLY;
}

void QGitStatusOptions::setShowIndexAndWorkdir()
{
    d.show = GIT_STATUS_SHOW_INDEX_AND_WORKDIR;
}

void QGitStatusOptions::setShowOnlyIndex()
{
    d.show = GIT_STATUS_SHOW_INDEX_ONLY;
}

void QGitStatusOptions::setShowOnlyWorkdir()
{
    d.show = GIT_STATUS_SHOW_WORKDIR_ONLY;
}

bool QGitStatusOptions::untrackedIncluded() const
{
    return d.flags & GIT_STATUS_OPT_INCLUDE_UNTRACKED;
}

void QGitStatusOptions::setUntrackedIncluded(bool value)
{
    if (value) {
        d.flags |= GIT_STATUS_OPT_INCLUDE_UNTRACKED;
    }
}

bool QGitStatusOptions::ignoredIncluded() const
{
    return d.flags & GIT_STATUS_OPT_INCLUDE_IGNORED;
}

void QGitStatusOptions::setIgnoredIncluded(bool value)
{
    if (value) {
        d.flags |= GIT_STATUS_OPT_INCLUDE_IGNORED;
    }
}

bool QGitStatusOptions::unmodifiedIncluded() const
{
    return d.flags & GIT_STATUS_OPT_INCLUDE_UNMODIFIED;
}

void QGitStatusOptions::setUnmodifiedIncluded(bool value)
{
    if (value) {
        d.flags |= GIT_STATUS_OPT_INCLUDE_UNMODIFIED;
    }
}

bool QGitStatusOptions::submodulesExcluded() const
{
    return d.flags & GIT_STATUS_OPT_EXCLUDE_SUBMODULES;
}

void QGitStatusOptions::setSubmodulesExcluded(bool value)
{
    if (value) {
        d.flags |= GIT_STATUS_OPT_EXCLUDE_SUBMODULES;
    }
}

bool QGitStatusOptions::untrackedDirsRecursed() const
{
    return d.flags & GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS;
}

void QGitStatusOptions::setUntrackedDirsRecursed(bool value)
{
    if (value) {
        d.flags |= GIT_STATUS_OPT_RECURSE_UNTRACKED_DIRS;
    }
}

bool QGitStatusOptions::isPathspecMatchDisabled() const
{
    return d.flags & GIT_STATUS_OPT_DISABLE_PATHSPEC_MATCH;
}

void QGitStatusOptions::setPathspecMatchDisabled(bool value)
{
    if (value) {
        d.flags |= GIT_STATUS_OPT_DISABLE_PATHSPEC_MATCH;
    }
}

bool QGitStatusOptions::ignoredDirsRecursed() const
{
    return d.flags & GIT_STATUS_OPT_RECURSE_IGNORED_DIRS;
}

void QGitStatusOptions::setIgnoredDirsRecursed(bool value)
{
    if (value) {
        d.flags |= GIT_STATUS_OPT_RECURSE_IGNORED_DIRS;
    }
}

bool QGitStatusOptions::headToIndexRenames() const
{
    return d.flags & GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX;
}

void QGitStatusOptions::setHeadtoIndexRenames(bool value)
{
    if (value) {
        d.flags |= GIT_STATUS_OPT_RENAMES_HEAD_TO_INDEX;
    }
}

bool QGitStatusOptions::indexToWorkdirRenames() const
{
    return d.flags & GIT_STATUS_OPT_RENAMES_INDEX_TO_WORKDIR;
}

void QGitStatusOptions::setIndexToWorkdirRenames(bool value)
{
    if (value) {
        d.flags |= GIT_STATUS_OPT_RENAMES_INDEX_TO_WORKDIR;
    }
}

bool QGitStatusOptions::isSortCaseSensitively() const
{
    return d.flags & GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;
}

void QGitStatusOptions::setSortCaseSensitively(bool value)
{
    if (value) {
        d.flags |= GIT_STATUS_OPT_SORT_CASE_SENSITIVELY;
    }
}

bool QGitStatusOptions::isSortCaseInsensitively() const
{
    return d.flags & GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY;
}

void QGitStatusOptions::setSortCaseInsensitively(bool value)
{
    if (value) {
        d.flags |= GIT_STATUS_OPT_SORT_CASE_INSENSITIVELY;
    }
}

git_status_options QGitStatusOptions::data() const
{
    return d;
}

const git_status_options QGitStatusOptions::constData() const
{
    return d;
}

}
