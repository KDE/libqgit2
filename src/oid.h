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
    class LIBQGIT2_OID_EXPORT OId
    {
        public:
            OId( const git_oid *oid = 0, QObject* parent = 0 );

            /**
             * * Copy an oid from one class to another.
             * * @param other oid class to copy from.
             * */
            OId( const OId& other );
            ~OId();

        public:

            /**
             * Parse a hex formatted object id into a git_oid.
             * @param str input hex string; must be pointing at the start of
             * the hex sequence and have at least the number of bytes
             * needed for an oid encoded in hex (40 bytes).
             * @return GIT_SUCCESS if valid; GIT_ENOTOID on failure.
             */
            int makeString(const QByteArray& string);

            /**
             * Copy an already raw oid into a git_oid structure.
             * @param raw the raw input bytes to be copied.
             */
            void makeRaw(const unsigned char *raw);

            /**
             * Format a git_oid into a hex string.
             * @param str output hex string; must be pointing at the start of
             * the hex sequence and have at least the number of bytes
             * needed for an oid encoded in hex (40 bytes). Only the
             * oid digits are written; a '\\0' terminator must be added
             * by the caller if it is required.
             */
            void format(char* string);

            /**
             * Format a git_oid into a loose-object path string.
             * <p>
             * The resulting string is "aa/...", where "aa" is the first two
             * hex digitis of the oid and "..." is the remaining 38 digits.
             *
             * @param str output hex string; must be pointing at the start of
             * the hex sequence and have at least the number of bytes
             * needed for an oid encoded in hex (41 bytes). Only the
             * oid digits are written; a '\\0' terminator must be added
             * by the caller if it is required.
             */
            void pathFormat(char* string);

            /**
             * Format a gid_oid into a new string.
             * @param oid the oid structure to format
             * @return the c-string; NULL if memory is exhausted. Caller must
             * deallocate the string with free().
             */
            char* allocFormat();

            /**
             * Format an OId into a "buffer" as a hex format string.
             * <p>
             * If the buffer is smaller than GIT_OID_HEXSZ+1, then the resulting
             * oid c-string will be truncated to n-1 characters. If there are
             * any input parameter errors (out == NULL, n == 0, oid == NULL),
             * then a pointer to an empty string is returned, so that the return
             * value can always be printed.
             *
             * @param out the buffer into which the oid string is output.
             * @param n the size of the out buffer.
             * @return the out buffer, assuming no input parameter
             * errors, otherwise to an empty string.
             */
            char* toString(char* out, size_t n);

            /**
             * Compare two oid class.
             * @param oid oid class compare with.
             * @return <0, 0, >0 if a < b, a == b, a > b.
             */
            int compare(OId *oid);

            git_oid* data() const;
            const git_oid* constData() const;

        private:
            git_oid *m_oid;
    };
}

#endif // LIBQGIT2_OID_H
