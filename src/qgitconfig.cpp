#include "qgitconfig.h"

#include <QtCore/QDir>
#include <QtCore/QFile>

#include <git2/errors.h>
#include <git2/config.h>

#include <qgitexception.h>
#include <qgitrepository.h>

namespace LibQGit2
{


QGitConfig::QGitConfig()
    : d(0)
{
    // try to open the global configuration file
    // on error, initialize the internal pointer
    if ( git_config_open_global(&d) < GIT_SUCCESS)
        git_config_new(&d);
}

QGitConfig::~QGitConfig()
{
    git_config_free(d);
}

QString QGitConfig::findGlobal()
{
    char buffer[GIT_PATH_MAX];
    qGitThrow( git_config_find_global(buffer) );

    return QFile::decodeName(buffer);
}

QString QGitConfig::findSystem()
{
    char buffer[GIT_PATH_MAX];
    qGitThrow( git_config_find_system(buffer) );

    return QFile::decodeName(buffer);
}

bool QGitConfig::open(const QString &path)
{
    return GIT_SUCCESS == git_config_add_file_ondisk(d, QFile::encodeName(path).constData(), Local);
}

bool QGitConfig::open(const QGitRepository &repo)
{
    return open(QDir::cleanPath(repo.path() + "/config"));
}

QVariant QGitConfig::value(const QString &key, const QVariant &defaultValue) const
{
    const char * result = 0;
    if (git_config_get_string(d, key.toUtf8().constData(), &result) == 0)
        return QString::fromUtf8(result);

    return defaultValue;
}

void QGitConfig::setValue(const QString &key, const QVariant &value)
{
    qGitThrow( git_config_set_string(d, key.toUtf8(), value.toString().toUtf8().constData()) );
}


} // LibQGit2

