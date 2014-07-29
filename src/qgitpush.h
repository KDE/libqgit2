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

#ifndef LIBQGIT2_PUSH_H
#define LIBQGIT2_PUSH_H

#include <QtCore/QPointer>
#include <QtCore/QSharedPointer>

#include "qgitsignature.h"
#include "libqgit2_config.h"

struct git_push;

namespace LibQGit2
{

class Remote;

/**
 * @brief A class that is used for pushing to a remote.
 *
 * @ingroup LibQGit2
 * @{
 */
class LIBQGIT2_EXPORT Push
{
public:
    /**
     * Creates a \c Push object for \a remote.
     *
     * @note If \a remote has no parent set this object takes ownership of it.
     */
    Push(Remote &remote);

    /**
     * Adds a new refspec to this \c Push. Refspecs specify what local repository
     * reference is pushed to what remote reference. For the syntax of refspecs
     * see git documentation.
     * @throws LibQGit2::Exception
     */
    void addRefSpec(const QString &refSpec);

    /**
     * Executes the push.
     * @param signature The identity to use when updating reflogs
     * @param message The message to insert into the reflogs. If left as the
     *        default (a null string), a message "update by push" is used.
     * @throws LibQGit2::Exception
     */
    void execute(const Signature &signature = Signature(), const QString &message = QString());

private:
    QSharedPointer<Remote> m_remote;
    QSharedPointer<git_push> m_data;
};

/** @} */

}

#endif // LIBQGIT2_PUSH_H
