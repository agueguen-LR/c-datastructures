add_library(avl-tree SHARED avl-tree/avl-tree.c)

add_library(c-datastructures INTERFACE)
target_link_libraries(c-datastructures INTERFACE
	avl-tree
)
target_include_directories(c-datastructures INTERFACE
    ${CMAKE_SOURCE_DIR}/avl-tree
)

find_package(Coverage)
enable_coverage()

find_package(HtmlCoverage)
enable_html_coverage()
