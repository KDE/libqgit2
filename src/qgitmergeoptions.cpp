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

#include "qgitmergeoptions.h"

namespace LibQGit2 {

MergeOptions::MergeOptions(FavorType favor, Flags flags)
{
    git_merge_options temp = GIT_MERGE_OPTIONS_INIT;
    d = temp;

    switch (favor) {
    case Normal:
        d.file_favor = GIT_MERGE_FILE_FAVOR_NORMAL;
        break;
    case Ours:
        d.file_favor = GIT_MERGE_FILE_FAVOR_OURS;
        break;
    case Theirs:
        d.file_favor = GIT_MERGE_FILE_FAVOR_THEIRS;
        break;
    case Union:
        d.file_favor = GIT_MERGE_FILE_FAVOR_UNION;
        break;
    }

    if (flags.testFlag(FindRenames)) {
        d.flags = GIT_MERGE_TREE_FIND_RENAMES;
    }
}

const git_merge_options* MergeOptions::data() const
{
    return &d;
}

}
