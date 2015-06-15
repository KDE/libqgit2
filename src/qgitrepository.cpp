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
#include <QtCore/QFileInfo>
#include <QtCore/QVector>

#include "qgitrepository.h"
#include "qgitconfig.h"
#include "qgittag.h"
#include "qgitblob.h"
#include "qgitsignature.h"
#include "qgitexception.h"
#include "qgitstatus.h"
#include "qgitremote.h"
#include "qgitcredentials.h"
#include "qgitdiff.h"
#include "private/annotatedcommit.h"
#include "private/buffer.h"
#include "private/pathcodec.h"
#include "private/remotecallbacks.h"
#include "private/strarray.h"

namespace {
    void do_not_free(git_repository*) {}
}

namespace LibQGit2
{


class Repository::Private : public internal::RemoteListener
{
public:
    typedef QSharedPointer<git_repository> ptr_type;
    ptr_type d;
    QMap<QString, Credentials> m_remote_credentials;
    Repository &m_owner;

    Private(git_repository *repository, bool own, Repository &owner) :
        d(repository, own ? git_repository_free : do_not_free),
        m_owner(owner)
    {
    }

    Private(const Private &other, Repository &owner) :
        d(other.d),
        m_remote_credentials(other.m_remote_credentials),
        m_owner(owner)
    {
    }

    void init(const QString& path, bool isBare)
    {
        d.clear();
        git_repository *repo = 0;
        qGitThrow(git_repository_init(&repo, PathCodec::toLibGit2(path), isBare));
        setData(repo);
    }

    void open(const QString& path)
    {
        d.clear();
        git_repository *repo = 0;
        qGitThrow(git_repository_open(&repo, PathCodec::toLibGit2(path)));
        setData(repo);
    }

    void setData(git_repository *repo)
    {
        d = ptr_type(repo, git_repository_free);
    }

    git_repository* safeData(const char *funcName) const {
        if (d.isNull()){
            throw Exception("Repository::" + QString(funcName) + "(): no repository available");
        }
        return d.data();
    }

    int progress(int transferProgress)
    {
        emit m_owner.cloneProgress(transferProgress);
        return 0;
    }
};


#define SAFE_DATA d_ptr->safeData(LIBQGIT2_FUNC_NAME)

#define THROW(msg) throw Exception(QString("Repository::") + LIBQGIT2_FUNC_NAME + "(): " + msg)

#define AVOID(statement, msg) if (statement) {\
    THROW(msg);\
}


Repository::Repository(git_repository *repository, bool own)
    : d_ptr(new Private(repository, own, *this))
{
}

Repository::Repository(const Repository& other)
    : d_ptr(new Private(*other.d_ptr, *this))
{
}

Repository::~Repository()
{
}

QString Repository::discover(const QString& startPath, bool acrossFs, const QStringList& ceilingDirs)
{
    internal::Buffer repoPath;
    QByteArray joinedCeilingDirs = PathCodec::toLibGit2(ceilingDirs.join(QChar(GIT_PATH_LIST_SEPARATOR)));
    qGitThrow(git_repository_discover(repoPath.data(), PathCodec::toLibGit2(startPath), acrossFs, joinedCeilingDirs));

    return repoPath.asPath();
}

void Repository::init(const QString& path, bool isBare)
{
    d_ptr->init(path, isBare);
}

void Repository::open(const QString& path)
{
    d_ptr->open(path);
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
    qGitThrow(git_repository_head(&ref, SAFE_DATA));
    return Reference(ref);
}

bool Repository::isHeadDetached() const
{
    return qGitThrow(git_repository_head_detached(SAFE_DATA)) == 1;
}

bool Repository::isHeadUnborn() const
{
    return qGitThrow(git_repository_head_unborn(SAFE_DATA)) == 1;
}

bool Repository::isEmpty() const
{
    return qGitThrow(git_repository_is_empty(SAFE_DATA)) == 1;
}

bool Repository::isBare() const
{
    return qGitThrow(git_repository_is_bare(SAFE_DATA)) == 1;
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
    return PathCodec::fromLibGit2(git_repository_path(SAFE_DATA));
}

QString Repository::workDirPath() const
{
    return PathCodec::fromLibGit2(git_repository_workdir(SAFE_DATA));
}

Config Repository::configuration() const
{
    git_config *cfg;
    qGitThrow(git_repository_config(&cfg, SAFE_DATA));
    return Config(cfg);
}

Reference Repository::lookupRef(const QString& name) const
{
    git_reference *ref = 0;
    qGitThrow(git_reference_lookup(&ref, SAFE_DATA, PathCodec::toLibGit2(name)));
    return Reference(ref);
}

