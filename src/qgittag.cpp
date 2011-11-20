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

#include "qgittag.h"
#include "qgitoid.h"
#include "qgitsignature.h"

#include <git2/tag.h>

using namespace LibQGit2;

QGitTag::QGitTag(git_tag *tag)
    : QGitObject(reinterpret_cast<git_object*>(tag))
{
}

QGitTag::QGitTag(const QGitTag& other)
    : QGitObject(other)
{
}

QGitTag::~QGitTag()
{
}

QGitOId QGitTag::oid() const
{
    return QGitOId(git_tag_id(data()));
}

QGitObject QGitTag::target() const
{
    git_object *obj;
    git_tag_target(&obj, data());
    return QGitObject(obj);
}

const QString QGitTag::name() const
{
    return git_tag_name(data());
}

QGitSignatureRef QGitTag::tagger() const
{
    return QGitSignatureRef(git_tag_tagger(data()));
}

const QString QGitTag::message()
{
    return git_tag_message(data());
}

git_tag* QGitTag::data() const
{
    return reinterpret_cast<git_tag*>(QGitObject::data());
}

const git_tag* QGitTag::constData() const
{
    return reinterpret_cast<git_tag*>(QGitObject::data());
}

