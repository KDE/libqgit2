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

#ifndef LIBQGIT2_REBASE_H
#define LIBQGIT2_REBASE_H

#include "qgitsignature.h"

namespace LibQGit2
{

class OId;
class RebaseOptions;

/**
 * @brief Wrapper class for git_rebase.
 * Represents a Git rebase.
 *
 * @ingroup LibQGit2
 * @{
 */
class LIBQGIT2_EXPORT Rebase
{
public:
    /**
     * Create a wrapper for a libgit2 rebase object. Constructed object
     * takes ownership of \a rebase and frees it in the destructor.
     * @param rebase Raw pointer to the libgit2 rebase object
     * @param opts Specifies how the rebase is to be conducted
     */
    explicit Rebase(git_rebase *rebase, const RebaseOptions &opts);

    /**
     * Aborts a rebase that is currently in progress, resetting the repository
     * and working directory to their state before rebase began.
     *
     * @param signature The identity that is aborting the rebase
     * @throws LibQGit2::Exception
     */
    void abort(const Signature &signature);

    /**
     * Finishes a rebase that is currently in progress once all patches have
     * been applied.
     *
     * @param signature The identity that is finishing the rebase
     * @throws LibQGit2::Exception
     */
    void finish(const Signature &signature = Signature());

    /**
     * Performs the next rebase operation by applying the patch and updating the index
     * and working directory with the changes.  If there are conflicts, those will need
     * to be addressed before committing the changes.
     *
     * @throws LibQGit2::Exception
     */
    bool next();

    /**
     * Commits the current patch.  You must have resolved any conflicts that
     * were introduced during the patch application from the `LibQGit2::Rebase::next`
     * invocation.
     *
     * @param committer The committer of the rebase
     * @param author The author of the updated commit, or \a null to keep the
     *        author from the original commit
     * @param message The message for this commit, or \a null to use the message
     *        from the original commit.
     * @return OID of the newly created commit
     * @throws LibQGit2::Exception
     */
    OId commit(const Signature &committer, const Signature &author = Signature(), const QString &message = QString());

    git_rebase *data() const;
    git_rebase *constData() const;

private:
    struct Private;
    QSharedPointer<Private> d_ptr;
};
/** @} */
}
#endif // LIBQGIT2_REBASE_H
