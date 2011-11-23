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
#include "qgitexception.h"

#include <git2/signature.h>

using namespace LibQGit2;

QGitSignatureBuilder::QGitSignatureBuilder(const QString& name, const QString& email, QDateTime dateTime)
{
    qGitThrow(git_signature_new(&d, qPrintable(name), qPrintable(email), dateTime.toTime_t(), dateTime.utcOffset() / 60));
}

QGitSignatureBuilder::QGitSignatureBuilder(const QString& name, const QString& email)
{
    qGitThrow(git_signature_now(&d, qPrintable(name), qPrintable(email)));
}

QGitSignatureBuilder::QGitSignatureBuilder(const QGitSignatureBuilder& other)
{
    d = git_signature_dup(other.d);
}

QGitSignatureBuilder::~QGitSignatureBuilder()
{
    git_signature_free(d);
}

QString QGitSignatureBuilder::name() const
{
    return QString(d->name);
}

QString QGitSignatureBuilder::email() const
{
    return QString(d->email);
}

QDateTime QGitSignatureBuilder::when() const
{
    QDateTime dt;
    dt.setTime_t(d->when.time);
    dt.setUtcOffset(d->when.offset * 60);
    return dt;
}

const git_signature* QGitSignatureBuilder::data() const
{
    return d;
}

QGitSignature::QGitSignature(const git_signature *signature)
    : d(signature)
{
}

QGitSignature::QGitSignature(const QGitSignature& other)
    : d(other.data())
{
}

QGitSignature::QGitSignature(const QGitSignatureBuilder& other)
    : d(other.data())
{
}

QGitSignature::~QGitSignature()
{
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

const git_signature *QGitSignature::data() const
{
    return d;
}
