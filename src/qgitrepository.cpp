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
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301 USA
 */

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QVector>
#include <QtCore/QStringList>
#include <QtCore/QDebug>

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

    struct RemoteRAII {
        git_remote& remote;
        RemoteRAII(git_remote* r) : remote(*r) {}
        ~RemoteRAII() { git_remote_free(&remote); }
    };
}

namespace LibQGit2
{

Repository::Repository(git_repository *repository, bool own)
    : d(repository, own ? git_repository_free : do_not_free)
{
}

Repository::Repository( const Repository& other )
    : d(other.d)
{
}

Repository::~Repository()
{
}

QString Repository::discover(const QString& startPath, bool acrossFs, const QStringList& ceilingDirs)
{
    QByteArray repoPath(GIT_PATH_MAX, Qt::Uninitialized);
    QByteArray joinedCeilingDirs = QFile::encodeName(ceilingDirs.join(QChar(GIT_PATH_LIST_SEPARATOR)));
    qGitThrow(git_repository_discover(repoPath.data(), repoPath.length(),
                                      QFile::encodeName(startPath),
                                      acrossFs, joinedCeilingDirs));
    return QFile::decodeName(repoPath);
}

void Repository::init(const QString& path, bool isBare)
{
    d.clear();
    git_repository *repo = 0;
    qGitThrow(git_repository_init(&repo, QFile::encodeName(path), isBare));
    d = ptr_type(repo, git_repository_free);
}

void Repository::open(const QString& path)
{
    d.clear();
    git_repository *repo = 0;
    qGitThrow(git_repository_open(&repo, QFile::encodeName(path)));
    d = ptr_type(repo, git_repository_free);
}

void Repository::discoverAndOpen(const QString &startPath,
                                     bool acrossFs,
                                     const QStringList &ceilingDirs)
{
    open(discover(startPath, acrossFs, ceilingDirs));
}

Reference Repository::head() const
{
    git_reference *ref = 0;
    qGitThrow(git_repository_head(&ref, d.data()));
    return Reference(ref);
}

bool Repository::isHeadDetached() const
{
    return qGitThrow(git_repository_head_detached(d.data())) == 1;
}

bool Repository::isHeadUnborn() const
{
    return qGitThrow(git_repository_head_unborn(d.data())) == 1;
}

bool Repository::isEmpty() const
{
    return qGitThrow(git_repository_is_empty(d.data())) == 1;
}

bool Repository::isBare() const
{
    return qGitThrow(git_repository_is_bare(d.data())) == 1;
}

QString Repository::name() const
{
    QString repoPath = QDir::cleanPath( workDirPath() );
    if (isBare())
        repoPath = QDir::cleanPath( path() );

    return QFileInfo(repoPath).fileName();
}

QString Repository::path() const
{
    return QFile::decodeName(git_repository_path(d.data()));
}

QString Repository::workDirPath() const
{
    return QFile::decodeName(git_repository_workdir(d.data()));
}

Config Repository::configuration() const
{
    git_config *cfg;
    qGitThrow( git_repository_config(&cfg, d.data()) );
    return Config(cfg);
}

Reference* Repository::lookupRef(const QString& name) const
{
    git_reference *ref = 0;
    qGitThrow(git_reference_lookup(&ref, d.data(), QFile::encodeName(name)));
    Reference* qr = new Reference(ref);
    return qr;
}

OId* Repository::lookupRefOId(const QString& name) const
{
    git_oid oid;
    qGitThrow(git_reference_name_to_id(&oid, d.data(), QFile::encodeName(name)));
    OId* qoid = new OId(&oid);
    return qoid;
}

Reference* Repository::lookupShorthandRef(const QString& shorthand) const
{
    git_reference *ref = 0;
    qGitThrow(git_reference_dwim(&ref, d.data(), QFile::encodeName(shorthand)));
    Reference* qr = new Reference(ref);
    return qr;
}

Commit Repository::lookupCommit(const OId& oid) const
{
    git_commit *commit = 0;
    qGitThrow(git_commit_lookup_prefix(&commit, d.data(), oid.constData(), oid.length()));
    return Commit(commit);
}

Tag Repository::lookupTag(const OId& oid) const
{
    git_tag *tag = 0;
    qGitThrow(git_tag_lookup_prefix(&tag, d.data(), oid.constData(), oid.length()));
    return Tag(tag);
}

Tree Repository::lookupTree(const OId& oid) const
{
    git_tree *tree = 0;
    qGitThrow(git_tree_lookup_prefix(&tree, d.data(), oid.constData(), oid.length()));
    return Tree(tree);
}

Blob Repository::lookupBlob(const OId& oid) const
{
    git_blob *blob = 0;
    qGitThrow(git_blob_lookup_prefix(&blob, d.data(), oid.constData(), oid.length()));
    return Blob(blob);
}

Object Repository::lookupAny(const OId &oid) const
{
    git_object *object = 0;
    qGitThrow(git_object_lookup_prefix(&object, d.data(), oid.constData(), oid.length(), GIT_OBJ_ANY));
    return Object(object);
}

Reference* Repository::createRef(const QString& name, const LibQGit2::OId& oid, bool overwrite)
{
    git_reference *ref = 0;
    qGitThrow(git_reference_create(&ref, d.data(), QFile::encodeName(name), oid.constData(), overwrite));
    Reference* qr = new Reference(ref);
    return qr;
}

Reference* Repository::createSymbolicRef(const QString& name, const QString& target, bool overwrite)
{
    git_reference *ref = 0;
    qGitThrow(git_reference_symbolic_create(&ref, d.data(), QFile::encodeName(name), QFile::encodeName(target), overwrite));
    Reference* qr = new Reference(ref);
    return qr;
}

OId Repository::createCommit(const QString& ref,
                                     const Signature& author,
                                     const Signature& committer,
                                     const QString& message,
                                     const Tree& tree,
                                     const QList<Commit>& parents)
{
    QVector<const git_commit*> p;
    foreach (const Commit& parent, parents) {
        p.append(parent.data());
    }

    OId oid;
    qGitThrow(git_commit_create(oid.data(), d.data(), QFile::encodeName(ref), author.data(), committer.data(),
                                NULL, message.toUtf8(), tree.data(), p.size(), p.data()));
    return oid;
}

OId Repository::createTag(const QString& name,
                                  const Object& target,
                                  bool overwrite)
{
    OId oid;
    qGitThrow(git_tag_create_lightweight(oid.data(), d.data(), QFile::encodeName(name),
                                         target.data(), overwrite));
    return oid;
}

OId Repository::createTag(const QString& name,
                                  const Object& target,
                                  const Signature& tagger,
                                  const QString& message,
                                  bool overwrite)
{
    OId oid;
    qGitThrow(git_tag_create(oid.data(), d.data(), QFile::encodeName(name), target.data(),
                             tagger.data(), message.toUtf8(), overwrite));
    return oid;
}

void Repository::deleteTag(const QString& name)
{
    qGitThrow(git_tag_delete(d.data(), QFile::encodeName(name)));
}

OId Repository::createBlobFromFile(const QString& path)
{
    OId oid;
    qGitThrow(git_blob_create_fromdisk(oid.data(), d.data(), QFile::encodeName(path)));
    return oid;
}

OId Repository::createBlobFromBuffer(const QByteArray& buffer)
{
    OId oid;
    qGitThrow(git_blob_create_frombuffer(oid.data(), d.data(), buffer.data(), buffer.size()));
    return oid;
}

QStringList Repository::listTags(const QString& pattern) const
{
    QStringList list;
    git_strarray tags;
    qGitThrow(git_tag_list_match(&tags, qPrintable(pattern), d.data()));
    for (size_t i = 0; i < tags.count; ++i)
    {
        list << QString(tags.strings[i]);
    }
    git_strarray_free(&tags);
    return list;
}

QStringList Repository::listReferences() const
{
    QStringList list;
    git_strarray refs;
    qGitThrow(git_reference_list(&refs, d.data()));
    for (size_t i = 0; i < refs.count; ++i)
    {
        list << QString(refs.strings[i]);
    }
    git_strarray_free(&refs);
    return list;
}

Database Repository::database() const
{
    git_odb *odb;
    qGitThrow( git_repository_odb(&odb, d.data()) );
    return Database(odb);
}

Index Repository::index() const
{
    git_index *idx;
    qGitThrow(git_repository_index(&idx, d.data()));
    return Index(idx);
}

StatusList Repository::status(const StatusOptions *options) const
{
    const git_status_options opt = options->constData();
    git_status_list *status_list;
    qGitThrow(git_status_list_new(&status_list, d.data(), &opt));
    return StatusList(status_list);
}

git_repository* Repository::data() const
{
    return d.data();
}

const git_repository* Repository::constData() const
{
    return d.data();
}


int Repository::fetchProgressCallback(const git_transfer_progress* stats, void* data)
{
    if (!data) {
        return 1;
    }
    Repository* repo = static_cast<Repository*>(data);
    int percent = (int)(0.5 + 100.0 * ((double)stats->received_objects) / ((double)stats->total_objects));
    if (percent != repo->m_clone_progress) {
        emit repo->cloneProgress(percent);
        repo->m_clone_progress = percent;
    }
    return 0;
}


void Repository::clone(const QString& url, const QString& path)
{
    git_clone_options opts;
    memset(&opts, 0, sizeof(git_clone_options));
    opts = GIT_CLONE_OPTIONS_INIT;

    opts.remote_callbacks.transfer_progress = &fetchProgressCallback;
    opts.remote_callbacks.payload = (void*)this;;

    opts.checkout_opts = GIT_CHECKOUT_OPTS_INIT;
    opts.checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE_CREATE;

    m_clone_progress = 0;
    git_repository* repo = NULL;
    qGitThrow(git_clone(&repo, url.toLatin1(), QFile::encodeName(path), &opts));
    d = ptr_type(repo, git_repository_free);
}


void Repository::remoteAdd(const QString& name, const QString& url)
{
    if (d.isNull()){
        throw Exception("Repository::fetch(): no repository available");
    }

    git_remote* remote = 0;
    int ret = git_remote_load(&remote, data(), name.toLatin1());
    if (ret != 0 && ret != GIT_ENOTFOUND) {
        throw Exception();
    }

    RemoteRAII rai(remote); (void)rai;

    if (remote && QString::fromLatin1(git_remote_url(remote)) == url) {
        return;
    }
 
    qGitThrow(git_remote_create(&remote, data(), name.toLatin1(), url.toLatin1()));
}


void Repository::fetch(const QString& name, const QString& head)
{
    if (d.isNull()){
        throw Exception("Repository::fetch(): no repository available");
    }

    git_remote* remote = 0;
    qGitThrow(git_remote_load(&remote, data(), name.toLatin1()));

    const QString usedhead = head.isEmpty() ? "*" : head;
    const QString refspec = QString("refs/heads/%2:refs/remotes/%1/%2").arg(name).arg(usedhead);

    RemoteRAII rai(remote); (void)rai;

    git_strarray refs;
    const QByteArray hbytes = refspec.toLatin1();
    const char* strings[1];
    strings[0] = hbytes.constData();
    refs.count = 1;
    refs.strings = const_cast<char**>(strings);
    qGitThrow(git_remote_set_fetch_refspecs(remote, &refs));

    qGitThrow(git_remote_connect(remote, GIT_DIRECTION_FETCH));
    qGitThrow(git_remote_connected(remote));
    qGitThrow(git_remote_download(remote));
    qGitThrow(git_remote_update_tips(remote));

}


} // namespace LibQGit2
