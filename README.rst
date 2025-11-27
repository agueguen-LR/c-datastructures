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

.. code-block:: bash

  mkdir build
  cd build
  cmake ..  \
     -DBUILD_COVERAGE=1 \
     -DBUILD_HTML_COVERAGE=1 \
     -DUSE_VALGRIND=1 \
     -DCMAKE_BUILD_TYPE=Debug
  make

Review Code
-----------

Run tests and code coverage
~~~~~~~~~~~~~~~~~~~~~~~~~~~

Run the tests and check the code coverage.

Code coverage is generated in the build/coverage directory.

.. code-block:: bash

   make test
   make coverage
   make html-coverage

You can also run the test executables directly for basic visualisations of the structure during 'add' and 'remove' operations.

.. code-block:: bash

    ./test/avl-tree-test
    ./test/rb-tree-test
  
Run benchmarking
~~~~~~~~~~~~~~~~

Run the benchmarks for insert, search and remove operations.

There's an executable for each data structure, you must specify how many elements to insert/search/remove, how many operations should happen between each time measurement and a file prefix for the  3 output files (<prefix>_add.csv, <prefix>_search.csv and <prefix>_remove.csv).

2^20 (1,048,576) nodes is the max benchmarking node count currently.

The files that will be created based on the provided prefix must not already exist.

.. code-block:: bash

  ./benchmarking/avl-benchmark <node-count> <batch-size> <file-prefix> 
  ./benchmarking/rb-benchmark <node-count> <batch-size> <file-prefix> 
  

Run flaw finder
~~~~~~~~~~~~~~~

Check for any security issues in the code.

.. code-block:: bash

  make flawfinder

Format code
~~~~~~~~~~~~

Format the code using clang-format.

.. code-block:: bash

  make clang-format


.. |cmake| image:: https://github.com/agueguen-LR/c-datastructures/actions/workflows/cmake.yml/badge.svg
   :target: https://github.com/agueguen-LR/c-datastructures/actions
.. |Coveralls| image:: https://coveralls.io/repos/github/agueguen-LR/c-datastructures/badge.svg?branch=main
    :target: https://coveralls.io/github/agueguen-LR/c-datastructures?branch=main
