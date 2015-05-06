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
