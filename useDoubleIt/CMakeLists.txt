cmake_minimum_required(VERSION 3.16)


project(useDoubleIt)

set(CMAKE_CXX_STANDARD 11)

# SET(CMAKE_EXE_LINKER_FLAGS
#           "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath -Wl,/usr/local/julia/lib")
# set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# set julia file locations
set(JULIA_PATH "/usr/local/julia")
set(JULIA_INCLUDE_DIRS "${JULIA_PATH}/include/julia")
set(JULIA_LIBRARY "${JULIA_PATH}/lib")

link_directories(
	${JULIA_LIBRARY}
	# "/usr/local"
	"/usr/local/lib"
	"/usr/local/julia/lib"
)


# set(LD_LIBRARY_PATH "/usr/local/lib:${LD_LIBRARY_PATH}")
# set(LD_LIBRARY_PATH "/usr/local/julia/lib:${LD_LIBRARY_PATH}")

find_library(doubleIt REQUIRED
		 NAMES doubleIt libdoubleIt
	#HINTS /usr/local/lib/cmake/doubleIT
)



add_executable(main main.cpp)

target_link_libraries(main PUBLIC
	doubleIt
	julia
	# /usr/local/julia/lib/libjulia.so.1

)

# target_link_directories(main PUBLIC
# 	"/usr/local/julia/lib/"
# )

target_compile_options(main PRIVATE "-fpic")
