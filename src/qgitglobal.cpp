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

#include "qgitglobal.h"

#include "git2.h"

#include <QAtomicInt>

namespace LibQGit2
{

namespace internal {
const int Uninitialized = 0;
const int Initialized = 1;

QAtomicInt LibInitialized(Uninitialized);
}

using namespace internal;

bool initLibQGit2() {
    bool ret = false;
    if (LibInitialized.fetchAndAddRelaxed(Initialized) == Uninitialized) {
        git_libgit2_init();
        ret = true;
    }
    return ret;
}

bool shutdownLibQGit2() {
    bool ret = false;
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    if (int(LibInitialized) > Uninitialized) {
#else
    if (LibInitialized.load() > Uninitialized) {
#endif
        if (LibInitialized.fetchAndAddRelaxed(-Initialized) == Initialized) {
            git_libgit2_shutdown();
            ret = true;
        }
    }
    return ret;
}

}
