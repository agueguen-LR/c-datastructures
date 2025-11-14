add_executable(avl-benchmark avl-benchmark.c benchmark.c)
add_dependencies(avl-benchmark avl-tree)
target_link_libraries(avl-benchmark avl-tree)
target_include_directories(avl-benchmark PRIVATE ${CMAKE_SOURCE_DIR}/src/avl-tree/)
