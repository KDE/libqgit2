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

#include "qgitcredentials.h"
#include "credentials_p.h"

#include "private/pathcodec.h"

#include "git2.h"

namespace LibQGit2
{

CredentialsPrivate::CredentialsPrivate(unsigned int allowedTypes) :
    m_allowed_types(allowedTypes)
{
}

CredentialsPrivate::~CredentialsPrivate() {}

int CredentialsPrivate::create(git_cred**, const char*, const char*, unsigned int)
{
    return -1;
}

int CredentialsPrivate::create(Credentials &credentials, git_cred **cred, const char *url, const char *usernameFromUrl, unsigned int allowedTypes)
{
    CredentialsPrivate *d = credentials.d_func();

    int result = -1;
    if ((allowedTypes & d->m_allowed_types)) {
        result = d->create(cred, url, usernameFromUrl, allowedTypes);
    }

    return result;
}


struct SSHCredentialsPrivate : public CredentialsPrivate {
    SSHCredentialsPrivate(const QString &privateKeyPath, const QString &publicKeyPath, const QByteArray &userName, const QByteArray &passphrase) :
        CredentialsPrivate(GIT_CREDTYPE_SSH_KEY | GIT_CREDTYPE_USERNAME),
        m_private_key_path(PathCodec::toLibGit2(privateKeyPath)),
        m_public_key_path(PathCodec::toLibGit2(publicKeyPath)),
        m_user_name(userName),
        m_passphrase(passphrase)
    {
    }

protected:
    int create(git_cred **cred, const char*, const char*, unsigned int allowedTypes)
    {
        if (allowedTypes & GIT_CREDTYPE_USERNAME) {
            return git_cred_username_new(cred, m_user_name.data());
        }

        if (allowedTypes & GIT_CREDTYPE_SSH_KEY) {
            return git_cred_ssh_key_new(cred, m_user_name.data(), m_public_key_path.data(), m_private_key_path.data(), m_passphrase.data());
        }

        return -1;
    }

private:
    const QByteArray m_private_key_path;
    const QByteArray m_public_key_path;
    const QByteArray m_user_name;
    const QByteArray m_passphrase;
};


Credentials::Credentials() :
    d_ptr(new CredentialsPrivate(0))
{
}

Credentials::Credentials(CredentialsPrivate &p) :
    d_ptr(&p)
{
}

bool Credentials::isEmpty() const
{
    return d_ptr.isNull();
}

Credentials Credentials::ssh(const QString &privateKeyPath, const QString &publicKeyPath, const QByteArray &userName, const QByteArray &passphrase)
{
    return Credentials(*new SSHCredentialsPrivate(privateKeyPath, publicKeyPath, userName, passphrase));
}

}
