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

#ifndef LIBQGIT2_TAG_H
#define LIBQGIT2_TAG_H

#include "libqgit2_export.h"

#include "qgitsignature.h"
#include "qgitoid.h"
#include "qgitrepository.h"

#include <git2/tag.h>

#include <QtCore/QSharedPointer>

namespace LibQGit2
{
    class QGitObject;

    class LIBQGIT2_TAG_EXPORT QGitTag
    {
        public:

            explicit QGitTag(git_tag *tag = 0);

            QGitTag(const QGitTag& other);
            ~QGitTag();

            /**
             * Get the id of a tag.
             * @return object identity for the tag.
             */
            QGitOId oid() const;

            /**
             * Get the tagged object of a tag
             * @return reference to a repository object
             */
            QGitObject target() const;

            /**
             * Get the type of a tag's tagged object
             * @return type of the tagged object
             */
            git_otype type() const;

            /**
             * Get the name of a tag
             * @return name of the tag
             */
            const QString name() const;

            /**
             * Get the tagger (author) of a tag
             * @return reference to the tag's author
             */
            QGitSignatureRef tagger() const;

            /**
             * Get the message of a tag
             * @return message of the tag
             */
            const QString message();

            git_tag* data() const;
            const git_tag* constData() const;

        private:
            QSharedPointer<git_tag> d;
    };
}

#endif // LIBQGIT2_TAG_H
