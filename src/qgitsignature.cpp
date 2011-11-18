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

QGitSignature::QGitSignature(const QString& name, const QString& email, QDateTime dateTime)
{
    d = git_signature_new(qPrintable(name), qPrintable(email), dateTime.toTime_t(), dateTime.utcOffset() / 60);
}

QGitSignature::QGitSignature(git_signature *signature)
    : d(signature)
{
}

QGitSignature::QGitSignature(const QGitSignature& other)
{
    d = git_signature_dup(other.d);
}

QGitSignature::~QGitSignature()
{
    git_signature_free(d);
}

QString QGitSignature::name() const
{
    return QString(d->name);
}

QString QGitSignature::email() const
{
    return QString(d->email);
}

QDateTime QGitSignature::when() const
{
    QDateTime dt;
    dt.setTime_t(d->when.time);
    dt.setUtcOffset(d->when.offset * 60);
    return dt;
}

git_signature* QGitSignature::data() const
{
    return d;
}

const git_signature* QGitSignature::constData() const
{
    return d;
}


QGitSignatureRef::QGitSignatureRef(const git_signature *signature)
    : d(signature)
{
}

QGitSignatureRef::QGitSignatureRef(const QGitSignatureRef& other)
    : d(other.data())
{
}

QGitSignatureRef::QGitSignatureRef(const QGitSignature& other)
    : d(other.data())
{
}

QGitSignatureRef::~QGitSignatureRef()
{
}

QString QGitSignatureRef::name() const
{
    return QString(d->name);
}

QString QGitSignatureRef::email() const
{
    return QString(d->email);
}

QDateTime QGitSignatureRef::when() const
{
    QDateTime dt;
    dt.setTime_t(d->when.time);
    dt.setUtcOffset(d->when.offset * 60);
    return dt;
}

const git_signature *QGitSignatureRef::data() const
{
    return d;
}
