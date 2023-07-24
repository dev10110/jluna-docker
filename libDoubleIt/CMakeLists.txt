cmake_minimum_required(VERSION 3.16)


project(doubleIt VERSION 0.0.1 DESCRIPTION "testing julia and jluna library")

set(CMAKE_CXX_STANDARD 20)

# set julia file locations
set(JULIA_PATH "/usr/local/julia")
set(JULIA_INCLUDE_DIRS "${JULIA_PATH}/include/julia")
set(JULIA_LIBRARY "${JULIA_PATH}/lib")

# set the jLuna directory
set(JLUNA_INCLUDE_DIRS "/root/jluna")


# link to julia and jLuna
link_directories(
	${JULIA_LIBRARY}
	"/usr/local"
)

# now find the jluna package
find_package(jluna REQUIRED)
# find_package(julia REQUIRED)


# now define the library
add_library(doubleIt  SHARED
	src/double_it.cpp
)
set_target_properties(doubleIt PROPERTIES 
	SOVERSION ${PROJECT_VERSION_MAJOR}
	VERSION ${PROJECT_VERSION})
set_target_properties(doubleIt PROPERTIES PUBLIC_HEADER include/double_it.hpp)

# needs to be position independent
target_compile_options(doubleIt PRIVATE "-fpic")

# needs to use cxx 20 features

target_include_directories(doubleIt PRIVATE
	${JLUNA_INCLUDE_DIRS}
	${JULIA_INCLUDE_DIRS}
	include	
)

target_link_libraries(doubleIt PUBLIC
	jluna
	julia
)


## INSTALL

include(GNUInstallDirs)

install(
	TARGETS doubleIt
	EXPORT doubleItTargets
	# FILE_SET HEADERS
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} 
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(
	EXPORT doubleItTargets
	FILE doubleItTargets.cmake
	NAMESPACE double_it::
	DESTINATION lib/cmake/doubleIt
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
	"doubleItConfigVersion.cmake"
	VERSION ${doubleIt_VERSION}
	COMPATIBILITY AnyNewerVersion)

install(
	FILES "doubleItConfig.cmake" "${CMAKE_CURRENT_BINARY_DIR}/doubleItConfigVersion.cmake"
	DESTINATION lib/cmake/doubleIt)

## CREATE THE DEMO EXECUTABLE
add_executable(demo 
	examples/main.cpp
	src/double_it.cpp
)
target_compile_options(demo PRIVATE "-fpic")

target_link_libraries(demo PUBLIC
	julia
	jluna
)
target_include_directories(demo PRIVATE
	${JLUNA_INCLUDE_DIRS}
	${JULIA_INCLUDE_DIRS}
	include)






