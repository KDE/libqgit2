/******************************************************************************
 * This file is part of the libqgit2 library
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

#include <QtCore/QSharedPointer>
#include <QtCore/QStringList>

#include "libqgit2_export.h"

#include "qgitdatabase.h"
#include "qgitobject.h"
#include "qgitref.h"
#include "qgitindex.h"
#include "qgitstatuslist.h"
#include "qgitstatusoptions.h"

namespace LibQGit2
{
    class QGitCommit;
    class QGitConfig;
    class QGitTag;
    class QGitTree;
    class QGitBlob;
    class QGitSignature;

    /**
     * @brief Wrapper class for git_repository.
     * Represents a Git repository.
     *
     * @ingroup LibQGit2
     * @{
     */
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
             * Look for a git repository and return its path. The lookup start from startPath and
             * walk across parent directories if nothing has been found. The lookup ends when the
             * first repository is found, or when reaching a directory that is referenced in
             * ceilingDirs, or when the filesystem changes (unless acrossFs is true).
             *
             * The method will automatically detect if the repository is bare (if there is a
             * repository).
             *
             * The function will only return successfully if a repository was found, otherwise an
             * exception is thrown, providing an error message.
             *
             * @param startPath
             * The base path where the lookup starts.
             *
             * @param acrossFs
             * If true, then the lookup will not stop when a filesystem device change is detected
             * while exploring parent directories.
             *
             * @param ceilingDirs
             * A list of absolute symbolic link free paths. The lookup will stop if any of these
             * paths are reached. Note that the lookup always performs on startPath no matter if
             * startPath appears in ceilingDirs.
             *
             * @return The path of the found repository
             * @throws QGitException
             */
            static QString discover(const QString& startPath,
                                    bool acrossFs = false,
                                    const QStringList& ceilingDirs = QStringList());

            /**
             * Constructs a new Git repository in the given folder.
             *
             * @param path the path to the repository
             * @param isBare if true, a Git repository without a working directory is created
             * at the pointed path. If false, provided path will be considered as the working
             * directory into which the .git directory will be created.
             *
             * @throws QGitException
             */
            void init(const QString& path, bool isBare);

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
             * @throws QGitException
             */
            void open(const QString& path);

            /**
             * Convenience function for finding and opening a git repository.
             *
             * Calls discover() with the given arguments, and passes the result to open().
             *
             * @throws QGitException
             */
            void discoverAndOpen(const QString &startPath,
                                 bool acrossFs = false,
                                 const QStringList &ceilingDirs = QStringList());

            /**
             * Retrieve and resolve the reference pointed at by HEAD.
             *
             * @throws QGitException
             */
            Reference head() const;

            /**
             * Check if a repository's HEAD is detached
             *
             * A repository's HEAD is detached when it points directly to a commit
             * instead of a branch.
             *
             * @throws QGitException
             */
            bool isHeadDetached() const;

            /**
             * Check if the current branch is an orphan
             *
             * An orphan branch is one named from HEAD but which doesn't exist in
             * the refs namespace, because it doesn't have any commit to point to.
             *
             * @throws QGitException
             */
            bool isHeadOrphan() const;

            /**
             * Check if a repository is empty
             *
             * An empty repository has just been initialized and contains
             * no commits.
             *
             * @throws QGitException
             */
            bool isEmpty() const;

            /**
             * Check if a repository is bare
             *
             * @throws QGitException
             */
            bool isBare() const;

            /**
             * The name equals the repositories working directory name.
             * In case of a bare repository, the name equals the repositorie's directory.
             */
            QString name() const;

            /**
             * Get the path to the repository
             */
            QString path() const;

            /**
             * Get the path to the working directory
             */
            QString workDirPath() const;

            /**
             * The repositories configuration file. Includes the global git configuration file.
             */
            QGitConfig configuration() const;

            /**
             * Lookup a reference by its name in a repository.
             *
             * @throws QGitException
             * @return The reference with the given name
             */
            Reference* lookupRef(const QString& name) const;

            /**
             * Lookup a reference by its name in a repository and returns the oid of its target.
             *
             * @throws QGitException
             * @return The OId of the target 
             */
            QGitOId* lookupRefOId(const QString& name) const;

            /**
             * Lookup a reference by its shorthand name in a repository.
             *
             * @throws QGitException
             * @return The reference with the given name
             */
            Reference* lookupShorthandRef(const QString& shorthand) const;
            
            /**
             * Lookup a commit object from a repository.
             *
             * @throws QGitException
             */
            QGitCommit lookupCommit(const QGitOId& oid) const;

            /**
             * Lookup a tag object from the repository.
             *
             * @throws QGitException
             */
            QGitTag lookupTag(const QGitOId& oid) const;

            /**
             * Lookup a tree object from the repository.
             *
             * @throws QGitException
             */
            QGitTree lookupTree(const QGitOId& oid) const;

            /**
             * Lookup a blob object from a repository.
             *
             * @throws QGitException
             */
            QGitBlob lookupBlob(const QGitOId& oid) const;

            /**
             * Lookup a reference to one of the objects in a repostory.
             *
             * @throws QGitException
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
             *
             * @throws QGitException
             */
            Reference* createRef(const QString& name, const QGitOId& oid, bool overwrite = true);

            /**
             * Create a new symbolic reference.
             *
             * The reference will be created in the repository and written
             * to the disk.
             *
             * If `overwrite` is true and there already exists a reference
             * with the same name, it will be overwritten.
             *
             * @throws QGitException
             */
            Reference* createSymbolicRef(const QString& name, const QString& target, bool overwrite = true);

            /**
             * Create a new commit in the repository
             *
             * @throws QGitException
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
             *
             * @throws QGitException
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
             *
             * @throws QGitException
             */
            QGitOId createTag(const QString& name,
                              const QGitObject& target,
                              const QGitSignature& tagger,
                              const QString& message,
                              bool overwrite = true);

            /**
             * Delete an existing tag reference.
             *
             * @throws QGitException
             */
            void deleteTag(const QString& name);

            /**
             * Read a file from the working folder of a repository
             * and write it to the Object Database as a loose blob
             *
             * @throws QGitException
             */
            QGitOId createBlobFromFile(const QString& path);

            /**
             * Write an in-memory buffer to the ODB as a blob
             *
             * @throws QGitException
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
             * @throws QGitException
             */
            QStringList listTags(const QString& pattern = QString()) const;

            /**
             * Create a list with all references in the Repository.
             *
             * @param pattern Standard fnmatch pattern
             * @throws QGitException
             */
            QStringList listReferences() const;

            /**
             * @brief Get the object database behind a Git repository
             * 
             * @return a pointer to the object db
             */
            LibQGit2::QGitDatabase database() const;

            /**
             * @brief Get the Index file of a Git repository
             * 
             * This is a cheap operation; the index is only opened on the first call,
             * and subsequent calls only retrieve the previous pointer.
             *
             * @throws QGitException
             * @return The index file of the repository
             */
            QGitIndex index() const;

            /**
             * @brief Get the status information of the Git repository
             * 
             * This function returns the status of the repository entries, according to
             * the given options.
             * 
             * @throws QGitException
             * @return The list of status entries
             */
            QGitStatusList status(const QGitStatusOptions *options) const;
            
            git_repository* data() const;
            const git_repository* constData() const;

        private:
            typedef QSharedPointer<git_repository> ptr_type;
            ptr_type d;
    };

    /**@}*/
}

#endif // LIBQGIT2_REPOSITORY_H
