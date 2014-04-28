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

#include "qgitdiff.h"
#include "qgitdiffdelta.h"

namespace LibQGit2
{

Diff::Diff(git_diff *diff) :
    d(diff, git_diff_free)
{
}

size_t Diff::numDeltas() const
{
    size_t ret = 0;
    if (!d.isNull()) {
        ret = git_diff_num_deltas(d.data());
    }
    return ret;
}

DiffDelta Diff::delta(size_t index) const
{
    const git_diff_delta *delta = 0;
    if (!d.isNull()) {
        delta = git_diff_get_delta(d.data(), index);
    }
    return DiffDelta(delta);
}

}
