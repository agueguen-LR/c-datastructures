add_executable(avl-benchmark avl-benchmark.c benchmark.c)
add_dependencies(avl-benchmark avl-tree)
target_link_libraries(avl-benchmark avl-tree)
target_include_directories(avl-benchmark PRIVATE ${CMAKE_SOURCE_DIR}/src/avl-tree/)

add_executable(rb-benchmark rb-benchmark.c benchmark.c)
add_dependencies(rb-benchmark red-black-tree)
target_link_libraries(rb-benchmark red-black-tree)
target_include_directories(rb-benchmark PRIVATE ${CMAKE_SOURCE_DIR}/src/red-black-tree/)
