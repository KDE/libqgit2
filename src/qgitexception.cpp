/******************************************************************************
 * This file is part of the libqgit2 library
 * Copyright (c) 2011 Laszlo Papp <djszapi@archlinux.us>
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

#include "git2.h"

#include "qgitexception.h"

namespace LibQGit2
{

Exception::Exception()
{
    const git_error *err = giterr_last();

    if (err != NULL) {
        m = err->message;
        giterr_clear();
    }
}

Exception::~Exception() throw()
{
}

const char *Exception::what() const throw()
{
    return m;
}

QByteArray Exception::message() const throw()
{
    return m;
}

int qGitThrow(int ret)
{
    if (ret < 0) {
        throw Exception();
    }
    return ret;
}

}
