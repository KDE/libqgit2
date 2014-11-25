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

class Signature::Private {
    static void do_not_free(git_signature*) {}
public:
    Private(const git_signature *signature, bool own) :
        signature(const_cast<git_signature*>(signature), own ? git_signature_free : do_not_free)
    {
    }

    QSharedPointer<git_signature> signature;
};

Signature::Signature(const QString& name, const QString& email, QDateTime dateTime)
{
    git_signature *sig = 0;
    qGitThrow(git_signature_new(&sig, name.toUtf8(), email.toUtf8(), dateTime.toTime_t(), dateTime.utcOffset() / 60));
    d_ptr = QSharedPointer<Private>(new Private(sig, true));
}

Signature::Signature(const QString& name, const QString& email)
{
    git_signature *sig = 0;
    qGitThrow(git_signature_now(&sig, name.toUtf8(), email.toUtf8()));
    d_ptr = QSharedPointer<Private>(new Private(sig, true));
}

Signature::Signature(const git_signature *signature) :
    d_ptr(new Private(signature, false))
{
}

QString Signature::name() const
{
    QString ret;
    if (d_ptr->signature) {
        ret = QString::fromUtf8(d_ptr->signature->name);
    }
    return ret;
}

QString Signature::email() const
{
    QString ret;
    if (d_ptr->signature) {
        ret = QString::fromUtf8(d_ptr->signature->email);
    }
    return ret;
}

QDateTime Signature::when() const
{
    QDateTime dt;
    if (d_ptr->signature) {
        dt.setTime_t(d_ptr->signature->when.time);
        dt.setUtcOffset(d_ptr->signature->when.offset * 60);
    }
    return dt;
}

const git_signature *Signature::data() const
{
    return d_ptr->signature.data();
}

} // namespace LibQGit2
