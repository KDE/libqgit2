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

#ifndef LIBQGIT2_CREDENTIALS_P_H
#define LIBQGIT2_CREDENTIALS_P_H

struct git_cred;

namespace LibQGit2
{

class Credentials;

class CredentialsPrivate {
public:
    CredentialsPrivate(unsigned int allowedTypes);
    virtual ~CredentialsPrivate();

    static int create(Credentials &credentials, git_cred **cred, const char *url, const char *usernameFromUrl, unsigned int allowedTypes);

protected:
    virtual int create(git_cred **cred, const char *url, const char *usernameFromUrl, unsigned int allowedTypes);

private:
    unsigned int m_allowed_types;
};

}

#endif // LIBQGIT2_CREDENTIALS_P_H
