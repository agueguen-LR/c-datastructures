add_library(lib SHARED lib.c lib.h)

find_package(Coverage)
enable_coverage()

find_package(HtmlCoverage)
enable_html_coverage()
