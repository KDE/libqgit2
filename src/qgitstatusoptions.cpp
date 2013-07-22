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


#include <QtCore/QFile>

#include "qgitstatusoptions.h"

namespace LibQGit2
{

QGitStatusOptions::QGitStatusOptions()
    : d(GIT_STATUS_OPTIONS_INIT)
{
}

QGitStatusOptions::QGitStatusOptions(git_status_options status_options)
    : d(status_options)
{
}

QGitStatusOptions::QGitStatusOptions(const QGitStatusOptions &other)
    : d(other.d)
{
}

QGitStatusOptions::~QGitStatusOptions()
{
}

QGitStatusOptions::ShowFlags QGitStatusOptions::showFlags() const
{
    return show_flags;
}

void QGitStatusOptions::setShowFlags(QGitStatusOptions::ShowFlags sf)
{
    show_flags = sf;
    int i = show_flags;
    d.show = (git_status_show_t)i;

}

QGitStatusOptions::StatusFlags QGitStatusOptions::statusFlags() const
{
    return status_flags;
}

void QGitStatusOptions::setStatusFlags(QGitStatusOptions::StatusFlags sf)
{
    status_flags = sf;
    d.flags = status_flags;
}

git_status_options QGitStatusOptions::data() const
{
    return d;
}

const git_status_options QGitStatusOptions::constData() const
{
    return d;
}

}
