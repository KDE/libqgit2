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

#include "strarray.h"
#include <QtCore/QByteArray>

namespace LibQGit2 {
namespace internal {

static_assert(!std::is_copy_constructible<StrArray>::value, "StrArray must NOT be copy constructible");
static_assert(std::is_move_constructible<StrArray>::value, "StrArray must be move constructible");
static_assert(!std::is_copy_assignable<StrArray>::value, "StrArray must NOT be copy assignable");
static_assert(std::is_move_assignable<StrArray>::value, "StrArray must be move assignable");

StrArray::StrArray(const QList<QByteArray> &list) :
    m_strings(list)
{
    m_data.count = size_t(m_strings.size());

    if (m_data.count == 0) {
        return;
    }

    m_data.strings = new char*[m_data.count];
    for (size_t i = 0; i < m_data.count; ++i) {
        m_data.strings[i] = const_cast<char*>(m_strings.at(int(i)).data());
    }
}

StrArray::StrArray(StrArray &&other) :
    m_strings(std::move(other.m_strings)),
    m_data(other.m_data)
{
    other.m_data = {nullptr, 0};
}

StrArray::~StrArray()
{
    delete[] m_data.strings;
}

StrArray &StrArray::operator=(StrArray &&rhs)
{
    m_strings = std::move(rhs.m_strings);
    m_data = rhs.m_data;

    rhs.m_data = {nullptr, 0};

    return *this;
}

size_t StrArray::count() const
{
    return m_data.count;
}

const git_strarray& StrArray::data() const
{
    return m_data;
}

}
}
