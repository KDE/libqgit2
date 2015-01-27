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

#include "qgitstatusoptions.h"
#include <cstring>

namespace LibQGit2
{

StatusOptions::StatusOptions(ShowType aShowType, StatusFlags aStatusFlags)
{
    std::memset((void*)&d, 0, sizeof(git_status_options));
    d.version = GIT_STATUS_OPTIONS_VERSION;
    setShowType(aShowType);
    setStatusFlags(aStatusFlags);
}

StatusOptions::StatusOptions(git_status_options status_options)
    : d(status_options)
{
}

StatusOptions::StatusOptions(const StatusOptions &other)
    : d(other.d)
{
}

StatusOptions::~StatusOptions()
{
}

StatusOptions::ShowType StatusOptions::showType() const
{
    return show_type;
}

void StatusOptions::setShowType(ShowType type)
{
    show_type = type;
    int i = show_type;
    d.show = (git_status_show_t)i;
}

StatusOptions::StatusFlags StatusOptions::statusFlags() const
{
    return status_flags;
}

void StatusOptions::setStatusFlags(StatusOptions::StatusFlags sf)
{
    status_flags = sf;
    d.flags = status_flags;
}

git_status_options StatusOptions::data() const
{
    return d;
}

const git_status_options StatusOptions::constData() const
{
    return d;
}

}
