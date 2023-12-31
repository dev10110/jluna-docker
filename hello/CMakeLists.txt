cmake_minimum_required(VERSION 3.15)

project(hello)

# set file locations 
set(JULIA_PATH "/usr/local/julia")
set(JULIA_INCLUDE_DIRS "${JULIA_PATH}/include/julia")
set(JULIA_LIBRARY "${JULIA_PATH}/lib")
set(JLUNA_INCLUDE_DIRS "/root/jluna")


# link to julia and jluna
link_directories(
	${JULIA_LIBRARY}
	"/usr/local"
)
find_package(jluna REQUIRED)

add_executable(hello hello.cpp)

# needs to be positition independent code
target_compile_options(hello PRIVATE "-fpic")

target_link_libraries(hello PUBLIC
	jluna
	julia
)

target_include_directories(hello PRIVATE
	${JLUNA_INCLUDE_DIRS}
	${JULIA_INCLUDE_DIRS}
)

target_compile_features(hello PRIVATE cxx_std_20)
