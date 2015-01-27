/******************************************************************************
 * This file is part of the libqgit2 library
 * Copyright (C) 2011 Laszlo Papp <djszapi@archlinux.us>
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

#ifndef LIBQGIT2_LIB_CONFIG_H
#define LIBQGIT2_LIB_CONFIG_H

#include <QtCore/QtGlobal>

/** @defgroup LibQGit2 The Qt Library for Git revision control featuring libgit2
 * Qt wrapper classes for the LibGit2 library.
 */

#ifndef LIBQGIT2_EXPORT
#if defined(MAKE_LIBQGIT2_LIB)
#define LIBQGIT2_EXPORT Q_DECL_EXPORT
#else
#define LIBQGIT2_EXPORT Q_DECL_IMPORT
#endif
#endif

#ifndef LIBQGIT2_EXPORT_DEPRECATED
#define LIBQGIT2_EXPORT_DEPRECATED Q_DECL_DEPRECATED LIBQGIT2_EXPORT
#endif


#ifndef LIBQGIT2_FUNC_NAME
#define LIBQGIT2_FUNC_NAME __func__
#endif

#endif // LIBQGIT2_LIB_CONFIG_H
