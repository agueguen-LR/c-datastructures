C datastructures
================
|cmake| |Coveralls| 

A collection of C data structures with unit tests and insert, search and remove benchmarking.

Usage
-----

Requirements
~~~~~~~~~~~~

The following packages are required to build and test the project:

- CMake (3.10 or higher)
- GCC
- GNU Make

- clang-tools (optional, for code formatting)
- flawfinder (optional, for security analysis)
- lcov (optional, for code coverage)
- valgrind (optional, for memory checking)

Compilation
~~~~~~~~~~~

Create the build directory and compile the project.

.. code:: shell-session

  $ mkdir build
  $ cd build
  $ cmake ..  \
     -DBUILD_COVERAGE=1 \
     -DBUILD_HTML_COVERAGE=1 \
     -DUSE_VALGRIND=1 \
  $ make

Review Code
-----------

Run test and code coverage
~~~~~~~~~~~~~~~~~~~~~~~~~~

Run the tests and check the code coverage.

Code coverage is generated in the build/coverage directory.

.. code:: shell-session

   $ make test
   $ make coverage
   $ make html-coverage

Run flaw finder
~~~~~~~~~~~~~~~

Check for any security issues in the code.

.. code:: shell-session

  $ make flawfinder

Format code
~~~~~~~~~~~~

Format the code using clang-format.

.. code:: shell-session

  $ make clang-format


.. |cmake| image:: https://github.com/agueguen-LR/c-datastructures/actions/workflows/cmake.yml/badge.svg
   :target: https://github.com/agueguen-LR/c-datastructures/actions
.. |Coveralls| image:: https://coveralls.io/repos/github/agueguen-LR/c-datastructures/badge.svg?branch=main
    :target: https://coveralls.io/github/agueguen-LR/c-datastructures?branch=main
