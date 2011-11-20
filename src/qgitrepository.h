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

#ifndef LIBQGIT2_REPOSITORY_H
#define LIBQGIT2_REPOSITORY_H

#include "libqgit2_export.h"

#include "qgitdatabase.h"
#include "qgitobject.h"
#include "qgitref.h"
#include "qgitindex.h"

#include <git2/repository.h>

namespace LibQGit2
{
    class QGitCommit;
    class QGitTree;
    class QGitSignatureRef;

    class LIBQGIT2_REPOSITORY_EXPORT QGitRepository
    {
        public:
            /**
             * Constructs a new Git repository in the given folder.
             *
             * TODO:
             * - Reinit the repository
             * - Create config files
             *
             * @param path the path to the repository
             * @param isBare if true, a Git repository without a working directory is created
             * at the pointed path. If false, provided path will be considered as the working
             * directory into which the .git directory will be created.
             *
             * @return 0 on success; error code otherwise
             */
            explicit QGitRepository(const QString& path, unsigned isBare);
            explicit QGitRepository(git_repository *repository = 0);

            QGitRepository( const QGitRepository& other );

            /**
             * Destruct a previously allocated repository
             */
            ~QGitRepository();

        public:
            /**
             * Open a git repository.
             *
             * The 'path' argument must point to an existing git repository
             * folder, e.g.
             *
             * /path/to/my_repo/.git/ (normal repository)
             * objects/
             * index
             * HEAD
             *
             * /path/to/bare_repo/ (bare repository)
             * objects/
             * index
             * HEAD
             *
             * The method will automatically detect if 'path' is a normal
             * or bare repository or fail is 'path' is neither.
             *
             * @param path the path to the repository
             * @return 0 on success; error code otherwise
             */
            int open(const QString& path);

            /**
             * Open a git repository by manually specifying all its paths
             *
             * @param gitDir The full path to the repository folder
             * e.g. a '.git' folder for live repos, any folder for bare
             * Equivalent to $GIT_DIR.
             * Cannot be NULL.
             *
             * @param gitObjectDirectory The full path to the ODB folder.
             * the folder where all the loose and packed objects are stored
             * Equivalent to $GIT_OBJECT_DIRECTORY.
             * If NULL, "$GIT_DIR/objects/" is assumed.
             *
             * @param gitIndexFile The full path to the index (dircache) file
             * Equivalent to $GIT_INDEX_FILE.
             * If NULL, "$GIT_DIR/index" is assumed.
             *
             * @param gitWorkTree The full path to the working tree of the repository,
             * if the repository is not bare.
             * Equivalent to $GIT_WORK_TREE.
             * If NULL, the repository is assumed to be bare.
             *
             * @return 0 on success; error code otherwise
             */
            int open2(const QString& gitDir,
                      const QString& gitObjectDirectory,
                      const QString& gitIndexFile,
                      const QString& gitWorkTree);


            /**
             * Open a git repository by manually specifying its paths and
             * the object database it will use.
             *
             * @param gitDir The full path to the repository folder
             * e.g. a '.git' folder for live repos, any folder for bare
             * Equivalent to $GIT_DIR.
             * Cannot be NULL.
             *
             * @param objectDatabase A pointer to a git_odb created & initialized
             * by the user (e.g. with custom backends). This object database
             * will be owned by the repository and will be automatically free'd.
             * It should not be manually free'd by the user, or this
             * git_repository object will become invalid.
             *
             * @param gitIndexFile The full path to the index (dircache) file
             * Equivalent to $GIT_INDEX_FILE.
             * If NULL, "$GIT_DIR/index" is assumed.
             *
             * @param gitWorkTree The full path to the working tree of the repository,
             * if the repository is not bare.
             * Equivalent to $GIT_WORK_TREE.
             * If NULL, the repository is assumed to be bare.
             *
             * @return 0 on success; error code otherwise
             */
            int open3(const QString& gitDir,
                      QGitDatabase *objectDatabase,
                      const QString& gitIndexFile,
                      const QString& gitWorkTree);

            /**
             * Lookup a reference to one of the objects in the repostory.
             *
             * The generated reference is owned by the repository and
             * should not be freed by the user.
             *
             * The 'type' parameter must match the type of the object
             * in the odb; the method will fail otherwise.
             * The special value 'GIT_OBJ_ANY' may be passed to let
             * the method guess the object's type.
             *
             * @param object pointer to the looked-up object
             * @param id the unique identifier for the object
             * @param type the type of the object
             * @return a reference to the object
             */
            int lookup(QGitObject& object, const QGitOId& id, git_otype type);

            /**
             * Lookup a reference given it's name
             */
            QGitRef lookup(const QString& name);

            QGitCommit lookupCommit(const QGitOId& oid);

            QGitRef createRef(const QString& name, const QGitOId& oid, bool force = true);
            QGitRef createSymbolicRef(const QString& name, const QString& target, bool force = true);

            QGitOId createCommit(const QString& ref,
                                 const QGitSignatureRef& author,
                                 const QGitSignatureRef& committer,
                                 const QString& message,
                                 const QGitTree& tree,
                                 const QList<QGitCommit>& parents);

            /**
             * Get the object database behind a Git repository
             *
             * @return a pointer to the object db
             */
            QGitDatabase* database() const;

            /**
             * Get the Index file of a Git repository
             *
             * This is a cheap operation; the index is only opened on the first call,
             * and subsequent calls only retrieve the previous pointer.
             *
             * @param index Pointer where to store the index
             * @return 0 on success; error code if the index could not be opened
             */
            int index(QGitIndex& index) const;

            git_repository* data() const;
            const git_repository* constData() const;

        private:
            git_repository *m_repository;
    };
}

#endif // LIBQGIT2_REPOSITORY_H
