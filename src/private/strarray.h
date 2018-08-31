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

#ifndef LIBQGIT2_STRARRAY_H
#define LIBQGIT2_STRARRAY_H

#include <QList>
#include "git2.h"

namespace LibQGit2 {
namespace internal {

class StrArray
{
public:
    explicit StrArray(const QList<QByteArray> &list = QList<QByteArray>());
    StrArray(const StrArray &other) = delete;
    StrArray(StrArray &&other);
    ~StrArray();

    StrArray &operator=(const StrArray &rhs) = delete;
    StrArray &operator=(StrArray &&rhs);

    size_t count() const;

    const git_strarray& data() const;

private:
    QList<QByteArray> m_strings;
    git_strarray m_data{nullptr, 0};
};

}
}

#endif // LIBQGIT2_STRARRAY_H
