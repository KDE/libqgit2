/******************************************************************************
 * This file is part of the Gluon Development Platform
 * Copyright (c) 2011 Laszlo Papp <djszapi@archlinux.us>
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

#ifndef LIBQGIT2_OID_H
#define LIBQGIT2_OID_H

#include "libqgit2_export.h"

#include <QtCore/QString>
#include <QtCore/QDateTime>

#include <git2/oid.h>

namespace LibQGit2
{
    class LIBQGIT2_OID_EXPORT QGitOId
    {
        public:

            /**
             * Constructor
             */
            explicit QGitOId(const git_oid *oid = 0);

            /**
             * Copy constructor
             */
            QGitOId(const QGitOId& other);

            /**
             * Destructor
             */
            ~QGitOId();

            /**
             * Parse a hex formatted object id into a OId.
             *
             * @param string
             * Input hex string; if less than 40 bytes, prefix lookup will be performed. Must be
             * at least 4 bytes.
             *
             * @return OId; null OId on failure.
             */
            static QGitOId fromString(const QByteArray& string);

            /**
             * Copy an already raw oid into a git_oid structure.
             * @param raw the raw input bytes to be copied.
             */
            static QGitOId fromRawData(const QByteArray& raw);

            /**
              Checks if this is a valid Git OId. An OId is invalid if it is empty or 0x0000... (20 byte).
              @return True, if the OId is valid. False if not.
              */
            bool isValid() const;

            /**
             * Format a OId into a hex string.
             */
            QByteArray format() const;

            /**
             * Format a git_oid into a loose-object path string.
             *
             * The resulting string is "aa/...", where "aa" is the first two
             * hex digitis of the oid and "..." is the remaining 38 digits.
             */
            QByteArray pathFormat() const;

            git_oid* data();
            const git_oid* data() const;
            const git_oid* constData() const;

            int length() const;

        private:
            QByteArray d;
    };

    /**
     * Compare two oid objects
     */
    bool operator==(const QGitOId& oid1, const QGitOId& oid2);
}

#endif // LIBQGIT2_OID_H
