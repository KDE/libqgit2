// A try to port libgit2 status.c example to libqgit2


#include "TestHelpers.h"

#include <iostream>

#include "qgitrepository.h"
#include "qgitstatuslist.h"
#include "qgitstatusentry.h"
#include "qgitdiffdelta.h"
#include "qgitdifffile.h"
#include "qgitstatus.h"
#include "qgitstatusoptions.h"


class TestStatusOptions : public QObject
{
    Q_OBJECT

public:
    TestStatusOptions();
};


using namespace LibQGit2;

TestStatusOptions::TestStatusOptions()
{
    Repository repo;
    repo.open(ExistingRepository);

    StatusOptions opt;
    opt.setShowType(StatusOptions::ShowIndexAndWorkdir);
    opt.setStatusFlags(StatusOptions::IncludeUnmodified | StatusOptions::IncludeUntracked |
        StatusOptions::RenamesHeadToIndex | StatusOptions::RenamesIndexToWorkdir
    );

    StatusList status_list = repo.status(opt);

    size_t entries = status_list.entryCount();
    for (size_t i = 0; i < entries; ++i) {
        const StatusEntry entry = status_list.entryByIndex(i);

        if (entry.status().isCurrent()) {
            std::cout << "C";
        } else {
            std::cout << " ";
        }

        if (entry.status().isNewInIndex()) {
            std::cout << "N";
        } else {
            std::cout << " ";
        }

        if (entry.status().isModifiedInIndex()) {
            std::cout << "M";
        } else {
            std::cout << " ";
        }

        if (entry.status().isDeletedInIndex()) {
            std::cout << "D";
        } else {
            std::cout << " ";
        }

        if (entry.status().isRenamedInIndex()) {
            std::cout << "R";
        } else {
            std::cout << " ";
        }

        if (entry.status().isTypeChangedInIndex()) {
            std::cout << "T";
        } else {
            std::cout << " ";
        }

        std::cout << " ";

        if (entry.status().isNewInWorkdir()) {
            std::cout << "N";
        } else {
            std::cout << " ";
        }

        if (entry.status().isModifiedInWorkdir()) {
            std::cout << "M";
        } else {
            std::cout << " ";
        }

        if (entry.status().isDeletedInWorkdir()) {
            std::cout << "D";
        } else {
            std::cout << " ";
        }

        if (entry.status().isRenamedInWorkdir()) {
            std::cout << "R";
        } else {
            std::cout << " ";
        }

        if (entry.status().isTypeChangedInWorkdir()) {
            std::cout << "T";
        } else {
            std::cout << " ";
        }

        std::cout << " ";

        std::cout << entry.indexToWorkdir().newFile().path().toStdString() << std::endl;
    }
}


QTEST_MAIN(TestStatusOptions)

#include "StatusOptions.moc"
