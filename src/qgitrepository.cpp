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

#include "qgitrepository.h"
#include "qgitcommit.h"
#include "qgittag.h"
#include "qgittree.h"
#include "qgitblob.h"
#include "qgitsignature.h"

#include <git2/errors.h>
#include <git2/repository.h>
#include <git2/refs.h>
#include <git2/commit.h>
#include <git2/tag.h>
#include <git2/tree.h>
#include <git2/blob.h>

#include <QtCore/QFile>
#include <QtCore/QVector>

using namespace LibQGit2;

namespace {
void do_not_free(git_repository*) {}
}

QGitRepository::QGitRepository(git_repository *repository, bool own)
    : d(repository, own ? git_repository_free : do_not_free)
{
}

QGitRepository::QGitRepository( const QGitRepository& other )
    : d(other.d)
{
}

QGitRepository::~QGitRepository()
{
}

int QGitRepository::init(const QString& path, bool isBare)
{
    d.clear();
    git_repository *repo = 0;
    int ret = git_repository_init(&repo, QFile::encodeName(path), isBare);
    if (ret == GIT_SUCCESS)
        d = ptr_type(repo, git_repository_free);
    return ret;
}

int QGitRepository::open(const QString& path)
{
    d.clear();
    git_repository *repo = 0;
    int ret = git_repository_open(&repo, QFile::encodeName(path));
    if (ret == GIT_SUCCESS)
        d = ptr_type(repo, git_repository_free);

    return ret;
}

int QGitRepository::open(const QString& gitDir,
                         const QString& gitObjectDirectory,
                         const QString& gitIndexFile,
                         const QString& gitWorkTree)
{
    d.clear();
    git_repository *repo = 0;
    int ret = git_repository_open2(&repo,
                                   QFile::encodeName(gitDir),
                                   QFile::encodeName(gitObjectDirectory),
                                   QFile::encodeName(gitIndexFile),
                                   QFile::encodeName(gitWorkTree));
    if (ret == GIT_SUCCESS)
        d = ptr_type(repo, git_repository_free);

    return ret;
}

int QGitRepository::open(const QString& gitDir,
                         QGitDatabase *objectDatabase,
                         const QString& gitIndexFile,
                         const QString& gitWorkTree)
{
    d.clear();
    git_repository *repo = 0;
    int ret = git_repository_open3(&repo,
                                   QFile::encodeName(gitDir),
                                   objectDatabase->data(),
                                   QFile::encodeName(gitIndexFile),
                                   QFile::encodeName(gitWorkTree));
    if (ret == GIT_SUCCESS)
        d = ptr_type(repo, git_repository_free);

    return ret;
}

QGitRef QGitRepository::lookupRef(const QString& name)
{
    git_reference *ref = 0;
    git_reference_lookup(&ref, data(), QFile::encodeName(name));
    return QGitRef(ref);
}

QGitCommit QGitRepository::lookupCommit(const QGitOId& oid)
{
    git_commit *commit = 0;
    git_commit_lookup_prefix(&commit, data(), oid.data(), oid.length());
    return QGitCommit(commit);
}

QGitTag QGitRepository::lookupTag(const QGitOId& oid)
{
    git_tag *tag = 0;
    git_tag_lookup_prefix(&tag, data(), oid.data(), oid.length());
    return QGitTag(tag);
}

QGitTree QGitRepository::lookupTree(const QGitOId& oid)
{
    git_tree *tree = 0;
    git_tree_lookup_prefix(&tree, data(), oid.data(), oid.length());
    return QGitTree(tree);
}

QGitBlob QGitRepository::lookupBlob(const QGitOId& oid)
{
    git_blob *blob = 0;
    git_blob_lookup_prefix(&blob, data(), oid.data(), oid.length());
    return QGitBlob(blob);
}

QGitObject QGitRepository::lookupAny(const QGitOId &oid)
{
    git_object *object = 0;
    git_object_lookup_prefix(&object, data(), oid.data(), oid.length(), GIT_OBJ_ANY);
    return QGitObject(object);
}

QGitRef QGitRepository::createRef(const QString& name, const QGitOId& oid, bool overwrite)
{
    git_reference *ref = 0;
    git_reference_create_oid(&ref, data(), QFile::encodeName(name), oid.data(), overwrite);
    return QGitRef(ref);
}

QGitRef QGitRepository::createSymbolicRef(const QString& name, const QString& target, bool overwrite)
{
    git_reference *ref = 0;
    git_reference_create_symbolic(&ref, data(), QFile::encodeName(name), QFile::encodeName(target), overwrite);
    return QGitRef(ref);
}

QGitOId QGitRepository::createCommit(const QString& ref,
                                     const QGitSignatureRef& author,
                                     const QGitSignatureRef& committer,
                                     const QString& message,
                                     const QGitTree& tree,
                                     const QList<QGitCommit>& parents)
{
    QVector<const git_commit*> p;
    foreach (const QGitCommit& parent, parents) {
        p.append(parent.data());
    }

    QGitOId oid;
    git_commit_create(oid.data(), data(), QFile::encodeName(ref), author.data(), committer.data(),
                      NULL, message.toUtf8(), tree.data(), p.size(), p.data());
    return oid;
}

QGitOId QGitRepository::createTag(const QString& name,
                                  const QGitObject& target,
                                  const QGitSignatureRef& tagger,
                                  const QString& message,
                                  bool overwrite)
{
    QGitOId oid;
    git_tag_create(oid.data(), data(), QFile::encodeName(name), target.data(),
                   tagger.data(), message.toUtf8(), overwrite);
    return oid;
}

QGitOId QGitRepository::createBlobFromFile(const QString& path)
{
    QGitOId oid;
    git_blob_create_fromfile(oid.data(), data(), QFile::encodeName(path));
    return oid;
}

QGitOId QGitRepository::createBlobFromBuffer(const QByteArray& buffer)
{
    QGitOId oid;
    git_blob_create_frombuffer(oid.data(), data(), buffer.data(), buffer.size());
    return oid;
}

QGitDatabase* QGitRepository::database() const
{
    QGitDatabase *database = new QGitDatabase(git_repository_database(data()));
    return database;
}

int QGitRepository::index(QGitIndex& index) const
{
    git_index *idx;
    int ret = git_repository_index(&idx, data());
    index.reset(idx);
    return ret;
}

git_repository* QGitRepository::data() const
{
    return d.data();
}

const git_repository* QGitRepository::constData() const
{
    return d.data();
}

