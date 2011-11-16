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

using namespace LibQGit2;

QGitTag::QGitTag(QGitRepository* repository)
{
    git_tag_new(&m_tag, repository->data());
}

QGitTag::QGitTag( const QGitTag& other )
{
    m_tag = other.m_tag;
}

QGitTag::~QGitTag()
{
}

int QGitTag::lookup(QGitRepository *repository, const QGitOId& oid)
{
    return git_tag_lookup(&m_tag, repository->data(), oid.data());
}

QGitOId QGitTag::id() const
{
    return QGitOId(git_tag_id(m_tag));
}

const QGitObject* QGitTag::target() const
{
    const QGitObject *object = new QGitObject(git_tag_target(m_tag));
    return object;
}

git_otype QGitTag::type() const
{
    return git_tag_type(m_tag);
}

const QString QGitTag::name() const
{
    return git_tag_name(m_tag);
}

const QGitSignature* QGitTag::tagger() const
{
    const QGitSignature *signature = new QGitSignature(git_tag_tagger(m_tag));
    return signature;
}

const QString QGitTag::message()
{
    return git_tag_message(m_tag);
}

void QGitTag::setTarget(const QGitObject &target)
{
    return git_tag_set_target(m_tag, target.data());
}

void QGitTag::setName(const QString& name)
{
    return git_tag_set_name(m_tag, name.toAscii().constData());
}

void QGitTag::setTagger(const QGitSignature& taggerSig)
{
    return git_tag_set_tagger(m_tag, taggerSig.data());
}

void QGitTag::setMessage(const QString& message)
{
    return git_tag_set_message(m_tag, message.toAscii().constData());
}

git_tag* QGitTag::data() const
{
    return m_tag;
}

const git_tag* QGitTag::constData() const
{
    return const_cast<const git_tag *>(m_tag);
}

