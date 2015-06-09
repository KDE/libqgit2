# 0.22

## v0.22.0

* libgit2 types revealed through the libqgit2 API renamed as needed.
* Removed the Push object and Repository::push() method. They got
  replaced by the Remote::push() method.
* A slight behavioral change in Repository::fetch() method when no
  refspecs are given: previously all branches were fetched. Now the
  fetch refspecs configured for the remote are used.

## v0.22.1

* Added CMake code to find libgit2 as required by KDE CI system.

## v0.22.2

* Handle paths with non-ASCII characters correctly on Windows.

# 0.next

## v0.next.0

* Added basic rebase support.
* Repository::fetch() reports progress via fetchProgress signal.
* Added Repository::shouldIgnore() method.
