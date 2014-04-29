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

#include "qgitdiffdelta.h"
#include "qgitdifffile.h"

namespace LibQGit2 {

DiffDelta::DiffDelta(const git_diff_delta *d) : m_diff_delta(d)
{
}

DiffDelta::Type DiffDelta::type() const
{
    DiffDelta::Type ret = Unknown;
    if (m_diff_delta != NULL) {
        ret = DiffDelta::Type(m_diff_delta->status);
    }
    return ret;
}

DiffFile DiffDelta::oldFile() const
{
    return DiffFile(m_diff_delta != NULL ? &m_diff_delta->old_file : NULL);
}

DiffFile DiffDelta::newFile() const
{
    return DiffFile(m_diff_delta != NULL ? &m_diff_delta->new_file : NULL);
}

}
