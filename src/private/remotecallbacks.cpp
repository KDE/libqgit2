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

#include "remotecallbacks.h"
#include "qgitcredentials.h"
#include "credentials_p.h"
#include "qgitexception.h"

namespace LibQGit2
{
namespace internal
{


RemoteListener::~RemoteListener() {}


RemoteCallbacks::RemoteCallbacks(git_remote *remote, RemoteListener *listener, const Credentials &credentials) :
    m_listener(listener),
    m_credentials(credentials),
    m_transferProgress(-1)
{
    git_remote_callbacks remoteCallbacks = rawCallbacks();
    qGitThrow(git_remote_set_callbacks(remote, &remoteCallbacks));
}

RemoteCallbacks::RemoteCallbacks(RemoteListener *listener, const Credentials &credentials) :
    m_listener(listener),
    m_credentials(credentials),
    m_transferProgress(-1)
{
}

git_remote_callbacks RemoteCallbacks::rawCallbacks() const
{
    git_remote_callbacks remoteCallbacks = GIT_REMOTE_CALLBACKS_INIT;
    remoteCallbacks.payload = (void*)this;

    if (m_listener) {
        remoteCallbacks.transfer_progress = &transferProgressCallback;
    }

    if (!m_credentials.isEmpty()) {
        remoteCallbacks.credentials = &acquireCredentialsCallback;
    }

    return remoteCallbacks;
}

int RemoteCallbacks::transferProgressCallback(const git_transfer_progress* stats, void* data)
{
    int ret = 0;

    if (data && stats) {
        RemoteCallbacks* cb = static_cast<RemoteCallbacks*>(data);

        int percent = (int)(0.5 + 100.0 * ((double)stats->received_objects) / ((double)stats->total_objects));
        if (percent != cb->m_transferProgress) {
            cb->m_transferProgress = percent;
            ret = cb->m_listener->progress(percent);
        }
    }

    return ret;
}

int RemoteCallbacks::acquireCredentialsCallback(git_cred **cred, const char *url, const char *usernameFromUrl, unsigned int allowedTypes, void *data)
{
    int result = -1;
    if (data) {
        Credentials &credentials = static_cast<RemoteCallbacks*>(data)->m_credentials;
        result = CredentialsPrivate::create(credentials, cred, url, usernameFromUrl, allowedTypes);
    }

    return result;
}

}
}
