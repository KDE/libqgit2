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

#ifndef LIBQGIT2_EXCEPTION_H
#define LIBQGIT2_EXCEPTION_H

#include <exception>
#include "libqgit2_config.h"
#include <QtCore/QByteArray>

namespace LibQGit2
{
    /**
     * @brief Exception class to throw Git exceptions.
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_EXPORT Exception : public std::exception
    {
        public:
            /**
             * A category for an exception.
             *
             * @note These map directly to libgit2 error codes.
             * @see https://libgit2.github.com/libgit2/#v0.21.0/type/git_error_t
             */
            enum Category {
                None,
                NoMemory,
                OS,
                Invalid,
                Reference,
                ZLib,
                Repository,
                Config,
                RegEx,
                ODB,
                Index,
                Object,
                Net,
                Tag,
                Tree,
                Indexer,
                SSL,
                Submodule,
                Thread,
                Stash,
                Checkout,
                FetchHead,
                Merge,
                SSH,
                Filter,
                Revert,
                Callback,
                Cherrypick
            };

            Exception();
            Exception(const QString& msg, Category category = None);
            ~Exception() throw();

            const char *what() const throw();

            QByteArray message() const throw();

            Category category() const throw();

        private:
            QByteArray m_msg;
            Category m_category;
    };

    inline int qGitThrow(int ret) { if (ret < 0) throw Exception(); return ret; }

    inline void qGitEnsureValue(int value, int ret) { if (qGitThrow(ret) != value) throw Exception(); }

    /**@}*/
}

#endif // LIBQGIT2_EXCEPTION_H
