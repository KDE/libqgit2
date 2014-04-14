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

#include "libqgit2_export.h"

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
