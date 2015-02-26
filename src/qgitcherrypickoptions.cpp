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

#include "qgitcherrypickoptions.h"

namespace LibQGit2
{

class CherryPickOptions::Private
{
public:
    Private(const MergeOptions &mergeOpts, const CheckoutOptions &checkoutOpts)
    {
        git_cherrypick_options temp = GIT_CHERRYPICK_OPTIONS_INIT;
        native = temp;

        native.merge_opts = *mergeOpts.data();
        native.checkout_opts = *checkoutOpts.data();
    }

    git_cherrypick_options native;
};


CherryPickOptions::CherryPickOptions(const MergeOptions &mergeOpts, const CheckoutOptions &checkoutOpts)
    : d_ptr(new Private(mergeOpts, checkoutOpts))
{
}


void CherryPickOptions::setMainline(unsigned int mainline)
{
    d_ptr->native.mainline = mainline;
}

const git_cherrypick_options *CherryPickOptions::data() const
{
    return &d_ptr->native;
}

}
