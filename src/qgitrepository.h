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

#include <QtCore/QSharedPointer>

struct git_repository;

namespace LibQGit2
{
    class QGitCommit;
    class QGitTag;
    class QGitTree;
    class QGitBlob;
    class QGitSignature;

    class LIBQGIT2_REPOSITORY_EXPORT QGitRepository
    {
        public:

            /**
             * Construct a wrapper around a libgit2 repository pointer.
             *
             * If `own` is true, this QGitRepository takes ownership of the pointer, and makes
             * sure it is freed when the owner is deleted, unless there are other instances
             * sharing the ownership. If `own` is true, the pointer must not be freed manually,
             * and must not be passed to another QGitRepository instance also with `own` true.
             */
            explicit QGitRepository(git_repository *repository = 0, bool own = false);

            /**
             * Copy constructor; creates a copy of the object, sharing the same underlaying data
             * structure.
             */
            QGitRepository(const QGitRepository& other);

            /**
             * Destruct a previously allocated repository
             */
            ~QGitRepository();

            /**
             * Constructs a new Git repository in the given folder.
             *
             * @param path the path to the repository
             * @param isBare if true, a Git repository without a working directory is created
             * at the pointed path. If false, provided path will be considered as the working
             * directory into which the .git directory will be created.
             *
             * @return 0 on success; error code otherwise
             */
            int init(const QString& path, bool isBare);

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
            int open(const QString& gitDir,
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
            int open(const QString& gitDir,
                     QGitDatabase *objectDatabase,
                     const QString& gitIndexFile,
                     const QString& gitWorkTree);

            /**
             * Retrieve and resolve the reference pointed at by HEAD.
             */
            QGitRef head() const;

            /**
             * Check if a repository's HEAD is detached
             *
             * A repository's HEAD is detached when it points directly to a commit
             * instead of a branch.
             */
            bool isHeadDetached() const;

            /**
             * Check if the current branch is an orphan
             *
             * An orphan branch is one named from HEAD but which doesn't exist in
             * the refs namespace, because it doesn't have any commit to point to.
             */
            bool isHeadOrphan() const;

            /**
             * Check if a repository is empty
             *
             * An empty repository has just been initialized and contains
             * no commits.
             */
            bool isEmpty() const;

            /**
             * Check if a repository is bare
             */
            bool isBare() const;

            /**
             * Get the path to the repository
             */
            QString path() const;

            /**
             * Get the path to the index
             */
            QString indexPath() const;

            /**
             * Get the path to the ODB
             */
            QString databasePath() const;

            /**
             * Get the path to the working directory
             */
            QString workDirPath() const;

            /**
             * Lookup a reference by its name in a repository.
             */
            QGitRef lookupRef(const QString& name) const;

            /**
             * Lookup a commit object from a repository.
             */
            QGitCommit lookupCommit(const QGitOId& oid) const;

            /**
             * Lookup a tag object from the repository.
             */
            QGitTag lookupTag(const QGitOId& oid) const;

            /**
             * Lookup a tree object from the repository.
             */
            QGitTree lookupTree(const QGitOId& oid) const;

            /**
             * Lookup a blob object from a repository.
             */
            QGitBlob lookupBlob(const QGitOId& oid) const;

            /**
             * Lookup a reference to one of the objects in a repostory.
             */
            QGitObject lookupAny(const QGitOId& oid) const;

            /**
             * Create a new object id reference.
             *
             * The reference will be created in the repository and written
             * to the disk.
             *
             * If `overwrite` is true and there already exists a reference
             * with the same name, it will be overwritten.
             */
            QGitRef createRef(const QString& name, const QGitOId& oid, bool overwrite = true);

            /**
             * Create a new symbolic reference.
             *
             * The reference will be created in the repository and written
             * to the disk.
             *
             * If `overwrite` is true and there already exists a reference
             * with the same name, it will be overwritten.
             */
            QGitRef createSymbolicRef(const QString& name, const QString& target, bool overwrite = true);

            /**
             * Create a new commit in the repository
             */
            QGitOId createCommit(const QString& ref,
                                 const QGitSignature& author,
                                 const QGitSignature& committer,
                                 const QString& message,
                                 const QGitTree& tree,
                                 const QList<QGitCommit>& parents);

            /**
             * Create a new lightweight tag pointing at a target object
             *
             * A new direct reference will be created pointing to
             * this target object. If `force` is true and a reference
             * already exists with the given name, it'll be replaced.
             */
            QGitOId createTag(const QString& name,
                              const QGitObject& target,
                              bool overwrite = true);

            /**
             * Create a new tag in the repository from an object
             *
             * A new reference will also be created pointing to
             * this tag object. If `overwrite` is true and a reference
             * already exists with the given name, it'll be replaced.
             */
            QGitOId createTag(const QString& name,
                              const QGitObject& target,
                              const QGitSignature& tagger,
                              const QString& message,
                              bool overwrite = true);

            /**
             * Delete an existing tag reference.
             */
            void deleteTag(const QString& name);

            /**
             * Read a file from the working folder of a repository
             * and write it to the Object Database as a loose blob
             */
            QGitOId createBlobFromFile(const QString& path);

            /**
             * Write an in-memory buffer to the ODB as a blob
             */
            QGitOId createBlobFromBuffer(const QByteArray& buffer);

            /**
             * Create a list with all the tags in the Repository
             * which name match a defined pattern
             *
             * If an empty pattern is provided, all the tags
             * will be returned.
             *
             * @param pattern Standard fnmatch pattern
             */
            QStringList listTags(const QString& pattern = "");

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
             */
            QGitIndex index() const;

            git_repository* data() const;
            const git_repository* constData() const;

        private:
            typedef QSharedPointer<git_repository> ptr_type;
            ptr_type d;
    };
}

#endif // LIBQGIT2_REPOSITORY_H
