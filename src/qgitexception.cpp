/******************************************************************************
 * This file is part of the Gluon Development Platform
 * Copyright (c) 2011 Laszlo Papp <djszapi@archlinux.us>
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

#include "qgitexception.h"

#include <git2/errors.h>

namespace LibQGit2
{

QGitException::QGitException(int error)
{
    if (git_lasterror())
        m = git_lasterror();
    else
        m = git_strerror(error);
}

QGitException::~QGitException() throw()
{
}

const char *QGitException::what() const throw()
{
    return m;
}

int qGitThrow(int ret)
{
    if (ret < 0) {
        throw QGitException(ret);
    }
    return ret;
}

}
