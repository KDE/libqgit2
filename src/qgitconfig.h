/******************************************************************************
 * This file is part of the Gluon Development Platform
 * Copyright (c) 2012 Laszlo Papp <djszapi@archlinux.us>
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

#ifndef QGITCONFIG_H
#define QGITCONFIG_H

#include <libqgit2_export.h>

#include <QtCore/QVariant>

struct git_config;

namespace LibQGit2
{

    class QGitRepository;

    /**
      * @brief Represents the git configuration file.
      *
      * @ingroup LibQGit2
      * @{
      */
    class LIBQGIT2_EXPORT QGitConfig
    {
    public:
        /**
          The configuration´s priority, when a value is read.
          */
        enum AccessPriority
        {
            Global  = 1,
            Local   = 2
        };

   public:
        QGitConfig();
        virtual ~QGitConfig();

        bool open(const QString &path);
        bool open(const QGitRepository &repo);

        /**
          * Read a value from the configuration.
          * @todo handle the QVariant type correctly
          */
        QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
        /**
          * Write a value in the configuration.
          *
          * @param key the name of the value to write
          * @param value the value
          *
          * @todo handle the QVariant type correctly
          */
        void setValue(const QString &key, const QVariant &value);
        /**
          * Remove a value from the configuration.
          *
          * @param key the name for the value to remove
          */
        void remove(const QString &key);

        //! @todo implement QGitConfig::foreach
        //foreach()

    public:
        /**
          * Searches for the global configuration file located in $HOME.
          * @see git_config_find_global
          */
        static QString findGlobal();

        /**
          * Searches for the system configuration file.
          * @see git_config_find_system
          */
        static QString findSystem();

    private:
        git_config *    d; //!< internal pointer to the libgit2 config instance
    };

    /**@}*/
}

#endif // QGITCONFIG_H
