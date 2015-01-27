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

#include "git2.h"

#include "qgitexception.h"
#include <QtCore/QString>

namespace LibQGit2
{

Exception::Exception() :
    m_category(None)
{
    const git_error *err = giterr_last();

    if (err != NULL) {
        m_msg = err->message;

		switch (err->klass) {
		case GITERR_NOMEMORY:   m_category = NoMemory; break;
		case GITERR_OS:         m_category = OS; break;
		case GITERR_INVALID:    m_category = Invalid; break;
		case GITERR_REFERENCE:  m_category = Reference; break;
		case GITERR_ZLIB:       m_category = ZLib; break;
		case GITERR_REPOSITORY: m_category = Repository; break;
		case GITERR_CONFIG:     m_category = Config; break;
		case GITERR_REGEX:      m_category = RegEx; break;
		case GITERR_ODB:        m_category = ODB; break;
		case GITERR_INDEX:      m_category = Index; break;
		case GITERR_OBJECT:     m_category = Object; break;
		case GITERR_NET:        m_category = Net; break;
		case GITERR_TAG:        m_category = Tag; break;
		case GITERR_TREE:       m_category = Tree; break;
		case GITERR_INDEXER:    m_category = Indexer; break;
		case GITERR_SSL:        m_category = SSL; break;
		case GITERR_SUBMODULE:  m_category = Submodule; break;
		case GITERR_THREAD:     m_category = Thread; break;
		case GITERR_STASH:      m_category = Stash; break;
		case GITERR_CHECKOUT:   m_category = Checkout; break;
		case GITERR_FETCHHEAD:  m_category = FetchHead; break;
		case GITERR_MERGE:      m_category = Merge; break;
		case GITERR_SSH:        m_category = SSH; break;
		case GITERR_FILTER:     m_category = Filter; break;
		case GITERR_REVERT:     m_category = Revert; break;
		case GITERR_CALLBACK:   m_category = Callback; break;
		case GITERR_CHERRYPICK: m_category = Cherrypick; break;
		case GITERR_NONE:
		default:                m_category = None; break;
		}

        giterr_clear();
    }
}

Exception::Exception(const QString& msg, Category category) :
    m_msg(msg.toLatin1()),
    m_category(category)
{
}

Exception::~Exception() throw()
{
}

const char *Exception::what() const throw()
{
    return m_msg;
}

QByteArray Exception::message() const throw()
{
    return m_msg;
}

Exception::Category Exception::category() const throw()
{
    return m_category;
}

}
