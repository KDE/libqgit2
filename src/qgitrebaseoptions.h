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

#ifndef REBASEOPTIONS_H
#define REBASEOPTIONS_H

#include "libqgit2_config.h"
#include "git2.h"
#include <QSharedPointer>

namespace LibQGit2
{
class CheckoutOptions;

/**
 * Options that specify how a rebase is performed.
 *
 * @ingroup LibQGit2
 * @{
 */
class LIBQGIT2_EXPORT RebaseOptions
{
public:
    /**
     * Constructs a new RebaseOptions object.
     * @param checkoutOpts Specifies the checkout options to be used when applying patches
     */
    RebaseOptions(const CheckoutOptions &checkoutOpts);

    const CheckoutOptions &checkoutOptions() const;

    const git_rebase_options *constData() const;
private:
    struct Private;
    QSharedPointer<Private> d_ptr;
};
/** @} */
}

#endif // REBASEOPTIONS_H
