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

#include "qgitsignature.h"
#include "qgitexception.h"

namespace LibQGit2
{

SignatureBuilder::SignatureBuilder(const QString& name, const QString& email, QDateTime dateTime)
{
    qGitThrow(git_signature_new(&d, qPrintable(name), qPrintable(email), dateTime.toTime_t(), dateTime.utcOffset() / 60));
}

SignatureBuilder::SignatureBuilder(const QString& name, const QString& email)
{
    qGitThrow(git_signature_now(&d, qPrintable(name), qPrintable(email)));
}

SignatureBuilder::SignatureBuilder(const SignatureBuilder& other)
{
    qGitThrow(git_signature_dup(&d, other.d));
}

SignatureBuilder::~SignatureBuilder()
{
    git_signature_free(d);
}

QString SignatureBuilder::name() const
{
    return QString::fromUtf8(d->name);
}

QString SignatureBuilder::email() const
{
    return QString::fromUtf8(d->email);
}

QDateTime SignatureBuilder::when() const
{
    QDateTime dt;
    dt.setTime_t(d->when.time);
    dt.setUtcOffset(d->when.offset * 60);
    return dt;
}

const git_signature* SignatureBuilder::data() const
{
    return d;
}

Signature::Signature(const git_signature *signature)
    : d(signature)
{
}

Signature::Signature(const Signature& other)
    : d(other.data())
{
}

Signature::Signature(const SignatureBuilder& other)
    : d(other.data())
{
}

Signature::~Signature()
{
}

QString Signature::name() const
{
    return QString::fromUtf8(d->name);
}

QString Signature::email() const
{
    return QString::fromUtf8(d->email);
}

QDateTime Signature::when() const
{
    QDateTime dt;
    dt.setTime_t(d->when.time);
    dt.setUtcOffset(d->when.offset * 60);
    return dt;
}

const git_signature *Signature::data() const
{
    return d;
}

} // namespace LibQGit2
