#ifndef QRev_H
#define QRev_H

#include <QtCore/QObject>

#include "qgitrepository.h"

class QRev : public QObject
{
Q_OBJECT
public:
    QRev();
    virtual ~QRev();
private:
    LibQGit2::QGitRepository *repo;
};

#endif // QRev_H
