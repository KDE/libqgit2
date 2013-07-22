#ifndef QStatus_H
#define QStatus_H

#include <QtCore/QObject>

#include "qgitrepository.h"

class QStatus : public QObject
{
Q_OBJECT
public:
    QStatus();
    virtual ~QStatus();
private:
    LibQGit2::QGitRepository *repo;
};

#endif // QStatus_H
