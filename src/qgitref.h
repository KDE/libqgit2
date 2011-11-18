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

#ifndef LIBQGIT2_REF_H
#define LIBQGIT2_REF_H

#include "libqgit2_export.h"

#include <QtCore/QString>

#include <git2/refs.h>

namespace LibQGit2
{
    class QGitRepository;
    class QGitOId;
    class LIBQGIT2_REFS_EXPORT QGitRef
    {
        public:
            /**
             * Create a new reference.
             *
             * The reference will be empty and exclusively
             * in-memory until it is filled with the setter
             * methods and written back to disk using
             * `git_reference_write`.
             *
             * @param repository Repository where that reference exists
             * @return 0 on success; error code otherwise
             */
            explicit QGitRef(const QGitRepository& repository);

            explicit QGitRef(git_reference *ref);

            QGitRef( const QGitRef& other );

            /**
             * Free an existing reference object.
             */
            ~QGitRef();

        public:

            /**
             * Get the OID pointed to by a reference.
             *
             * Only available if the reference is direct (i.e. not symbolic)
             *
             * @return a pointer to the oid if available, NULL otherwise
             */
            QGitOId oid() const;

            /**
             * Get full name to the reference pointed by this reference
             *
             * Only available if the reference is symbolic
             *
             * @return a pointer to the name if available, NULL otherwise
             */
            QString target() const;

            /**
             * Get the type of a reference
             *
             * Either direct (GIT_REF_OID) or symbolic (GIT_REF_SYMBOLIC)
             *
             * @return the type
             */
            git_rtype type() const;

            /**
             * Get the full name of a reference
             *
             * @return the full name for the ref
             */
            QString name() const;

            /**
             * Resolve a symbolic reference
             *
             * Thie method iteratively peels a symbolic reference
             * until it resolves to a direct reference to an OID.
             *
             * If a direct reference is passed as an argument,
             * that reference is returned immediately
             *
             * @param resolvedRef Pointer to the peeled reference
             * @return 0 on success; error code otherwise
             */
            int resolve(QGitRef* resolvedRef);

            /**
             * Write a reference back to disk.
             *
             * The reference must have a valid name and a valid target
             * (either direct or symbolic).
             *
             * If the reference has been loaded from disk and no changes
             * have been made, no action will take place.
             *
             * The writing to disk is atomic.
             *
             * @return 0 on success; error code otherwise
             */
            int write();

            /**
             * Get the repository where a reference resides
             *
             * @return a pointer to the repository
             */
            QGitRepository owner();

            /**
             * Set the name of a reference.
             *
             * This marks the reference as modified; changes
             * won't take effect until it is manually written back
             * to disk.
             *
             * @param name The new name for the reference
             */
            void setName(const QString& name);

            /**
             * Set the target reference of a reference.
             *
             * This converts the reference into a symbolic
             * reference.
             *
             * This marks the reference as modified; changes
             * won't take effect until it is manually written back
             * to disk.
             *
             * @param target The new target for the reference
             */
            int setTarget(const QString& target);

            /**
             * Set the OID target of a reference.
             *
             * This converts the reference into a direct
             * reference.
             *
             * This marks the reference as modified; changes
             * won't take effect until it is manually written back
             * to disk.
             *
             * @param target The new target OID for the reference
             */
            void setOId(const QGitOId& oid);

            git_reference* data() const;
            const git_reference* constData() const;

        private:
            git_reference *m_reference;
    };
}

#endif // LIBQGIT2_REF_H
