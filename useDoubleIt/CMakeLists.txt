cmake_minimum_required(VERSION 3.16)


project(useDoubleIt)

set(CMAKE_CXX_STANDARD 11)

# set julia file locations
set(JULIA_PATH "/usr/local/julia")
set(JULIA_INCLUDE_DIRS "${JULIA_PATH}/include/julia")
set(JULIA_LIBRARY "${JULIA_PATH}/lib")

link_directories(
	${JULIA_LIBRARY}
	"/usr/local"
)
message("trying to link against ${JULIA_LIBRARY}")

find_library(doubleIt REQUIRED
	 NAMES doubleIt libdoubleIt
	#HINTS /usr/local/lib/cmake/doubleIT
)


add_executable(main main.cpp)

target_link_libraries(main PUBLIC
	doubleIt
	# julia
)

target_link_directories(main PUBLIC
	"/usr/local/julia/lib/"
)

target_compile_options(main PRIVATE "-fpic")
