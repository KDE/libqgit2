/******************************************************************************
 * This file is part of the libqgit2 library
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

#ifndef LIBQGIT2_DIFFDELTA_H
#define LIBQGIT2_DIFFDELTA_H

#include "libqgit2_config.h"

#include "git2.h"

namespace LibQGit2 {

class DiffFile;

/**
 * @brief Wrapper class for git_diff_delta.
 *
 * @ingroup LibQGit2
 * @{
 */
class LIBQGIT2_EXPORT DiffDelta
{
public:
    DiffDelta(const git_diff_delta *d);

    enum Type {
        Unknown = GIT_DELTA_UNMODIFIED - 1,  ///< unknown type
        Unmodified = GIT_DELTA_UNMODIFIED,   ///< no changes
        Added = GIT_DELTA_ADDED,             ///< entry does not exist in old version
        Deleted = GIT_DELTA_DELETED,         ///< entry does not exist in new version
        Modified = GIT_DELTA_MODIFIED,       ///< entry content changed between old and new
        Renamed = GIT_DELTA_RENAMED,         ///< entry was renamed between old and new
        Copied = GIT_DELTA_COPIED,           ///< entry was copied from another old entry
        Ignored = GIT_DELTA_IGNORED,         ///< entry is ignored item in workdir
        Untracked = GIT_DELTA_UNTRACKED,     ///< entry is untracked item in workdir
        Typechange = GIT_DELTA_TYPECHANGE,   ///< type of entry changed between old and new
    };

    /**
     * Gets the type of this delta.
     */
    Type type() const;

    /**
     * Returns the information of the file on the "old" side of the diff.
     */
    DiffFile oldFile() const;

    /**
     * Returns the information of the file on the "new" side of the diff.
     */
    DiffFile newFile() const;

private:
    const git_diff_delta *m_diff_delta;
};

/** @} */

}

#endif // LIBQGIT2_DIFFDELTA_H
