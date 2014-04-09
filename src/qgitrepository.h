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

#ifndef LIBQGIT2_REPOSITORY_H
#define LIBQGIT2_REPOSITORY_H

#include <QtCore/QSharedPointer>
#include <QtCore/QStringList>
#include <QtCore/QObject>
#include <QtCore/QMap>

#include "libqgit2_export.h"

#include "qgitdatabase.h"
#include "qgitobject.h"
#include "qgitref.h"
#include "qgitindex.h"
#include "qgitstatuslist.h"
#include "qgitstatusoptions.h"

namespace LibQGit2
{
    class Commit;
    class Config;
    class Tag;
    class Tree;
    class Blob;
    class Signature;
    class Credentials;
    class Push;

    /**
     * @brief Wrapper class for git_repository.
     * Represents a Git repository.
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_EXPORT Repository : public QObject
    {
            Q_OBJECT

        public:

            /**
             * Construct a wrapper around a libgit2 repository pointer.
             *
             * If `own` is true, this Repository takes ownership of the pointer, and makes
             * sure it is freed when the owner is deleted, unless there are other instances
             * sharing the ownership. If `own` is true, the pointer must not be freed manually,
             * and must not be passed to another Repository instance also with `own` true.
             */
            explicit Repository(git_repository *repository = 0, bool own = false);

            /**
             * Copy constructor; creates a copy of the object, sharing the same underlaying data
             * structure.
             */
            Repository(const Repository& other);

            /**
             * Destruct a previously allocated repository
             */
            ~Repository();

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
             * @throws LibQGit2::Exception
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
             * @throws LibQGit2::Exception
             */
            void init(const QString& path, bool isBare = false);

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
             * @throws LibQGit2::Exception
             */
            void open(const QString& path);

            /**
             * Convenience function for finding and opening a git repository.
             *
             * Calls discover() with the given arguments, and passes the result to open().
             *
             * @throws LibQGit2::Exception
             */
            void discoverAndOpen(const QString &startPath,
                                 bool acrossFs = false,
                                 const QStringList &ceilingDirs = QStringList());

            /**
             * Retrieve and resolve the reference pointed at by HEAD.
             *
             * @throws LibQGit2::Exception
             */
            Reference head() const;

            /**
             * Check if a repository's HEAD is detached
             *
             * A repository's HEAD is detached when it points directly to a commit
             * instead of a branch.
             *
             * @throws LibQGit2::Exception
             */
            bool isHeadDetached() const;

            /**
             * Check if the current branch is unborn
             *
             * An unborn branch is one named from HEAD but which doesn't exist
             * in the refs namespace, because it doesn't have any commit to point to.
             *
             * @throws LibQGit2::Exception
             */
            bool isHeadUnborn() const;

            /**
             * Check if a repository is empty
             *
             * An empty repository has just been initialized and contains
             * no commits.
             *
             * @throws LibQGit2::Exception
             */
            bool isEmpty() const;

            /**
             * Check if a repository is bare
             *
             * @throws LibQGit2::Exception
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
            Config configuration() const;

            /**
             * Lookup a reference by its name in a repository.
             *
             * @throws LibQGit2::Exception
             * @return The reference with the given name
             */
            Reference* lookupRef(const QString& name) const;

            /**
             * Lookup a reference by its name in a repository and returns the oid of its target.
             *
             * @throws LibQGit2::Exception
             * @return The OId of the target
             */
            OId* lookupRefOId(const QString& name) const;

            /**
             * Lookup a reference by its shorthand name in a repository.
             *
             * @throws LibQGit2::Exception
             * @return The reference with the given name
             */
            Reference* lookupShorthandRef(const QString& shorthand) const;

            /**
             * Lookup a commit object from a repository.
             *
             * @throws LibQGit2::Exception
             */
            Commit lookupCommit(const OId& oid) const;

            /**
             * Lookup a tag object from the repository.
             *
             * @throws LibQGit2::Exception
             */
            Tag lookupTag(const OId& oid) const;

            /**
             * Lookup a tree object from the repository.
             *
             * @throws LibQGit2::Exception
             */
            Tree lookupTree(const OId& oid) const;

            /**
             * Lookup a blob object from a repository.
             *
             * @throws LibQGit2::Exception
             */
            Blob lookupBlob(const OId& oid) const;

            /**
             * Lookup a reference to one of the objects in a repostory.
             *
             * @throws LibQGit2::Exception
             */
            Object lookupAny(const OId& oid) const;

            /**
             * Create a new object id reference.
             *
             * The reference will be created in the repository and written
             * to the disk.
             *
             * If `overwrite` is true and there already exists a reference
             * with the same name, it will be overwritten.
             *
             * @throws LibQGit2::Exception
             */
            Reference* createRef(const QString& name, const OId& oid, bool overwrite = true);

            /**
             * Create a new symbolic reference.
             *
             * The reference will be created in the repository and written
             * to the disk.
             *
             * If `overwrite` is true and there already exists a reference
             * with the same name, it will be overwritten.
             *
             * @throws LibQGit2::Exception
             */
            Reference* createSymbolicRef(const QString& name, const QString& target, bool overwrite = true);

