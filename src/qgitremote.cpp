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

#include "git2.h"

namespace LibQGit2 {

struct Remote::Private : public internal::RemoteListener
{
    Private(Remote &parent, git_remote *remote, const Credentials &credentials) :
        m_data(remote, git_remote_free),
        m_parent(parent),
        m_callbacks(remote, this, credentials),
        m_transfer_progress(0)
    {
    }

    int progress(const git_transfer_progress &stats)
    {
        int percent = (int)(0.5 + 100.0 * ((double)stats.received_objects) / ((double)stats.total_objects));
        if (percent != m_transfer_progress) {
            emit m_parent.transferProgress(percent);
            m_transfer_progress = percent;
        }
        return 0;
    }

    QSharedPointer<git_remote> m_data;

private:
    Remote &m_parent;
    internal::RemoteCallbacks m_callbacks;
    int m_transfer_progress;
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

git_remote* Remote::data() const
{
    return d_ptr->m_data.data();
}

}
