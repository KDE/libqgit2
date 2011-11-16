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

#include "qgitsignature.h"

using namespace LibQGit2;

QGitSignature::QGitSignature(const QString& name, const QString& email, QDateTime dateTime, int offset)
{
    m_signature = git_signature_new(name.toAscii().data(), email.toAscii().data(), dateTime.toTime_t(), offset);
}

QGitSignature::QGitSignature(const git_signature *signature)
    : m_signature(const_cast<git_signature *>(signature))
{
}

QGitSignature::QGitSignature( const QGitSignature& other )
{
    m_signature = other.m_signature;
}

QGitSignature::~QGitSignature()
{
    git_signature_free(m_signature);
}

git_signature* QGitSignature::data() const
{
    return m_signature;
}

const git_signature* QGitSignature::constData() const
{
    return const_cast<const git_signature *>(m_signature);
}