            /**
             * Create a new commit in the repository
             *
             * @throws LibQGit2::Exception
             */
            OId createCommit(const QString& ref,
                                 const Signature& author,
                                 const Signature& committer,
                                 const QString& message,
                                 const Tree& tree,
                                 const QList<Commit>& parents);

            /**
             * Create a new lightweight tag pointing at a target object
             *
             * A new direct reference will be created pointing to
             * this target object. If `force` is true and a reference
             * already exists with the given name, it'll be replaced.
             *
             * @throws LibQGit2::Exception
             */
            OId createTag(const QString& name,
                              const Object& target,
                              bool overwrite = true);

            /**
             * Create a new tag in the repository from an object
             *
             * A new reference will also be created pointing to
             * this tag object. If `overwrite` is true and a reference
             * already exists with the given name, it'll be replaced.
             *
             * @throws LibQGit2::Exception
             */
            OId createTag(const QString& name,
                              const Object& target,
                              const Signature& tagger,
                              const QString& message,
                              bool overwrite = true);

            /**
             * Delete an existing tag reference.
             *
             * @throws LibQGit2::Exception
             */
            void deleteTag(const QString& name);

            /**
             * Read a file from the working folder of a repository
             * and write it to the Object Database as a loose blob
             *
             * @throws LibQGit2::Exception
             */
            OId createBlobFromFile(const QString& path);

            /**
             * Write an in-memory buffer to the ODB as a blob
             *
             * @throws LibQGit2::Exception
             */
            OId createBlobFromBuffer(const QByteArray& buffer);

            /**
             * Create a list with all the tags in the Repository
             * which name match a defined pattern
             *
             * If an empty pattern is provided, all the tags
             * will be returned.
             *
             * @param pattern Standard fnmatch pattern
             * @throws LibQGit2::Exception
             */
            QStringList listTags(const QString& pattern = QString()) const;

            /**
             * Create a list with all references in the Repository.
             *
             * @param pattern Standard fnmatch pattern
             * @throws LibQGit2::Exception
             */
            QStringList listReferences() const;

            /**
             * @brief Get the object database behind a Git repository
             *
             * @return a pointer to the object db
             */
            LibQGit2::Database database() const;

            /**
             * @brief Get the Index file of a Git repository
             *
             * This is a cheap operation; the index is only opened on the first call,
             * and subsequent calls only retrieve the previous pointer.
             *
             * @throws LibQGit2::Exception
             * @return The index file of the repository
             */
            Index index() const;

            /**
             * @brief Get the status information of the Git repository
             *
             * This function returns the status of the repository entries, according to
             * the given options.
             *
             * @throws LibQGit2::Exception
             * @return The list of status entries
             */
            StatusList status(const StatusOptions *options) const;

            /**
             * @brief Sets a \c Credentials object to be used for a named remote.
             *
             * Some remotes require authentication when communicating with them. Authentication
             * is performed by using \c Credentials objects. Each named remote can have its own
             * \c Credentials object. The credentials for a remote must be set using this method
             * before trying to communicate with it.
             *
             * @param remoteName the name of the remote
             * @param credentials the \c Credentials to be used for the remote
             */
            void setRemoteCredentials(const QString& remoteName, Credentials credentials);

            /**
            * Clone a git repository.
            *
            * Signal cloneProgress(int) is emitted with progress in percent.
            *
            * @param url URL of the git repository
            * @param path non-existing directory for the new clone
            * @throws LibQGit2::Exception
            */
            void clone(const QString& url, const QString& path);

            /**
            * Add remote repository
            *
            * @param name name of the remote
            * @param url URL of the reote git repository
            * @throws LibQGit2::Exception
            */
            void remoteAdd(const QString& name, const QString& url);

            /**
            * Fetch from known remote repository.
            *
            * @param remote name of the remote repository (e.g. "origin")
            * @param head name of head to fetch (e.g. "master"), default: "*" (all branches)
            * @throws LibQGit2::Exception
            */
            void fetch(const QString& remote, const QString& head = "");

            QStringList remoteBranches(const QString& remoteName);


            /**
            * Checkout a remote branch without creating a local branch.
            *
            * @param branch  branch name
            * @param force   use forced checkout, default is false
            * @param remote  remote which should be used, default is 'origin'
            * @throws LibQGit2::Exception
            */
            void checkoutRemote(const QString& branch, bool force = false, const QString& remote = "origin");

            /**
			 * Gets a \c Push object for the named remote.
			 *
			 * @param remoteName the name of the remote to push to.
			 * @return An object to be used for pushing to the named remote.
			 * @throws LibQGit2::Exception
             */
            Push push(const QString &remoteName);

            git_repository* data() const;
            const git_repository* constData() const;

        signals:
            void cloneProgress(int);

        private:
            typedef QSharedPointer<git_repository> ptr_type;
            ptr_type d;

            QMap<QString, Credentials> m_remote_credentials;
    };

    /**@}*/
}

#endif // LIBQGIT2_REPOSITORY_H
