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

#ifndef LIBQGIT2_CHERRYPICKOPTIONS_H
#define LIBQGIT2_CHERRYPICKOPTIONS_H

#include "git2.h"
#include <QSharedPointer>
#include "qgitmergeoptions.h"
#include "qgitcheckoutoptions.h"

namespace LibQGit2
{
    /**
     * Options that specify how a cherry-pick operation is performed.
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_EXPORT CherryPickOptions
    {
    public:
        /**
         * Constructs a new CherryPickOptions.
         * @param mergeOpts The merge options to use in the cherry-pick.
         * @param checkoutOpts The checkout options to use in the cherry-pick.
         */
        CherryPickOptions(const MergeOptions &mergeOpts = MergeOptions(), const CheckoutOptions &checkoutOpts = CheckoutOptions());

        /**
         * Sets the Mainline for the cherry-pick.
         *
         * Usually you cannot cherry-pick a merge because you do not know which side of the merge
         * should be considered the mainline. This parameter specifies the parent number (starting from 1)
         * of the mainline and allows cherry-pick to replay the change relative to the specified parent.
         * @param mainline The parent to use when cherry-picking a merge commit.
         */
        void setMainline(unsigned int mainline);

        const git_cherrypick_options* data() const;

    private:
        class Private;
        QSharedPointer<Private> d_ptr;
        Q_DECLARE_PRIVATE()
    };

    /** @} */

}

#endif // LIBQGIT2_CHERRYPICKOPTIONS_H
