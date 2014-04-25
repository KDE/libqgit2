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

#include "qgitrepository.h"
#include "qgitcommit.h"
#include "qgitconfig.h"
#include "qgittag.h"
#include "qgittree.h"
#include "qgitblob.h"
#include "qgitsignature.h"
#include "qgitexception.h"
#include "qgitstatus.h"
#include "qgitremote.h"
#include "qgitcredentials.h"
#include "qgitpush.h"

namespace {
    void do_not_free(git_repository*) {}
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

Reference Repository::lookupRef(const QString& name) const
{
    git_reference *ref = 0;
    qGitThrow(git_reference_lookup(&ref, d.data(), QFile::encodeName(name)));
    return Reference(ref);
}

OId Repository::lookupRefOId(const QString& name) const
{
    git_oid oid;
    qGitThrow(git_reference_name_to_id(&oid, d.data(), QFile::encodeName(name)));
    return OId(&oid);
}

Reference Repository::lookupShorthandRef(const QString& shorthand) const
{
    git_reference *ref = 0;
    qGitThrow(git_reference_dwim(&ref, d.data(), QFile::encodeName(shorthand)));
    return Reference(ref);
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

Object Repository::lookupRevision(const QString &revspec) const
{
    git_object *object = 0;
    qGitThrow(git_revparse_single(&object, d.data(), revspec.toLatin1()));
    return Object(object);
}

Reference Repository::createRef(const QString& name, const LibQGit2::OId& oid, bool overwrite)
{
    git_reference *ref = 0;
    qGitThrow(git_reference_create(&ref, d.data(), QFile::encodeName(name), oid.constData(), overwrite));
    return Reference(ref);
}

Reference Repository::createSymbolicRef(const QString& name, const QString& target, bool overwrite)
{
    git_reference *ref = 0;
    qGitThrow(git_reference_symbolic_create(&ref, d.data(), QFile::encodeName(name), QFile::encodeName(target), overwrite));
    return Reference(ref);
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

StatusList Repository::status(const StatusOptions &options) const
{
    const git_status_options opt = options.constData();
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


void Repository::setRemoteCredentials(const QString& remoteName, Credentials credentials)
{
    m_remote_credentials[remoteName] = credentials;
}


void Repository::clone(const QString& url, const QString& path)
{
    init(path);

    const QString remoteName("origin");
    remoteAdd(remoteName, url);

    git_remote *_remote = NULL;
    qGitThrow(git_remote_load(&_remote, data(), remoteName.toLatin1()));
    Remote remote(_remote, m_remote_credentials.value(remoteName));
    connect(&remote, SIGNAL(transferProgress(int)), SIGNAL(cloneProgress(int)));

    git_checkout_opts checkoutOpts = GIT_CHECKOUT_OPTS_INIT;
    checkoutOpts.checkout_strategy = GIT_CHECKOUT_SAFE_CREATE;
    qGitEnsureValue(0, git_clone_into(d.data(), remote.data(), &checkoutOpts, NULL));
}


void Repository::remoteAdd(const QString& name, const QString& url, bool changeUrlIfExists)
{
    if (d.isNull()){
        throw Exception("Repository::remoteAdd(): no repository available");
    }

    git_remote *r = NULL;
    switch (git_remote_load(&r, data(), name.toLatin1())) {
    case GIT_ENOTFOUND:
        r = NULL;
        qGitThrow(git_remote_create(&r, data(), name.toLatin1(), url.toLatin1()));
        break;

    case GIT_OK:
        if (QString::fromLatin1(git_remote_url(r)) != url) {
            if (changeUrlIfExists) {
                qGitThrow(git_remote_set_url(r, url.toLatin1()));
                qGitThrow(git_remote_save(r));
            } else {
                throw Exception("Repository::remoteAdd() remote already exists");
            }
        }
        break;

    default:
        throw Exception();
        break;
    }
}


Remote* Repository::remote(const QString &remoteName, QObject *parent) const
{
    if (d.isNull()){
        throw Exception("Repository::remote(): no repository available");
    }

    git_remote *r = NULL;
    qGitThrow(git_remote_load(&r, data(), remoteName.toLatin1()));
    return new Remote(r, m_remote_credentials.value(remoteName), parent);
}


void Repository::fetch(const QString& name, const QString& head)
{
    if (d.isNull()){
        throw Exception("Repository::fetch(): no repository available");
    }

    git_remote *_remote = NULL;
    qGitThrow(git_remote_load(&_remote, data(), name.toLatin1()));
    Remote remote(_remote, m_remote_credentials.value(name));

    const QString usedhead = head.isEmpty() ? "*" : head;
    const QString refspec = QString("refs/heads/%2:refs/remotes/%1/%2").arg(name).arg(usedhead);

    git_strarray refs;
    const QByteArray hbytes = refspec.toLatin1();
    const char* strings[1];
    strings[0] = hbytes.constData();
    refs.count = 1;
    refs.strings = const_cast<char**>(strings);
    qGitThrow(git_remote_set_fetch_refspecs(remote.data(), &refs));

    qGitThrow(git_remote_connect(remote.data(), GIT_DIRECTION_FETCH));
    qGitEnsureValue(1, git_remote_connected(remote.data()));
    qGitThrow(git_remote_download(remote.data()));
    qGitThrow(git_remote_update_tips(remote.data()));
}


QStringList Repository::remoteBranches(const QString& remoteName)
{
    if (d.isNull()){
        throw Exception("Repository::remoteBranches(): no repository available");
    }

    git_remote *_remote = NULL;
    qGitThrow(git_remote_load(&_remote, data(), remoteName.toLatin1()));
    Remote remote(_remote, m_remote_credentials.value(remoteName));

    qGitThrow(git_remote_connect(remote.data(), GIT_DIRECTION_FETCH));
    qGitEnsureValue(1, git_remote_connected(remote.data()));

    /* List the heads on the remote */
    const git_remote_head** remote_heads = NULL;
    size_t count = 0;
    qGitThrow(git_remote_ls(&remote_heads, &count, remote.data()));
    QStringList heads;
    for (size_t i = 0; i < count; ++i) {
        const git_remote_head* head = remote_heads[i];
        if (head && head->name) {
            QString ref = QString::fromLatin1(head->name);
            if (ref.startsWith("refs/heads/")) {
                heads << ref.replace("refs/heads/", "");
            }
        }
    }

    return heads;
}


void Repository::checkoutTree(const Object &treeish, bool force)
{
    git_checkout_opts opts = GIT_CHECKOUT_OPTS_INIT;
    opts.checkout_strategy = force ? GIT_CHECKOUT_FORCE : GIT_CHECKOUT_SAFE;
    qGitThrow(git_checkout_tree(d.data(), treeish.constData(), &opts));
}


void Repository::checkoutRemote(const QString& branch, bool force, const QString& remote)
{
    if (d.isNull()){
        throw Exception("Repository::checkoutRemote(): no repository available");
    }

    const QString refspec = "refs/remotes/" + remote + "/" + branch;
    checkoutTree(lookupRevision(refspec), force);

    qGitThrow(git_repository_set_head(data(), refspec.toLatin1()));
}


Push Repository::push(const QString &remoteName)
{
    if (d.isNull()) {
        throw Exception("Repository::push(): no repository available");
    }

    return Push(*remote(remoteName));
}


} // namespace LibQGit2
