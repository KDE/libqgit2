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

#include "tag.h"

using namespace LibQGit2;

Tag::Tag(Repository* repository, QObject* parent)
{
    git_tag_new(&m_tag, repository->data());
}

Tag::Tag( const Tag& other )
{
    m_tag = other.m_tag;
}

Tag::~Tag()
{
}

int Tag::lookup(Repository *repository, const OId& oid)
{
    return git_tag_lookup(&m_tag, repository->data(), oid.constData());
}

const OId* Tag::id() const
{
    const OId *oid = new OId(git_tag_id(m_tag));
    return oid;
}

const Object* Tag::target() const
{
    const Object *object = new Object(git_tag_target(m_tag));
    return object;
}

git_otype Tag::type() const
{
    return git_tag_type(m_tag);
}

const QString Tag::name() const
{
    return git_tag_name(m_tag);
}

const Signature* Tag::tagger() const
{
    const Signature *signature = new Signature(git_tag_tagger(m_tag));
    return signature;
}

const QString Tag::message()
{
    return git_tag_message(m_tag);
}

void Tag::setTarget(const Object &target)
{
    return git_tag_set_target(m_tag, target.data());
}

void Tag::setName(const QString& name)
{
    return git_tag_set_name(m_tag, name.toAscii().constData());
}

void Tag::setTagger(const Signature& taggerSig)
{
    return git_tag_set_tagger(m_tag, taggerSig.data());
}

void Tag::setMessage(const QString& message)
{
    return git_tag_set_message(m_tag, message.toAscii().constData());
}

git_tag* Tag::data() const
{
    return m_tag;
}

const git_tag* Tag::constData() const
{
    return const_cast<const git_tag *>(m_tag);
}