OId Repository::lookupRefOId(const QString& name) const
{
    git_oid oid;
    qGitThrow(git_reference_name_to_id(&oid, SAFE_DATA, PathCodec::toLibGit2(name)));
    return OId(&oid);
}

Reference Repository::lookupShorthandRef(const QString& shorthand) const
{
    git_reference *ref = 0;
    qGitThrow(git_reference_dwim(&ref, SAFE_DATA, PathCodec::toLibGit2(shorthand)));
    return Reference(ref);
}

Commit Repository::lookupCommit(const OId& oid) const
{
    git_commit *commit = 0;
    qGitThrow(git_commit_lookup_prefix(&commit, SAFE_DATA, oid.constData(), oid.length()));
    return Commit(commit);
}

Tag Repository::lookupTag(const OId& oid) const
{
    git_tag *tag = 0;
    qGitThrow(git_tag_lookup_prefix(&tag, SAFE_DATA, oid.constData(), oid.length()));
    return Tag(tag);
}

Tree Repository::lookupTree(const OId& oid) const
{
    git_tree *tree = 0;
    qGitThrow(git_tree_lookup_prefix(&tree, SAFE_DATA, oid.constData(), oid.length()));
    return Tree(tree);
}

Blob Repository::lookupBlob(const OId& oid) const
{
    git_blob *blob = 0;
    qGitThrow(git_blob_lookup_prefix(&blob, SAFE_DATA, oid.constData(), oid.length()));
    return Blob(blob);
}

Object Repository::lookupAny(const OId &oid) const
{
    git_object *object = 0;
    qGitThrow(git_object_lookup_prefix(&object, SAFE_DATA, oid.constData(), oid.length(), GIT_OBJ_ANY));
    return Object(object);
}

Object Repository::lookupRevision(const QString &revspec) const
{
    git_object *object = 0;
    qGitThrow(git_revparse_single(&object, SAFE_DATA, revspec.toLatin1()));
    return Object(object);
}

Reference Repository::createRef(const QString& name, const LibQGit2::OId& oid, bool overwrite, const Signature &signature, const QString &message)
{
    git_reference *ref = 0;
    qGitThrow(git_reference_create(&ref, SAFE_DATA, PathCodec::toLibGit2(name), oid.constData(), overwrite, signature.data(), message.toUtf8()));
    return Reference(ref);
}

Reference Repository::createSymbolicRef(const QString& name, const QString& target, bool overwrite, const Signature &signature, const QString &message)
{
    git_reference *ref = 0;
    qGitThrow(git_reference_symbolic_create(&ref, SAFE_DATA, PathCodec::toLibGit2(name), PathCodec::toLibGit2(target), overwrite, signature.data(), message.toUtf8()));
    return Reference(ref);
}

OId Repository::createCommit(const Tree& tree, const QList<Commit>& parents, const Signature& author, const Signature& committer, const QString& message, const QString &ref)
{
    QVector<const git_commit*> p;
    foreach (const Commit& parent, parents) {
        p.append(parent.data());
    }

    OId oid;
    qGitThrow(git_commit_create(oid.data(), SAFE_DATA, ref.isEmpty() ? NULL : PathCodec::toLibGit2(ref).constData(), author.data(), committer.data(),
                                NULL, message.toUtf8(), tree.data(), p.size(), p.data()));
    return oid;
}

