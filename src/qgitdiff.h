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

#ifndef LIBQGIT2_DIFF_H
#define LIBQGIT2_DIFF_H

#include <QSharedPointer>

#include "git2.h"

#include "libqgit2_export.h"

namespace LibQGit2
{

class DiffDelta;

class LIBQGIT2_EXPORT Diff
{
public:
    Diff(git_diff *diff = 0);

    size_t numDeltas() const;

    DiffDelta delta(size_t index) const;

public:
    QSharedPointer<git_diff> d;
};

}

#endif // LIBQGIT2_DIFF_H
