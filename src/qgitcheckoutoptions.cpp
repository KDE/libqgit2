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

#include "qgitcheckoutoptions.h"

namespace LibQGit2
{

CheckoutOptions::CheckoutOptions(Strategy strategy, Flags flags)
{
    git_checkout_options temp = GIT_CHECKOUT_OPTIONS_INIT;
    d = temp;

    switch (strategy) {
    case Safe:
        d.checkout_strategy |= GIT_CHECKOUT_SAFE;
        break;
    case SafeCreate:
        d.checkout_strategy |= GIT_CHECKOUT_SAFE_CREATE;
        break;
    case Force:
        d.checkout_strategy |= GIT_CHECKOUT_FORCE;
        break;
    default:
        break;
    }

    if (flags.testFlag(AllowConflicts)) {
        d.checkout_strategy |= GIT_CHECKOUT_ALLOW_CONFLICTS;
    }
    if (flags.testFlag(RemoveUntracked)) {
        d.checkout_strategy |= GIT_CHECKOUT_REMOVE_UNTRACKED;
    }
    if (flags.testFlag(RemoveIgnored)) {
        d.checkout_strategy |= GIT_CHECKOUT_REMOVE_IGNORED;
    }
    if (flags.testFlag(UpdateOnly)) {
        d.checkout_strategy |= GIT_CHECKOUT_UPDATE_ONLY;
    }
    if (flags.testFlag(DontUpdateIndex)) {
        d.checkout_strategy |= GIT_CHECKOUT_DONT_UPDATE_INDEX;
    }
    if (flags.testFlag(NoRefresh)) {
        d.checkout_strategy |= GIT_CHECKOUT_NO_REFRESH;
    }
}

const git_checkout_options* CheckoutOptions::data() const {
    return &d;
}

}
