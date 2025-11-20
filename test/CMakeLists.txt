include_directories(${CMAKE_SOURCE_DIR}/src)

find_package(Valgrind)
message(STATUS "VALGRIND_EXECUTABLE=${VALGRIND_EXECUTABLE}")
if(VALGRIND_EXECUTABLE)
  set(VALGRIND ${VALGRIND_EXECUTABLE})
endif()

file(GLOB FILES "${CMAKE_SOURCE_DIR}/test/*.c")
message(STATUS "FILES=${FILES}")

foreach(FILENAME ${FILES})
  get_filename_component(TEST ${FILENAME} NAME_WE)
  add_executable(${TEST} ${FILENAME})
  add_dependencies(${TEST} c-datastructures)
  target_link_libraries(${TEST} c-datastructures)
  target_include_directories(${TEST} PRIVATE
		${CMAKE_SOURCE_DIR}/src/avl-tree/
		${CMAKE_SOURCE_DIR}/src/red-black-tree/
	)
  add_test("${TEST}" ./${TEST})
  if(VALGRIND)
    add_test("${TEST}[valgrind]" ${VALGRIND} --leak-check=full --quiet
             --error-exitcode=1 ./${TEST})
  endif()
endforeach()
