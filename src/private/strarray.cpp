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

StrArray::StrArray()
{
    m_data.count = 0;
    m_data.strings = NULL;
}

StrArray::StrArray(const QList<QByteArray> &list) :
    m_strings(list)
{
    m_data.count = 0;
    m_data.strings = NULL;

    updateNative();
}

StrArray::~StrArray()
{
    free(m_data.strings);
}

void StrArray::set(const QList<QByteArray> &list)
{
    m_strings = list;
    updateNative();
}

size_t StrArray::count() const
{
    return m_data.count;
}

const git_strarray& StrArray::data() const
{
    return m_data;
}

void StrArray::updateNative()
{
    const size_t newCount = m_strings.size();
    if (newCount == 0) {
        free(m_data.strings);
        m_data.strings = NULL;
    } else if (newCount > m_data.count) {
        m_data.strings = (char**)realloc(m_data.strings, newCount * sizeof(char*));
    }
    m_data.count = newCount;

    for (size_t i = 0; i < newCount; ++i) {
        m_data.strings[i] = m_strings[i].data();
    }
}

}
}
