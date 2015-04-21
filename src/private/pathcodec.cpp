#include "pathcodec.h"
#include <QFile>

// POSIX emulation in libgit2 assumes all i/o to be UTF-8 encoded
#ifdef _WIN32
QByteArray PathCodec::toLibGit2(const QString &path)
{
    return path.toUtf8();
}

QString PathCodec::fromLibGit2(const QByteArray &path)
{
    return QString::fromUtf8(path);
}
// in every other system the POSIX functions probably expect
// encoding to be the same as system locale, so let's not
// fix that which isn't broken
#else
QByteArray PathCodec::toLibGit2(const QString &path)
{
    return QFile::encodeName(path);
}

QString PathCodec::fromLibGit2(const QByteArray &path)
{
    return QFile::decodeName(path);
}
#endif

QString PathCodec::fromLibGit2(const char *path)
{
    return fromLibGit2(QByteArray(path));
}
