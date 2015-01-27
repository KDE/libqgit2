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
#include "credentials_p.h"
#include "qgitexception.h"

#include "git2.h"

namespace LibQGit2 {

struct Remote::Private
{
    Private(Remote &parent, git_remote *remote, const Credentials &credentials) :
        m_data(remote, git_remote_free),
        m_parent(parent),
        m_credentials(credentials),
        m_transfer_progress(0)
    {
        git_remote_callbacks remoteCallbacks = GIT_REMOTE_CALLBACKS_INIT;
        remoteCallbacks.payload = (void*)this;
        remoteCallbacks.transfer_progress = &transferProgressCallback;
        if (!m_credentials.isEmpty()) {
            remoteCallbacks.credentials = &acquireCredentialsCallback;
        }
        qGitThrow(git_remote_set_callbacks(m_data.data(), &remoteCallbacks));
    }

    QSharedPointer<git_remote> m_data;

private:
    static int transferProgressCallback(const git_transfer_progress* stats, void* data)
    {
        if (!data) {
            return 1;
        }

        Private &payload = *static_cast<Private*>(data);
        int percent = (int)(0.5 + 100.0 * ((double)stats->received_objects) / ((double)stats->total_objects));
        if (percent != payload.m_transfer_progress) {
            emit payload.m_parent.transferProgress(percent);
            payload.m_transfer_progress = percent;
        }
        return 0;
    }

    static int acquireCredentialsCallback(git_cred **cred, const char *url, const char *username_from_url, unsigned int allowed_types, void *data)
    {
        int result = -1;
        if (data) {
            Credentials &credentials = static_cast<Private*>(data)->m_credentials;
            result = CredentialsPrivate::create(credentials, cred, url, username_from_url, allowed_types);
        }

        return result;
    }

    Remote &m_parent;
    Credentials m_credentials;
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
