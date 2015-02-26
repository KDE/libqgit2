libqgit2
========

A Qt/C++ wrapper for libgit2

Official repository is
https://projects.kde.org/projects/playground/libs/libqgit2/repository/

Feel free to fork this repository to play with the library but remember
it is not the official one.

The version number follows that of libgit2: libqgit2 0.21.x uses libgit2
0.21.x and libqgit2 0.22.x uses libgit2 0.22.x and so on.

Compiler warnings
=================

Currently, compiler complains with the use of QSharedPointer on opaque
libgit structures. This is a known issue of Qt4 that has already been
fixed in Qt5, see [this report](https://codereview.qt-project.org/#change,26974).
I'm going to keep the QSharedPointer stuff by now, I'll replace it only
if problems arise.
