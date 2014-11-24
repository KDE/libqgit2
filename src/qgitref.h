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

#ifndef LIBQGIT2_REF_H
#define LIBQGIT2_REF_H

#include <QtCore/QSharedPointer>
#include <QtCore/QString>

#include "git2.h"

#include "qgitsignature.h"
#include "libqgit2_config.h"

namespace LibQGit2
{
    class Repository;
    class OId;

    /**
     * @brief Wrapper class for git_refs.
     * Represents a Git reference. Reference objects are branches, tags, etc.
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_EXPORT Reference
    {
        public:

            /**
             * Create an new ref object
             */
            explicit Reference(git_reference *ref = 0);

            /**
             * Copy constructor
             */
            Reference(const Reference& other);

            /**
             * Free an existing reference object.
             */
            ~Reference();

        public:

            /**
             * Get the OID pointed to by a reference.
             *
             * Only available if the reference is direct (i.e. not symbolic)
             *
             * @return a pointer to the oid if available, NULL otherwise
             */
            OId target() const;

            /**
             * Get full name to the reference pointed by this reference
             *
             * Only available if the reference is symbolic
             *
             * @return a pointer to the name if available, NULL otherwise
             */
            QString symbolicTarget() const;

            /**
             * Return true if the reference is direct (i.e. a reference to an OID)
             */
            bool isDirect() const;

            /**
             * Return true if the reference is symbolig (i.e. a reference to another ref)
             */
            bool isSymbolic() const;

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
             * @throws LibQGit2::Exception
             */
            Reference resolve() const;

            /**
             * Get the repository where a reference resides
             *
             * @return a pointer to the repository
             */
            Repository owner() const;

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
             * @param signature The identity that will used to populate the reflog entry
             * @param message The one line long message to be appended to the reflog
             * @throws LibQGit2::Exception
             */
            void setSymbolicTarget(const QString& target, const Signature &signature = Signature(), const QString &message = QString());

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
             * @param signature The identity that will used to populate the reflog entry
             * @param message The one line long message to be appended to the reflog
             * @throws LibQGit2::Exception
             */
            void setTarget(const OId& oid, const Signature &signature = Signature(), const QString &message = QString());

            bool isNull() const;

            git_reference* data() const;
            const git_reference* constData() const;

        private:
            typedef QSharedPointer<git_reference> ptr_type;
            ptr_type d;
    };

    /**@}*/
}

#endif // LIBQGIT2_REF_H
