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

#ifndef LIBQGIT2_REMOTE_H
#define LIBQGIT2_REMOTE_H

#include "libqgit2_config.h"
#include "qgitsignature.h"
#include "qgitcredentials.h"

#include <QtCore/QSharedPointer>

struct git_remote;

namespace LibQGit2 {

/**
 * @brief Represents a git remote
 *
 * @ingroup LibQGit2
 * @{
 */
class LIBQGIT2_EXPORT Remote : public QObject
{
    Q_OBJECT

public:
    /**
     * @param remote The raw remote pointer. This needs to be initialized beforehand.
     *        This object takes ownership of this pointer and frees it when this object is destructed.
     * @param credentials Credentials to be used with this remote if any.
     * @param parent The parent of this QObject.
     */
    explicit Remote(git_remote *remote, const Credentials &credentials = Credentials(), QObject *parent = 0);

    /**
     * Gets the URL specified for this remote.
     */
    QString url() const;

    /**
     * Perform a push on this Remote.
     * @param refSpecs The refspecs to use for pushing. If left empty the configured refspecs will be used.
     * @param signature The identity to use when updating reflogs.
     * @param message The message to insert into the reflogs. If left as the
     *        default (a null string), a message "update by push" is used.
     * @throws LibQGit2::Exception
     */
    void push(const QList<QString> &refSpecs = QList<QString>(), const Signature &signature = Signature(), const QString &message = QString());

    git_remote* data() const;

signals:
    void transferProgress(int);

private:
    Q_DISABLE_COPY(Remote)

    struct Private;
    QSharedPointer<Private> d_ptr;
};

/** @} */

}

#endif // LIBQGIT2_REMOTE_H
