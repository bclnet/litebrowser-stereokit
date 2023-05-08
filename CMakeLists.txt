# To use this template, 
cmake_minimum_required(VERSION 3.11)
project(litebrowser VERSION "0.1.0" LANGUAGES CXX C)
include(FetchContent)

# Grab and build StereoKit from the GitHub repository. Here we're setting SK up as a statically linked library.
set(SK_BUILD_TESTS       OFF CACHE INTERNAL "")
set(SK_BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
FetchContent_Declare(StereoKitC GIT_REPOSITORY https://github.com/StereoKit/StereoKit.git GIT_TAG v0.3.6)
FetchContent_MakeAvailable(StereoKitC)

# Grab and build libs
FetchContent_Declare(cairo GIT_REPOSITORY https://github.com/tordex/cairo)
FetchContent_GetProperties(cairo)
if(NOT cairo_POPULATED)
	FetchContent_Populate(cairo)
	# link_directories(${cairo_SOURCE_DIR}/64)
	add_library(cairo SHARED IMPORTED)
	set_target_properties(cairo PROPERTIES IMPORTED_LOCATION ${cairo_SOURCE_DIR}/64/cairo.lib)
	set_target_properties(cairo PROPERTIES IMPORTED_IMPLIB   ${cairo_SOURCE_DIR}/64/cairo.lib)
	target_include_directories(cairo INTERFACE ${cairo_SOURCE_DIR})
endif()

# Grab and build libs
FetchContent_Declare(txdib GIT_REPOSITORY https://github.com/h3ml/txdib)
FetchContent_Declare(simpledib GIT_REPOSITORY https://github.com/h3ml/simpledib)
FetchContent_MakeAvailable(simpledib)
FetchContent_MakeAvailable(txdib simpledib)

# add litehtml
add_subdirectory(litehtml)

# Add source files to our target executable
add_executable(litebrowser
	src/main.cpp
)

# Link to dependencies
target_link_libraries(litebrowser
	PRIVATE StereoKitC
	PRIVATE cairo
	PRIVATE txdib
	PRIVATE simpledib
)