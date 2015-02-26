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

#include "qgitremote.h"
#include "qgitexception.h"
#include "private/remotecallbacks.h"
#include "private/strarray.h"

#include "git2.h"

namespace LibQGit2 {

struct Remote::Private : public internal::RemoteListener
{
    Private(Remote &parent, git_remote *remote, const Credentials &credentials) :
        m_data(remote, git_remote_free),
        m_parent(parent),
        m_callbacks(remote, this, credentials)
    {
    }

    int progress(int transferProgress)
    {
        emit m_parent.transferProgress(transferProgress);
        return 0;
    }

    QSharedPointer<git_remote> m_data;

private:
    Remote &m_parent;
    internal::RemoteCallbacks m_callbacks;
};


Remote::Remote(git_remote *remote, const Credentials &credentials, QObject *parent) :
    QObject(parent),
    d_ptr(new Private(*this, remote, credentials))
{
}

QString Remote::url() const
{
    return QString::fromLatin1(git_remote_url(data()));
}

void Remote::push(const QList<QString> &refSpecs, const Signature &signature, const QString &message)
{
    QList<QByteArray> baRefSpecs;
    foreach (const QString &ref, refSpecs) {
        baRefSpecs.append(ref.toLatin1());
    }
    internal::StrArray refspecs(baRefSpecs);

    git_push_options opts = GIT_PUSH_OPTIONS_INIT;
    qGitThrow(git_remote_push(data(), &refspecs.data(), &opts, signature.data(), message.isNull() ? NULL : message.toUtf8().constData()));
}

git_remote* Remote::data() const
{
    return d_ptr->m_data.data();
}

}
