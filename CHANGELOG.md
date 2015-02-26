v0.22
-----

* libgit2 types revealed through the libqgit2 API renamed as needed.
* Removed the Push object and Repository::push() method. They got
  replaced by the Remote::push() method.
* A slight behavioral change in Repository::fetch() method when no
  refspecs are given: previously all branches were fetched. Now the
  fetch refspecs configured for the remote are used.
