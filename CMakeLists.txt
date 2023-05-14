cmake_minimum_required(VERSION 3.11)
project(litebrowser VERSION "0.1.0" LANGUAGES CXX)
include(FetchContent)
if (UNIX)
    find_package(PkgConfig REQUIRED)
    set(SHLWAPI)
else()
    find_library(SHLWAPI Shlwapi.lib) 
endif()

# function: target_link_resources (https://stackoverflow.com/questions/11813271/embed-resources-eg-shader-code-images-into-executable-library-with-cmake)
if (NOT CMAKE_ARCHIVE_OUTPUT_DIRECTORY)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
endif()
set(RC_DEPENDS "")
message(linker: ${CMAKE_LINKER})
function(target_link_resources target path resources)
    foreach(input IN LISTS resources)
        file(RELATIVE_PATH relinput ${path} ${input})
        string(MAKE_C_IDENTIFIER ${relinput} input_identifier)
        set(output "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}/${input_identifier}.o")
        target_link_libraries(${target} ${output})
        message(${output})
        add_custom_command(
            OUTPUT ${output}
            COMMAND ${CMAKE_LINKER} --relocatable --format binary --output ${output} ${input}
            DEPENDS ${input}
        )
        set(RC_DEPENDS ${RC_DEPENDS} ${output} PARENT_SCOPE)
    endforeach()
endfunction()

# build: release build
#if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
#    set(CMAKE_BUILD_TYPE "Release")
#    message(STATUS "Build type not specified: defaulting to release")
#endif()

# module: add StereoKit
set(SK_BUILD_TESTS       OFF CACHE INTERNAL "")
set(SK_BUILD_SHARED_LIBS OFF CACHE INTERNAL "")
FetchContent_Declare(StereoKitC GIT_REPOSITORY https://github.com/StereoKit/StereoKit.git GIT_TAG v0.3.6)
FetchContent_MakeAvailable(StereoKitC)

# module: add cairo
if (UNIX)
    pkg_check_modules(LB_LIBS REQUIRED cairo)
else()
    FetchContent_Declare(cairo GIT_REPOSITORY https://github.com/tordex/cairo)
    FetchContent_GetProperties(cairo)
    if(NOT cairo_POPULATED)
	    FetchContent_Populate(cairo)
	    add_library(cairo SHARED IMPORTED)
	    set_target_properties(cairo PROPERTIES IMPORTED_LOCATION ${cairo_SOURCE_DIR}/64/cairo.lib)
	    set_target_properties(cairo PROPERTIES IMPORTED_IMPLIB   ${cairo_SOURCE_DIR}/64/cairo.lib)
	    target_include_directories(cairo INTERFACE ${cairo_SOURCE_DIR}/src)
    endif()
    set(LB_LIBS_INCLUDE_DIRS ${cairo_SOURCE_DIR}/src)
    set(LB_LIBS_LIBRARIES cairo)
endif()

# module: add txdib simpledib
FetchContent_Declare(txdib GIT_REPOSITORY https://github.com/h3ml/txdib)
FetchContent_Declare(simpledib GIT_REPOSITORY https://github.com/h3ml/simpledib)
FetchContent_MakeAvailable(txdib simpledib)
set(LB_LIBS_INCLUDE_DIRS ${LB_LIBS_INCLUDE_DIRS} ${txdib_SOURCE_DIR} ${simpledib_SOURCE_DIR})
set(LB_LIBS_LIBRARIES ${LB_LIBS_LIBRARIES} txdib simpledib)

# module: add litehtml
set(BUILD_TESTING 0)
add_subdirectory(litehtml)

# set source and headers
set(CONTAINERS litehtml/containers)
set(SOURCE
    #${CONTAINERS}/stereokit/container_stereokit.cpp
    ${CONTAINERS}/cairo/cairo_container.cpp
    ${CONTAINERS}/cairo/cairo_font.cpp
    src/litehtml/el_omnibox.cpp
    src/litehtml/sl_edit.cpp
    src/main.cpp
    src/browser_wnd.cpp
    src/htmlview_wnd.cpp
    src/toolbar_wnd.cpp
    src/web_history.cpp
    src/web_page.cpp
    src/message.cpp
)
set(HEADERS
    #${CONTAINERS}/stereokit/container_stereokit.h
    ${CONTAINERS}/cairo/cairo_container.cpp
    ${CONTAINERS}/cairo/cairo_font.cpp
    src/litehtml/el_omnibox.h
    src/litehtml/sl_edit.h
    src/globals.h
    src/browser_wnd.h
    src/htmlview_wnd.h
    src/toolbar_wnd.h
    src/web_history.h
    src/web_page.h
    src/message.h
)
file(GLOB RESOURCES src/html/*.*)
if (UNIX)
    set(SOURCE ${SOURCE}
        src/linux/http_loader.cpp
    )
    set(HEADERS ${HEADERS}
        src/linux/http_loader.h
    )
else()
    set(SOURCE ${SOURCE}
        src/win32/http_loader.cpp
        src/win32/TxThread.cpp
    )
    set(HEADERS ${HEADERS}
        src/win32/http_loader.h
        src/win32/TxThread.h
    )
    set(LB_LIBS_LIBRARIES ${LB_LIBS_LIBRARIES} gdiplus shlwapi Msimg32)
endif()

# executable
add_executable(litebrowser ${SOURCE} ${HEADERS})
target_compile_definitions(litebrowser PUBLIC UNICODE)
include_directories(litebrowser
    ${StereoKitC_SOURCE_DIR}
    ${litehtml_SOURCE_DIR}/include
    ${LB_LIBS_INCLUDE_DIRS})
target_link_resources(litebrowser ${CMAKE_CURRENT_SOURCE_DIR}/src "${RESOURCES}")
target_link_options(litebrowser
    PRIVATE ${LB_LIBS_LDFLAGS})
target_link_libraries(litebrowser
    StereoKitC
    litehtml
    ${LB_LIBS_LIBRARIES})
set_target_properties(litebrowser PROPERTIES
    CXX_STANDARD 11
    C_STANDARD 9
)
add_custom_target(rc ALL DEPENDS ${RC_DEPENDS})
# msvc: add incremental builds
if(MSVC)
    #target_compile_options(litebrowser PUBLIC "/ZI")
    target_link_options(litebrowser PUBLIC "/INCREMENTAL")
endif()

# copy dll
add_custom_command(TARGET litebrowser POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${cairo_SOURCE_DIR}/64/cairo.dll"
    $<TARGET_FILE_DIR:litebrowser>)
add_custom_command(TARGET litebrowser POST_BUILD 
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${txdib_SOURCE_DIR}/../freeimage-src/64/freeimage.dll"
    $<TARGET_FILE_DIR:litebrowser>)