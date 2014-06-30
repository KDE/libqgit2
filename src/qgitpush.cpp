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

#include "qgitpush.h"
#include "qgitremote.h"
#include "qgitexception.h"

#include "git2.h"

namespace LibQGit2
{

Push::Push(Remote &remote) :
    m_remote(remote.parent() == 0 ? &remote : 0)
{
    git_push *push = NULL;
    qGitThrow(git_push_new(&push, m_remote->data()));
    m_data = QSharedPointer<git_push>(push, git_push_free);
}


void Push::addRefSpec(const QString &refSpec)
{
    qGitThrow(git_push_add_refspec(m_data.data(), refSpec.toLatin1()));
}

void Push::execute(const Signature &signature, const QString &message)
{
    qGitThrow(git_push_finish(m_data.data()));

    if (!git_push_unpack_ok(m_data.data())) {
        throw Exception("Push::execute(): remote failed to unpack while pushing");
    }

    qGitThrow(git_push_update_tips(m_data.data(), signature.data(), message.isNull() ? NULL : message.toUtf8().constData()));
}

}
