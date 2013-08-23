/******************************************************************************
 * This file is part of the libqgit2 library
 * Copyright (c) 2011 Laszlo Papp <djszapi@archlinux.us>
 * Copyright (C) 2013 Leonardo Giordani
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

#include "qgitindexmodel.h"

#include <qgitindexentry.h>

namespace LibQGit2
{

IndexModel::IndexModel(const Index& index, QObject *parent)
    : QAbstractListModel(parent)
    , m_index(index)
{
}

IndexModel::~IndexModel()
{
}

int IndexModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid()) {
        return 0;
    } else {
        return m_index.entryCount();
    }
}

QVariant IndexModel::data(const QModelIndex& index, int role) const
{
    if (index.parent().isValid())
        return QVariant();

    if (index.column() != 0)
        return QVariant();

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        IndexEntry entry = m_index.getByIndex(index.row());
        return entry.path();
    } else {
        return QVariant();
    }
}

}
