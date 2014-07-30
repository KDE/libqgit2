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

#include <QFile>

namespace LibQGit2
{

class CheckoutOptions::Private
{
public:
    Private(Strategy strategy, Flags flags)
    {
        git_checkout_options temp = GIT_CHECKOUT_OPTIONS_INIT;
        native = temp;

        switch (strategy) {
        case Safe:
            native.checkout_strategy |= GIT_CHECKOUT_SAFE;
            break;
        case SafeCreate:
            native.checkout_strategy |= GIT_CHECKOUT_SAFE_CREATE;
            break;
        case Force:
            native.checkout_strategy |= GIT_CHECKOUT_FORCE;
            break;
        default:
            break;
        }

        if (flags.testFlag(AllowConflicts)) {
            native.checkout_strategy |= GIT_CHECKOUT_ALLOW_CONFLICTS;
        }
        if (flags.testFlag(RemoveUntracked)) {
            native.checkout_strategy |= GIT_CHECKOUT_REMOVE_UNTRACKED;
        }
        if (flags.testFlag(RemoveIgnored)) {
            native.checkout_strategy |= GIT_CHECKOUT_REMOVE_IGNORED;
        }
        if (flags.testFlag(UpdateOnly)) {
            native.checkout_strategy |= GIT_CHECKOUT_UPDATE_ONLY;
        }
        if (flags.testFlag(DontUpdateIndex)) {
            native.checkout_strategy |= GIT_CHECKOUT_DONT_UPDATE_INDEX;
        }
        if (flags.testFlag(NoRefresh)) {
            native.checkout_strategy |= GIT_CHECKOUT_NO_REFRESH;
        }
        if (flags.testFlag(SkipUnmerged)) {
            native.checkout_strategy |= GIT_CHECKOUT_SKIP_UNMERGED;
        }
        if (flags.testFlag(UnmergedUseOurs)) {
            native.checkout_strategy |= GIT_CHECKOUT_USE_OURS;
        }
        if (flags.testFlag(UnmergedUseTheirs)) {
            native.checkout_strategy |= GIT_CHECKOUT_USE_THEIRS;
        }
    }

    ~Private() {
        free(native.paths.strings);
    }

    void setTargetDirectory(const QString &dir)
    {
        m_target_directory = QFile::encodeName(dir);
        native.target_directory = m_target_directory.constData();
    }

    void setPaths(const QList<QString> &paths)
    {
        const size_t newCount = paths.size();
        if (newCount == 0) {
            free(native.paths.strings);
            native.paths.strings = NULL;
        } else if (newCount > native.paths.count) {
            native.paths.strings = (char**)realloc(native.paths.strings, newCount * sizeof(char*));
        }
        native.paths.count = newCount;

        m_paths.clear();
        foreach (const QString &path, paths) {
            m_paths.append(QFile::encodeName(path));
        }

        for (size_t i = 0; i < newCount; ++i) {
            native.paths.strings[i] = m_paths[i].data();
        }
    }

    git_checkout_options native;
    QByteArray m_target_directory;
    QList<QByteArray> m_paths;
};


CheckoutOptions::CheckoutOptions(Strategy strategy, Flags flags)
    : d_ptr(new Private(strategy, flags))
{
}

const git_checkout_options* CheckoutOptions::data() const
{
    return &d_ptr->native;
}

void CheckoutOptions::setTargetDirectory(const QString &dir)
{
    d_ptr->setTargetDirectory(dir);
}

void CheckoutOptions::setPaths(const QList<QString> &paths)
{
    d_ptr->setPaths(paths);
}

}