OId Repository::createTag(const QString& name,
                                  const Object& target,
                                  bool overwrite)
{
    OId oid;
    qGitThrow(git_tag_create_lightweight(oid.data(), SAFE_DATA, PathCodec::toLibGit2(name),
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
    qGitThrow(git_tag_create(oid.data(), SAFE_DATA, PathCodec::toLibGit2(name), target.data(),
                             tagger.data(), message.toUtf8(), overwrite));
    return oid;
}

void Repository::deleteTag(const QString& name)
{
    qGitThrow(git_tag_delete(SAFE_DATA, PathCodec::toLibGit2(name)));
}

OId Repository::createBlobFromFile(const QString& path)
{
    OId oid;
    qGitThrow(git_blob_create_fromdisk(oid.data(), SAFE_DATA, PathCodec::toLibGit2(path)));
    return oid;
}

OId Repository::createBlobFromBuffer(const QByteArray& buffer)
{
    OId oid;
    qGitThrow(git_blob_create_frombuffer(oid.data(), SAFE_DATA, buffer.data(), buffer.size()));
    return oid;
}

Reference Repository::createBranch(const QString &branchName, const Commit &target, bool force, const Signature &signature, const QString &message)
{
    Commit usedTarget(target);
    if (target.isNull()) {
        usedTarget = lookupCommit(head().target());
    }

    git_reference *ref = NULL;
    qGitThrow(git_branch_create(&ref, SAFE_DATA, branchName.toUtf8(), usedTarget.data(), force, signature.data(), message.isNull() ? NULL : message.toUtf8().constData()));
    return Reference(ref);
}

void Repository::deleteBranch(const QString &branchName)
{
    Reference branch = lookupShorthandRef(branchName);
    qGitThrow(git_branch_delete(branch.data()));
}

void Repository::cherryPick(const Commit &commit, const CherryPickOptions &opts)
{
    AVOID(commit.isNull(), "can not cherry-pick a null commit.")

    qGitThrow(git_cherrypick(SAFE_DATA, commit.data(), opts.data()));
}

QStringList Repository::listTags(const QString& pattern) const
{
    git_strarray tags;
    qGitThrow(git_tag_list_match(&tags, qPrintable(pattern), SAFE_DATA));
    QStringList list;
    for (size_t i = 0; i < tags.count; ++i)
    {
        list << QString(tags.strings[i]);
    }
    git_strarray_free(&tags);
    return list;
}

QStringList Repository::listReferences() const
{
    git_strarray refs;
    qGitThrow(git_reference_list(&refs, SAFE_DATA));
    QStringList list;
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
    qGitThrow( git_repository_odb(&odb, SAFE_DATA) );
    return Database(odb);
}

Index Repository::index() const
{
    git_index *idx;
    qGitThrow(git_repository_index(&idx, SAFE_DATA));
    return Index(idx);
}

StatusList Repository::status(const StatusOptions &options) const
{
    const git_status_options opt = options.constData();
    git_status_list *status_list;
    qGitThrow(git_status_list_new(&status_list, SAFE_DATA, &opt));
    return StatusList(status_list);
}

Repository::GraphRelationship Repository::commitRelationship(const Commit &local, const Commit &upstream) const
{
    GraphRelationship result;
    qGitThrow(git_graph_ahead_behind(&result.ahead, &result.behind, SAFE_DATA, local.oid().constData(), upstream.oid().constData()));
    return result;
}

Diff Repository::diffTrees(const Tree &oldTree, const Tree &newTree) const
{
    git_diff *diff = NULL;
    qGitThrow(git_diff_tree_to_tree(&diff, SAFE_DATA, oldTree.data(), newTree.data(), NULL));
    return Diff(diff);
}

Commit Repository::mergeBase(const Commit &one, const Commit &two) const
{
    OId out;
    qGitThrow(git_merge_base(out.data(), SAFE_DATA, one.oid().constData(), two.oid().constData()));
    return lookupCommit(out);
}

Index Repository::mergeTrees(const Tree &our, const Tree &their, const Tree &ancestor, const MergeOptions &opts)
{
    AVOID(our.isNull() && their.isNull(), "needed at least either 'our' or 'their' tree to merge.")

    git_index *index = NULL;
    qGitThrow(git_merge_trees(&index, SAFE_DATA, ancestor.data(), our.data(), their.data(), opts.data()));
    return Index(index);
}

git_repository* Repository::data() const
{
    return d_ptr->d.data();
}

const git_repository* Repository::constData() const
{
    return d_ptr->d.data();
}


void Repository::setRemoteCredentials(const QString& remoteName, Credentials credentials)
{
    d_ptr->m_remote_credentials[remoteName] = credentials;
}


void Repository::clone(const QString& url, const QString& path, const Signature &signature)
{
    const QString remoteName("origin");
    internal::RemoteCallbacks remoteCallbacks(d_ptr.data(), d_ptr->m_remote_credentials.value(remoteName));

    git_repository *repo = 0;
    git_clone_options opts = GIT_CLONE_OPTIONS_INIT;
    opts.remote_callbacks = remoteCallbacks.rawCallbacks();
    opts.checkout_opts.checkout_strategy = GIT_CHECKOUT_SAFE_CREATE;
    opts.signature = const_cast<git_signature*>(signature.data());
    qGitEnsureValue(0, git_clone(&repo, url.toLatin1(), PathCodec::toLibGit2(path), &opts));

    d_ptr->setData(repo);
}


void Repository::remoteAdd(const QString& name, const QString& url, bool changeUrlIfExists)
{
    git_remote *r = NULL;
    switch (git_remote_lookup(&r, SAFE_DATA, name.toLatin1())) {
    case GIT_ENOTFOUND:
        r = NULL;
        qGitThrow(git_remote_create(&r, SAFE_DATA, name.toLatin1(), url.toLatin1()));
        break;

    case GIT_OK:
        if (QString::fromLatin1(git_remote_url(r)) != url) {
            if (changeUrlIfExists) {
                qGitThrow(git_remote_set_url(r, url.toLatin1()));
                qGitThrow(git_remote_save(r));
            } else {
                THROW("remote already exists");
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
    git_remote *r = NULL;
    qGitThrow(git_remote_lookup(&r, SAFE_DATA, remoteName.toLatin1()));
    return new Remote(r, d_ptr->m_remote_credentials.value(remoteName), parent);
}


void Repository::fetch(const QString& name, const QString& head, const Signature &signature, const QString &message)
{
    git_remote *_remote = NULL;
    qGitThrow(git_remote_lookup(&_remote, SAFE_DATA, name.toLatin1()));
    Remote remote(_remote, d_ptr->m_remote_credentials.value(name));
    connect(&remote, SIGNAL(transferProgress(int)), this, SIGNAL(fetchProgress(int)));

    internal::StrArray refs;
    if (!head.isEmpty()) {
        const QString refspec = QString("refs/heads/%2:refs/remotes/%1/%2").arg(name).arg(head);
        refs.set(QList<QByteArray>() << refspec.toLatin1());
    }

    qGitThrow(git_remote_fetch(remote.data(), refs.count() > 0 ? &refs.data() : NULL, signature.data(), message.isNull() ? NULL : message.toUtf8().constData()));
}


QStringList Repository::remoteBranches(const QString& remoteName)
{
    git_remote *_remote = NULL;
    qGitThrow(git_remote_lookup(&_remote, SAFE_DATA, remoteName.toLatin1()));
    Remote remote(_remote, d_ptr->m_remote_credentials.value(remoteName));

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


void Repository::checkoutTree(const Object &treeish, const CheckoutOptions &opts)
{
    qGitThrow(git_checkout_tree(SAFE_DATA, treeish.constData(), opts.data()));
}


void Repository::checkoutHead(const CheckoutOptions &opts)
{
    qGitThrow(git_checkout_head(SAFE_DATA, opts.data()));
}


void Repository::checkoutRemote(const QString& branch, const CheckoutOptions &opts, const QString& remote, const Signature &signature, const QString &message)
{
    const QString refspec = "refs/remotes/" + remote + "/" + branch;
    checkoutTree(lookupRevision(refspec), opts);

    qGitThrow(git_repository_set_head(SAFE_DATA, refspec.toLatin1(), signature.data(), message.toUtf8()));
}


void Repository::reset(const Object &target, ResetType type, const Signature &signature, const QString &message)
{
    AVOID(target.isNull(), "can not reset to null target");

    git_reset_t resetType;
    switch (type) {
    case Soft:
        resetType = GIT_RESET_SOFT;
        break;
    case Mixed:
        resetType = GIT_RESET_MIXED;
        break;
    case Hard:
        resetType = GIT_RESET_HARD;
        break;
    default:
        THROW("invalid reset type argument");
    }

    qGitThrow(git_reset(SAFE_DATA, target.data(), resetType, NULL, const_cast<git_signature*>(signature.data()), message.isNull() ? NULL : message.toUtf8().constData()));
}

Rebase Repository::rebase(const Reference &branch, const Reference &upstream, const Reference &onto, const RebaseOptions &opts, const Signature &signature)
{
    git_rebase *rebase;
    internal::AnnotatedCommit commitBranch(*this, branch);
    internal::AnnotatedCommit commitUpstream(*this, upstream);
    internal::AnnotatedCommit commitOnto(*this, onto);
    qGitThrow(git_rebase_init(&rebase, data(), commitBranch.constData(), commitUpstream.constData(), commitOnto.constData(), signature.data(), opts.constData()));
    return Rebase(rebase, opts);
}

bool Repository::shouldIgnore(const QString &path) const
{
    QString usedPath(QDir::cleanPath(path));

    QFileInfo pathInfo(usedPath);
    if (pathInfo.isAbsolute()) {
        QString wd(workDirPath());
        if (usedPath.startsWith(wd)) {
            usedPath = usedPath.mid(wd.size());
        } else {
            THROW("Given path (" + path + ") is not within this repository's directory (" + wd + ").");
        }
    }

    int result;
    qGitThrow(git_status_should_ignore(&result, SAFE_DATA, PathCodec::toLibGit2(usedPath)));
    return result;
}

} // namespace LibQGit2
