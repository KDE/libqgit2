/******************************************************************************
 * This file is part of the libqgit2 library
 * Copyright (c) 2011 Laszlo Papp <djszapi@archlinux.us>
 * Copyright (C) 2013 Leonardo Giordani
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

#include <QtCore/QString>
#include <QtCore/QDateTime>

#include "git2.h"

#include "libqgit2_config.h"

namespace LibQGit2
{
    /**
     * @brief Wrapper class for git_oid.
     *
     * This class holds a Git SHA1 object id, i.e. 40 hexadecimal digits.
     * Instead of using the git_oid structure, which is an unsigned char array,
     * the object redefines it as a QByteArray and provides the conversion
     * to git_oid through data() and constData().
     *
     * Remember that the QByteArray stores 2 hexadecimal digits for each
     * element, so the length of the array is half that of the SHA1, namely 20 bytes.
     * This can either encompass a full oid (40 hexadecimal digits) or a part of it,
     * (short reference).
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_EXPORT OId
    {
        public:

            /**
             * Constructor
             */
            explicit OId(const git_oid *oid = 0);

            /**
             * Copy constructor
             */
            OId(const OId& other);

            /**
             * Destructor
             */
            ~OId();

            /**
             * Set the value of the object parsing a hex array.
             *
             * This method parses an array of hexadecimal values trying to
             * convert it into an OId.
             * If the array contains more than 40 values, only the first 40
             * will be converted.
             * If the array contains less than 40 values the resulting OId
             * will be a shortened OId, or a prefix.
             *
             * @param hex
             * Input hex string; must be at least 4 bytes.
             *
             * @throws Exception
             */
            void fromHex(const QByteArray& hex);

            /**
             * Set the value of the object parsing a string.
             *
             * This method behaves just like fromHex() but parses a string
             * that contains hexadecimal values. The same rules of fromHex()
             * apply here.
             *
             * @param string
             * Input string; must be at least 4 characters long.
             *
             * @throws Exception
             */
            void fromString(const QString& string);

            /**
             * Set the value of the object from a raw oid.
             *
             * This method uses the input raw hexadecimal array without parsing
             * it and without performing prefix lookup. The raw array must be
             * 40 characters long, otherwise throws Exception.
             *
             * @param raw the raw input bytes to be copied.
             * @throws Exception
             */
            void fromRawData(const QByteArray& raw);

            /**
             * Parse a hex formatted object id into a OId.
             *
             * @param string
             * Input hex string; if less than 40 bytes, prefix lookup will be performed. Must be
             * at least 4 bytes.
             *
             * @return OId; null OId on failure.
             * @throws Exception
             */
            static OId stringToOid(const QByteArray& string);

            /**
             * Copy an already raw oid into a git_oid structure.
             * @param raw the raw input bytes to be copied.
             */
            static OId rawDataToOid(const QByteArray& raw);

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
            const git_oid* constData() const;

            /**
             * Returns the length of the OId as a number of hexadecimal
             * characters.
             *
             * The full length of a OId is 40, but the OId represented by this
             * class may be shorter.
             */
            int length() const;

        private:
            QByteArray d;
    };

    /**
     * Compare two OIds.
     */
    LIBQGIT2_EXPORT bool operator ==(const OId &oid1, const OId &oid2);
    /**
     * Compare two OIds.
     */
    LIBQGIT2_EXPORT bool operator !=(const OId &oid1, const OId &oid2);

    /**@}*/
}

#endif // LIBQGIT2_OID_H
