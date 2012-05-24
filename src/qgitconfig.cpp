/******************************************************************************
 * This file is part of the Gluon Development Platform
 * Copyright (c) 2012 Laszlo Papp <djszapi@archlinux.us>
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

#include "qgitconfig.h"

#include <QtCore/QDir>
#include <QtCore/QFile>

#include <git2/errors.h>
#include <git2/config.h>

#include <src/qgitexception.h>
#include <src/qgitrepository.h>

namespace LibQGit2
{

QGitConfig::QGitConfig(git_config *cfg)
    : d(cfg)
{
    if (d == 0)
        git_config_new(&d);
}

QGitConfig::QGitConfig(const QGitConfig &other)
    : d(other.d)
{
}

QGitConfig::~QGitConfig()
{
    git_config_free(d);
}

QGitConfig QGitConfig::fromGlobalConfig()
{
    git_config *    cfg;
    if ( git_config_open_global(&cfg) == GIT_OK )
        return QGitConfig(cfg);

    return QGitConfig();
}

QString QGitConfig::findGlobal()
{
    char * buffer = 0;
    qGitThrow( git_config_find_global(buffer, GIT_PATH_MAX) );

    return QFile::decodeName(buffer);
}

QString QGitConfig::findSystem()
{
    char * buffer = 0;
    qGitThrow( git_config_find_system(buffer, GIT_PATH_MAX) );

    return QFile::decodeName(buffer);
}

bool QGitConfig::append(const QString &path, int priority)
{
    return GIT_OK == git_config_add_file_ondisk(d, QFile::encodeName(path).constData(), priority);
}

QVariant QGitConfig::value(const QString &key, const QVariant &defaultValue) const
{
    const char * result = 0;
    if (git_config_get_string(&result, d, key.toUtf8().constData()) == GIT_OK)
        return QString::fromUtf8(result);

    return defaultValue;
}

void QGitConfig::setValue(const QString &key, const QVariant &value)
{
    qGitThrow( git_config_set_string(d, key.toUtf8(), value.toString().toUtf8().constData()) );
}


} // LibQGit2

