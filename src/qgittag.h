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

namespace LibQGit2
{
    class QGitObject;
    class LIBQGIT2_TAG_EXPORT QGitTag
    {
        public:
            /**
             * Construct a new in-memory Tag.
             *
             * The tag object must be manually filled using
             * setter methods before it can be written to its
             * repository.
             *
             * @param repository The repository where the object will reside
             * @return 0 on success; error code otherwise
             */
            QGitTag( QGitRepository *repository, QObject* parent = 0 );

            QGitTag( const QGitTag& other );
            ~QGitTag();

        public:

            /**
             * Lookup a tag object from the repository.
             * The generated tag object is owned by the revision
             * repo and shall not be freed by the user.
             *
             * @param repository the repository to use when locating the tag.
             * @param id identity of the tag to locate.
             * @return 0 on success; error code otherwise
             */
            int lookup(QGitRepository *repository, const QGitOId& oid);

            /**
             * Get the id of a tag.
             * @return object identity for the tag.
             */
            const QGitOId* id() const;

            /**
             * Get the tagged object of a tag
             * @return reference to a repository object
             */
            const QGitObject* target() const;

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
            const QGitSignature* tagger() const;

            /**
             * Get the message of a tag
             * @return message of the tag
             */
            const QString message();

            /**
             * Set the target of a tag (i.e. the object that the tag points to)
             * @param target the new tagged target
             */
            void setTarget(const QGitObject &target);

            /**
             * Set the name of a tag
             * @param name the new name for the tag
             */
            void setName(const QString& name);

            /**
             * Set the tagger of a tag
             * @param taggerSig signature of the tagging action
             */
            void setTagger(const QGitSignature& taggerSig);


            /**
             * Set the message of a tag
             * @param message the new tagger for the tag
             */
            void setMessage(const QString& message);

            git_tag* data() const;
            const git_tag* constData() const;

        private:
            git_tag *m_tag;
    };
}

#endif // LIBQGIT2_TAG_H
