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

#include <qgitrepository.h>
#include <qgitcommit.h>
#include <qgitconfig.h>
#include <qgittag.h>
#include <qgittree.h>
#include <qgitblob.h>
#include <qgitsignature.h>
#include <qgitexception.h>
#include <qgitsubmodule.h>

#include <git2/errors.h>
#include <git2/repository.h>
#include <git2/refs.h>
#include <git2/commit.h>
#include <git2/tag.h>
#include <git2/tree.h>
#include <git2/blob.h>

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QVector>
#include <QtCore/QStringList>

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

QGitRef QGitRepository::head() const
{
    git_reference *ref = 0;
    qGitThrow(git_repository_head(&ref, data()));
    return QGitRef(ref);
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

QGitConfig QGitRepository::configuration() const
{
    git_config *cfg;
    qGitThrow( git_repository_config(&cfg, data()) );
    return QGitConfig(cfg);
}

QGitRef QGitRepository::lookupRef(const QString& name) const
{
    git_reference *ref = 0;
    qGitThrow(git_reference_lookup(&ref, data(), QFile::encodeName(name)));
    return QGitRef(ref);
}

QGitCommit QGitRepository::lookupCommit(const QGitOId& oid) const
{
    git_commit *commit = 0;
    qGitThrow(git_commit_lookup_prefix(&commit, data(), oid.constData(), oid.length()));
    return QGitCommit(commit);
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

QGitBlob QGitRepository::lookupBlob(const QGitOId& oid) const
{
    git_blob *blob = 0;
    qGitThrow(git_blob_lookup_prefix(&blob, data(), oid.constData(), oid.length()));
    return QGitBlob(blob);
}

QGitObject QGitRepository::lookupAny(const QGitOId &oid) const
{
    git_object *object = 0;
    qGitThrow(git_object_lookup_prefix(&object, data(), oid.constData(), oid.length(), GIT_OBJ_ANY));
    return QGitObject(object);
}

QGitRef QGitRepository::createRef(const QString& name, const QGitOId& oid, bool overwrite)
{
    git_reference *ref = 0;
    qGitThrow(git_reference_create_oid(&ref, data(), QFile::encodeName(name), oid.constData(), overwrite));
    return QGitRef(ref);
}

QGitRef QGitRepository::createSymbolicRef(const QString& name, const QString& target, bool overwrite)
{
    git_reference *ref = 0;
    qGitThrow(git_reference_create_symbolic(&ref, data(), QFile::encodeName(name), QFile::encodeName(target), overwrite));
    return QGitRef(ref);
}

QGitOId QGitRepository::createCommit(const QString& ref,
                                     const QGitSignature& author,
                                     const QGitSignature& committer,
                                     const QString& message,
                                     const QGitTree& tree,
                                     const QList<QGitCommit>& parents)
{
    QVector<const git_commit*> p;
    foreach (const QGitCommit& parent, parents) {
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
    qGitThrow(git_blob_create_fromfile(oid.data(), data(), QFile::encodeName(path)));
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
    qGitThrow(git_reference_listall( &refs, data(), GIT_REF_LISTALL));
    for (size_t i = 0; i < refs.count; ++i)
    {
        list << QString(refs.strings[i]);
    }
    git_strarray_free(&refs);
    return list;
}

QGitSubmoduleList QGitRepository::listSubmodules() const
{
    QGitPrivateSubmoduleLookupInfo submoduleInfo( data() );
    git_submodule_foreach(data(), &addToSubmoduleList, &submoduleInfo);
    return submoduleInfo.submodules;
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

git_repository* QGitRepository::data() const
{
    return d.data();
}

const git_repository* QGitRepository::constData() const
{
    return d.data();
}

int QGitRepository::addToSubmoduleList(const char *name, void *payload)
{
    Q_ASSERT(payload != 0);

    QGitPrivateSubmoduleLookupInfo * submoduleInfo = static_cast<QGitPrivateSubmoduleLookupInfo *>(payload);

    git_submodule *submodule;
    int err = git_submodule_lookup(&submodule, submoduleInfo->repo, name);
    if ( err == GIT_SUCCESS)
        submoduleInfo->submodules.append(QGitSubmodule(submodule));

    return err;
}

} // namespace LibQGit2
