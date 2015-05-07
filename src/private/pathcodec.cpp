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

#include "pathcodec.h"
#include <QFile>

// POSIX emulation in libgit2 assumes all i/o to be UTF-8 encoded
#ifdef _WIN32
QByteArray PathCodec::toLibGit2(const QString &path)
{
    return path.toUtf8();
}

QString PathCodec::fromLibGit2(const QByteArray &path)
{
    return QString::fromUtf8(path);
}
// in every other system the POSIX functions probably expect
// encoding to be the same as system locale, so let's not
// fix that which isn't broken
#else
QByteArray PathCodec::toLibGit2(const QString &path)
{
    return QFile::encodeName(path);
}

QString PathCodec::fromLibGit2(const QByteArray &path)
{
    return QFile::decodeName(path);
}
#endif

QString PathCodec::fromLibGit2(const char *path)
{
    return fromLibGit2(QByteArray(path));
}
