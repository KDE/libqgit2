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

#ifndef LIBQGIT2_CHECKOUTOPTIONS_H
#define LIBQGIT2_CHECKOUTOPTIONS_H

#include "git2.h"

#include "libqgit2_config.h"

namespace LibQGit2
{
    /**
     * Options that specify how a checkout operation is performed.
     *
     * @ingroup LibQGit2
     * @{
     */
    class LIBQGIT2_EXPORT CheckoutOptions
    {
    public:
        /**
         * A checkout strategy.
         */
        enum Strategy {
            None,         ///< No actual updates
            Safe,         ///< Allow safe updates that cannot overwrite uncommitted data
            SafeCreate,   ///< Allow safe updates plus creation of missing files
            Force         ///< Allow all updates to force working directory to look like index
        };

        /**
         * Options specifying details about how a checkout is performed.
         */
        enum Flag {
            AllowConflicts,    ///< Allow checkout to make safe updates even if conflicts are found
            RemoveUntracked,   ///< Remove untracked files not in index (that are not ignored)
            RemoveIgnored,     ///< Remove ignored files not in index
            UpdateOnly,        ///< Only update existing files, don't create new ones
            DontUpdateIndex,   ///< Normally checkout updates index entries as it goes; this stops that
            NoRefresh          ///< Don't refresh index/config/etc before doing checkout
        };
        Q_DECLARE_FLAGS(Flags, Flag)

        /**
         * Constructs a new CheckoutOptions.
         * @param strategy the checkout strategy to use
         * @param flags details about the checkout process
         */
        CheckoutOptions(Strategy strategy = None, Flags flags = Flags());

        const git_checkout_options* data() const;

    private:
        git_checkout_options d;
    };

    /** @} */

}

#endif // LIBQGIT2_CHECKOUTOPTIONS_H
