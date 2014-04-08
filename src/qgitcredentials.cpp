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

#include <QtCore/QFile>

#include "git2.h"

namespace LibQGit2
{

struct Credentials::CredCreator {
    virtual ~CredCreator() {}

    virtual int create(git_cred **cred, const char *url, const char *username_from_url) = 0;
};

struct SSHKeyCreator : public Credentials::CredCreator {
    SSHKeyCreator(const QString &privateKeyPath, const QString &publicKeyPath, const QByteArray &userName, const QByteArray &passphrase) :
        m_private_key_path(QFile::encodeName(privateKeyPath)),
        m_public_key_path(QFile::encodeName(publicKeyPath)),
        m_user_name(userName),
        m_passphrase(passphrase)
    {
    }

    int create(git_cred **cred, const char *url, const char *username_from_url) {
        return git_cred_ssh_key_new(cred, m_user_name.data(), m_public_key_path.data(), m_private_key_path.data(), m_passphrase.data());
    }

private:
    const QByteArray m_private_key_path;
    const QByteArray m_public_key_path;
    const QByteArray m_user_name;
    const QByteArray m_passphrase;
};



Credentials::Credentials() :
    m_allowed_types(0)
{
}


bool Credentials::isEmpty() const
{
    return m_cred_creator.isNull();
}


Credentials::Credentials(unsigned int allowed_types, CredCreator *creator) :
    m_allowed_types(allowed_types),
    m_cred_creator(creator)
{
}


Credentials Credentials::ssh(const QString &privateKeyPath, const QString &publicKeyPath, const QByteArray &userName, const QByteArray &passphrase)
{
    return Credentials(GIT_CREDTYPE_SSH_KEY, new SSHKeyCreator(privateKeyPath, publicKeyPath, userName, passphrase));
}


int Credentials::create(git_cred **cred, const char *url, const char *username_from_url, unsigned int allowed_types) const
{
    int result = -1;
    if ((allowed_types & m_allowed_types) && m_cred_creator) {
        result = m_cred_creator->create(cred, url, username_from_url);
    }

    return result;
}

}
