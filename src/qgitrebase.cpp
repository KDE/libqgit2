/******************************************************************************
 * This file is part of the libqgit2 library
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

#include "qgitrebase.h"
#include "qgitrebaseoptions.h"
#include "qgitcheckoutoptions.h"
#include "qgitoid.h"
#include "qgitexception.h"

namespace LibQGit2
{

struct Rebase::Private {
    Private(git_rebase *rebase, const RebaseOptions &opts)
        : mRebase(rebase, git_rebase_free),
          mOpts(opts)
    {
    }

    void abort(const Signature &signature)
    {
        qGitThrow(git_rebase_abort(data(), signature.data()));
    }

    OId commit(const Signature &committer, const Signature &author, const QString &message)
    {
        git_oid oid;
        qGitThrow(git_rebase_commit(&oid, data(), author.data(), committer.data(), NULL, message.isNull() ? NULL : message.toUtf8().constData()));
        return OId(&oid);
    }

    void finish(const Signature &signature)
    {
        qGitThrow(git_rebase_finish(data(), signature.data(), mOpts.constData()));
    }

    bool next()
    {
        git_rebase_operation *op;
        git_checkout_options *opts = const_cast<git_checkout_options*>(mOpts.checkoutOptions().data());
        int error = git_rebase_next(&op, data(), opts);
        if (error == GIT_ITEROVER) {
            return false;
        } else {
            qGitThrow(error);
            return true;
        }
    }

    git_rebase *data() const
    {
        return mRebase.data();
    }

    const git_rebase *constData() const
    {
        return mRebase.data();
    }

private:
    QSharedPointer<git_rebase> mRebase;
    const RebaseOptions mOpts;
};

Rebase::Rebase(git_rebase *rebase, const RebaseOptions &opts)
    : d_ptr(new Private(rebase, opts))
{
}

void Rebase::abort(const Signature &signature)
{
    return d_ptr->abort(signature);
}

OId Rebase::commit(const Signature &committer, const Signature &author, const QString &message)
{
    return d_ptr->commit(committer, author, message);
}

void Rebase::finish(const Signature &signature)
{
    d_ptr->finish(signature);
}

bool Rebase::next()
{
    return d_ptr->next();
}

git_rebase *Rebase::data() const
{
    return d_ptr->data();
}

git_rebase *Rebase::constData() const
{
    return d_ptr->data();
}
}
