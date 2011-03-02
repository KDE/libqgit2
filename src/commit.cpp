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

#include "commit.h"

#include "repository.h"
#include "signature.h"
#include "tree.h"

using namespace LibQGit2;

Commit::Commit( Repository *repository, QObject* parent)
{
    git_commit_new(&m_commit, repository->data());
}

Commit::Commit( const git_commit *commit, QObject* parent)
    : m_commit(const_cast<git_commit *>(m_commit))
{
}

Commit::Commit( const Commit& other )
{
    m_commit = other.m_commit;
}

Commit::~Commit()
{
}

int Commit::lookup(Repository *repository, const OId& oid)
{
    return git_commit_lookup(&m_commit, repository->data(), oid.constData());
}

const OId* Commit::id() const
{
    const OId *oid = new OId(git_commit_id(m_commit));
    return oid;
}

QString Commit::messageShort() const
{
    return QString::fromUtf8(git_commit_message_short(m_commit));
}

QString Commit::message() const
{
    return QString::fromUtf8(git_commit_message(m_commit));
}

QDateTime Commit::dateTime() const
{
    QDateTime dateTime;
    dateTime.setTime_t(git_commit_time(m_commit));
    return dateTime;
}

int Commit::timeOffset() const
{
    return git_commit_time_offset(m_commit);
}

const Signature* Commit::committer() const
{
    const Signature *signature = new Signature(git_commit_committer(m_commit));
    return signature;
}

const Signature* Commit::author() const
{
    const Signature *signature = new Signature(git_commit_author(m_commit));
    return signature;
}

const Tree* Commit::tree() const
{
    const Tree *tree = new Tree(git_commit_tree(m_commit));
    return tree;
}

unsigned int Commit::parentCount() const
{
    return git_commit_parentcount(m_commit);
}

Commit* Commit::parent(unsigned n) const
{
    Commit *commit = new Commit(git_commit_parent(m_commit, n));
    return commit;
}

int Commit::addParent(Commit* newParent)
{
    return git_commit_add_parent(m_commit, newParent->data());
}

void Commit::setMessage(const QString& message)
{
    return git_commit_set_message(m_commit, message.toAscii().constData());
}

void Commit::setCommitter(const Signature& committerSig)
{
    return git_commit_set_committer(m_commit, committerSig.constData());
}

void Commit::setAuthor(const Signature& authorSig)
{
    return git_commit_set_author(m_commit, authorSig.constData());
}

void Commit::setTree(const Tree& tree)
{
    return git_commit_set_tree(m_commit, tree.data());
}

git_commit* Commit::data() const
{
    return m_commit;
}

const git_commit* Commit::constData() const
{
    return const_cast<const git_commit *>(m_commit);
}

