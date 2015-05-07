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

#include "qgitrebaseoptions.h"
#include "qgitcheckoutoptions.h"

namespace LibQGit2
{

struct RebaseOptions::Private {
    Private(const CheckoutOptions &checkoutOpts)
        : mCheckoutOptions(checkoutOpts),
          mOpts()
    {
        git_rebase_options &opts = const_cast<git_rebase_options &>(mOpts);
        git_rebase_init_options(&opts, GIT_REBASE_OPTIONS_VERSION);
    }

    const CheckoutOptions &checkoutOptions() const
    {
        return mCheckoutOptions;
    }

    const git_rebase_options *constData() const
    {
        return &mOpts;
    }

private:
    const CheckoutOptions mCheckoutOptions;
    const git_rebase_options mOpts;
};

RebaseOptions::RebaseOptions(const CheckoutOptions &checkoutOptions)
    : d_ptr(new Private(checkoutOptions))
{
}

const CheckoutOptions &RebaseOptions::checkoutOptions() const
{
    return d_ptr->checkoutOptions();
}

const git_rebase_options *RebaseOptions::constData() const
{
    return d_ptr->constData();
}
}
