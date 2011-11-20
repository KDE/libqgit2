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

#include <git2/commit.h>
#include <git2/tag.h>

#include <QtCore/QFile>
#include <QtCore/QVector>

using namespace LibQGit2;

QGitRepository::QGitRepository(const QString& path, unsigned isBare)
{
    git_repository_init(&m_repository, QFile::encodeName(path), isBare);
}

QGitRepository::QGitRepository(git_repository *repository)
    : m_repository(repository)
{
}

QGitRepository::QGitRepository( const QGitRepository& other )
{
    m_repository = other.m_repository;
}

QGitRepository::~QGitRepository()
{
    git_repository_free(m_repository);
}

int QGitRepository::open(const QString& path)
{
    return git_repository_open(&m_repository, QFile::encodeName(path));
}

int QGitRepository::open(const QString& gitDir,
                         const QString& gitObjectDirectory,
                         const QString& gitIndexFile,
                         const QString& gitWorkTree)
{
    return git_repository_open2(&m_repository,
                                QFile::encodeName(gitDir),
                                QFile::encodeName(gitObjectDirectory),
                                QFile::encodeName(gitIndexFile),
                                QFile::encodeName(gitWorkTree));
}


int QGitRepository::open(const QString& gitDir,
                         QGitDatabase *objectDatabase,
                         const QString& gitIndexFile,
                         const QString& gitWorkTree)
{
    return git_repository_open3(&m_repository,
                                QFile::encodeName(gitDir),
                                objectDatabase->data(),
                                QFile::encodeName(gitIndexFile),
                                QFile::encodeName(gitWorkTree));
}

QGitRef QGitRepository::lookupRef(const QString& name) const
{
    git_reference *ref;
    git_reference_lookup(&ref, data(), QFile::encodeName(name));
    return QGitRef(ref);
}

QGitCommit QGitRepository::lookupCommit(const QGitOId& oid) const
{
    git_commit *commit;
    git_commit_lookup(&commit, data(), oid.data());
    return QGitCommit(commit);
}

QGitTag QGitRepository::lookupTag(const QGitOId& oid) const
{
    git_tag *tag;
    git_tag_lookup(&tag, data(), oid.data());
    return QGitTag(tag);
}

QGitTree QGitRepository::lookupTree(const QGitOId& oid) const
{
    git_tree *tree;
    git_tree_lookup(&tree, data(), oid.data());
    return QGitTree(tree);
}

QGitBlob QGitRepository::lookupBlob(const QGitOId& oid) const
{
    git_blob *blob;
    git_blob_lookup(&blob, data(), oid.data());
    return QGitBlob(blob);
}

QGitObject QGitRepository::lookupAny(const QGitOId &id) const
{
    git_object *object;
    git_object_lookup(&object, data(), id.data(), GIT_OBJ_ANY);
    return QGitObject(object);
}

QGitRef QGitRepository::createRef(const QString& name, const QGitOId& oid, bool overwrite)
{
    git_reference *ref;
    git_reference_create_oid(&ref, data(), QFile::encodeName(name), oid.data(), overwrite);
    return QGitRef(ref);
}

QGitRef QGitRepository::createSymbolicRef(const QString& name, const QString& target, bool overwrite)
{
    git_reference *ref;
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
    QGitDatabase *database = new QGitDatabase(git_repository_database(m_repository));
    return database;
}

int QGitRepository::index(QGitIndex& index) const
{
    git_index *idx;
    int ret = git_repository_index(&idx, m_repository);
    index.reset(idx);
    return ret;
}

git_repository* QGitRepository::data() const
{
    return m_repository;
}

const git_repository* QGitRepository::constData() const
{
    return m_repository;
}

