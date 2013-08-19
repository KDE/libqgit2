/******************************************************************************
 * This file is part of the libqgit2 library
 * Copyright (c) 2011 Laszlo Papp <djszapi@archlinux.us>
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

#ifndef LIBQGIT2_INDEX_MODEL_H
#define LIBQGIT2_INDEX_MODEL_H

#include "qgitindex.h"

#include <QAbstractListModel>

namespace LibQGit2
{

class LIBQGIT2_INDEX_EXPORT QGitIndexModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit QGitIndexModel(const QGitIndex& index, QObject *parent = 0);
    ~QGitIndexModel();

    int rowCount(const QModelIndex& parent) const;

    QVariant data(const QModelIndex& index, int role) const;

private:
    QGitIndex m_index;
};

}

#endif // LIBQGIT2_INDEX_MODEL_H
