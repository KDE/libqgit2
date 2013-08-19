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

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QVector>
#include <QtCore/QStringList>

#include <qgitrepository.h>
#include <qgitcommit.h>
#include <qgitconfig.h>
#include <qgittag.h>
#include <qgittree.h>
#include <qgitblob.h>
#include <qgitsignature.h>
#include <qgitexception.h>
#include <qgitstatus.h>

namespace {
void do_not_free(git_repository*) {}
}

namespace LibQGit2
{

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

QString QGitRepository::discover(const QString& startPath, bool acrossFs, const QStringList& ceilingDirs)
{
    QByteArray repoPath(GIT_PATH_MAX, Qt::Uninitialized);
    QByteArray joinedCeilingDirs = QFile::encodeName(ceilingDirs.join(QChar(GIT_PATH_LIST_SEPARATOR)));
    qGitThrow(git_repository_discover(repoPath.data(), repoPath.length(),
                                      QFile::encodeName(startPath),
                                      acrossFs, joinedCeilingDirs));
    return QFile::decodeName(repoPath);
}

void QGitRepository::init(const QString& path, bool isBare)
{
    d.clear();
    git_repository *repo = 0;
    qGitThrow(git_repository_init(&repo, QFile::encodeName(path), isBare));
    d = ptr_type(repo, git_repository_free);
}

void QGitRepository::open(const QString& path)
{
    d.clear();
    git_repository *repo = 0;
    qGitThrow(git_repository_open(&repo, QFile::encodeName(path)));
    d = ptr_type(repo, git_repository_free);
}

void QGitRepository::discoverAndOpen(const QString &startPath,
                                     bool acrossFs,
                                     const QStringList &ceilingDirs)
{
    open(discover(startPath, acrossFs, ceilingDirs));
}

Reference QGitRepository::head() const
{
    git_reference *ref = 0;
    qGitThrow(git_repository_head(&ref, data()));
    return Reference(ref);
}

bool QGitRepository::isHeadDetached() const
{
    return qGitThrow(git_repository_head_detached(data())) == 1;
}

bool QGitRepository::isHeadOrphan() const
{
    return qGitThrow(git_repository_head_orphan(data())) == 1;
}

bool QGitRepository::isEmpty() const
{
    return qGitThrow(git_repository_is_empty(data())) == 1;
}

bool QGitRepository::isBare() const
{
    return qGitThrow(git_repository_is_bare(data())) == 1;
}

QString QGitRepository::name() const
{
    QString repoPath = QDir::cleanPath( workDirPath() );
    if (isBare())
        repoPath = QDir::cleanPath( path() );

    return QFileInfo(repoPath).fileName();
}

QString QGitRepository::path() const
{
    return QFile::decodeName(git_repository_path(data()));
}

QString QGitRepository::workDirPath() const
{
    return QFile::decodeName(git_repository_workdir(data()));
}

Config QGitRepository::configuration() const
{
    git_config *cfg;
    qGitThrow( git_repository_config(&cfg, data()) );
    return Config(cfg);
}

Reference* QGitRepository::lookupRef(const QString& name) const
{
    git_reference *ref = 0;
    qGitThrow(git_reference_lookup(&ref, data(), QFile::encodeName(name)));
    Reference* qr = new Reference(ref);
    return qr;
}

QGitOId* QGitRepository::lookupRefOId(const QString& name) const
{
    git_oid oid;
    qGitThrow(git_reference_name_to_id(&oid, data(), QFile::encodeName(name)));
    QGitOId* qo = new QGitOId(&oid);
    return qo;
}

Reference* QGitRepository::lookupShorthandRef(const QString& shorthand) const
{
    git_reference *ref = 0;
    qGitThrow(git_reference_dwim(&ref, data(), QFile::encodeName(shorthand)));
    Reference* qr = new Reference(ref);
    return qr;
}

Commit QGitRepository::lookupCommit(const QGitOId& oid) const
{
    git_commit *commit = 0;
    qGitThrow(git_commit_lookup_prefix(&commit, data(), oid.constData(), oid.length()));
    return Commit(commit);
}

QGitTag QGitRepository::lookupTag(const QGitOId& oid) const
{
    git_tag *tag = 0;
    qGitThrow(git_tag_lookup_prefix(&tag, data(), oid.constData(), oid.length()));
    return QGitTag(tag);
}

QGitTree QGitRepository::lookupTree(const QGitOId& oid) const
{
    git_tree *tree = 0;
    qGitThrow(git_tree_lookup_prefix(&tree, data(), oid.constData(), oid.length()));
    return QGitTree(tree);
}

Blob QGitRepository::lookupBlob(const QGitOId& oid) const
{
    git_blob *blob = 0;
    qGitThrow(git_blob_lookup_prefix(&blob, data(), oid.constData(), oid.length()));
    return Blob(blob);
}

QGitObject QGitRepository::lookupAny(const QGitOId &oid) const
{
    git_object *object = 0;
    qGitThrow(git_object_lookup_prefix(&object, data(), oid.constData(), oid.length(), GIT_OBJ_ANY));
    return QGitObject(object);
}

Reference* QGitRepository::createRef(const QString& name, const LibQGit2::QGitOId& oid, bool overwrite)
{
    git_reference *ref = 0;
    qGitThrow(git_reference_create(&ref, data(), QFile::encodeName(name), oid.constData(), overwrite));
    Reference* qr = new Reference(ref);
    return qr;
}

Reference* QGitRepository::createSymbolicRef(const QString& name, const QString& target, bool overwrite)
{
    git_reference *ref = 0;
    qGitThrow(git_reference_symbolic_create(&ref, data(), QFile::encodeName(name), QFile::encodeName(target), overwrite));
    Reference* qr = new Reference(ref);
    return qr;
}

QGitOId QGitRepository::createCommit(const QString& ref,
                                     const QGitSignature& author,
                                     const QGitSignature& committer,
                                     const QString& message,
                                     const QGitTree& tree,
                                     const QList<Commit>& parents)
{
    QVector<const git_commit*> p;
    foreach (const Commit& parent, parents) {
        p.append(parent.data());
    }

    QGitOId oid;
    qGitThrow(git_commit_create(oid.data(), data(), QFile::encodeName(ref), author.data(), committer.data(),
                                NULL, message.toUtf8(), tree.data(), p.size(), p.data()));
    return oid;
}

QGitOId QGitRepository::createTag(const QString& name,
                                  const QGitObject& target,
                                  bool overwrite)
{
    QGitOId oid;
    qGitThrow(git_tag_create_lightweight(oid.data(), data(), QFile::encodeName(name),
                                         target.data(), overwrite));
    return oid;
}

QGitOId QGitRepository::createTag(const QString& name,
                                  const QGitObject& target,
                                  const QGitSignature& tagger,
                                  const QString& message,
                                  bool overwrite)
{
    QGitOId oid;
    qGitThrow(git_tag_create(oid.data(), data(), QFile::encodeName(name), target.data(),
                             tagger.data(), message.toUtf8(), overwrite));
    return oid;
}

void QGitRepository::deleteTag(const QString& name)
{
    qGitThrow(git_tag_delete(data(), QFile::encodeName(name)));
}

QGitOId QGitRepository::createBlobFromFile(const QString& path)
{
    QGitOId oid;
    qGitThrow(git_blob_create_fromdisk(oid.data(), data(), QFile::encodeName(path)));
    return oid;
}

QGitOId QGitRepository::createBlobFromBuffer(const QByteArray& buffer)
{
    QGitOId oid;
    qGitThrow(git_blob_create_frombuffer(oid.data(), data(), buffer.data(), buffer.size()));
    return oid;
}

QStringList QGitRepository::listTags(const QString& pattern) const
{
    QStringList list;
    git_strarray tags;
    qGitThrow(git_tag_list_match(&tags, qPrintable(pattern), data()));
    for (size_t i = 0; i < tags.count; ++i)
    {
        list << QString(tags.strings[i]);
    }
    git_strarray_free(&tags);
    return list;
}

QStringList QGitRepository::listReferences() const
{
    QStringList list;
    git_strarray refs;
    qGitThrow(git_reference_list(&refs, data()));
    for (size_t i = 0; i < refs.count; ++i)
    {
        list << QString(refs.strings[i]);
    }
    git_strarray_free(&refs);
    return list;
}

QGitDatabase QGitRepository::database() const
{
    git_odb *odb;
    qGitThrow( git_repository_odb(&odb, data()) );
    return QGitDatabase(odb);
}

QGitIndex QGitRepository::index() const
{
    git_index *idx;
    qGitThrow(git_repository_index(&idx, data()));
    return QGitIndex(idx);
}

QGitStatusList QGitRepository::status(const QGitStatusOptions *options) const
{
    const git_status_options opt = options->constData();
    git_status_list *status_list;
    qGitThrow(git_status_list_new(&status_list, data(), &opt));
    return QGitStatusList(status_list);
}

git_repository* QGitRepository::data() const
{
    return d.data();
}

const git_repository* QGitRepository::constData() const
{
    return d.data();
}

} // namespace LibQGit2
