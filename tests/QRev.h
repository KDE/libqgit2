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
    LibQGit2::Repository *repo;
};

#endif // QRev_H
