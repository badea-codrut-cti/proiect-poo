cmake_minimum_required(VERSION 3.10)

project(autodragan)

add_definitions(-DPROJECT_NAME="${PROJECT_NAME}")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "install_dir" CACHE PATH "..." FORCE)
endif()

set(is_debug "$<CONFIG:Debug>")
set(is_rel_with_deb "$<CONFIG:RelWithDebInfo>")
set(debug_mode "$<OR:${is_debug},${is_rel_with_deb}>")


function(set_custom_stdlib_and_sanitizers target add_apple_asan)
    if(MSVC)
        # see https://gitlab.kitware.com/cmake/cmake/-/issues/24922
        set_target_properties(${target} PROPERTIES VS_USER_PROPS "${CMAKE_SOURCE_DIR}/disable_modules.props")
        target_compile_options(${target} PRIVATE "$<${debug_mode}:/fsanitize=address>" /experimental:module-)
        return()
    endif()

    if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
        target_compile_options(${target} PRIVATE -stdlib=libc++)
        target_link_options(${target} PRIVATE -stdlib=libc++)
    endif()

    if(APPLE)
        # first check Apple since Apple is also a kind of Unix
        if("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang" AND add_apple_asan MATCHES true)
            target_compile_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
            target_link_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
        endif()
    elseif(UNIX)
        if("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
            if("${CMAKE_CXX_COMPILER_VERSION}" MATCHES "12.")
                # check leaks on Linux since macOS does not support the leaks sanitizer yet
                # leaks sanitizer is enabled by default on Linux, so we do not need to enable it explicitly
                target_compile_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
                target_link_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=address,undefined>")
            endif()
        elseif("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
            if("${CMAKE_CXX_COMPILER_VERSION}" MATCHES "14.")
                # use semi-colons instead of spaces to separate arguments
                # it is recommended to quote generator expressions in order to avoid unintentional splitting
                target_compile_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=memory,undefined;-fsanitize-recover=memory,undefined;-fsanitize-memory-track-origins>")
                target_link_options(${target} PRIVATE "$<${debug_mode}:-fsanitize=memory,undefined;-fsanitize-recover=memory,undefined;-fsanitize-memory-track-origins;-Wl,-rpath,tools/llvm-project/build/lib>")
            else()
                message("No matching Clang version to add sanitizer flags!")
            endif()
        endif()
    endif()
endfunction()

include(FetchContent)

FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
)

FetchContent_Declare(
  webview
  GIT_REPOSITORY https://github.com/webview/webview.git
)

FetchContent_MakeAvailable(json)

FetchContent_GetProperties(webview)
if(NOT webview_POPULATED)
  FetchContent_Populate(webview)
endif()

###############################################################################

# external dependencies with FetchContent
# include(FetchContent)
#
# NOTE: Also update env vars used for caching in
#         - .github/actions/configure-cmake/action.yml
#         - .github/workflows/cmake.yml
# FetchContent_Declare(
#         SomeLib
#         GIT_REPOSITORY https://github.com/<SomeUser>/<SomeLib>.git
#         GIT_TAG        <some_git_hash> # <which tag/branch @ <some_date>>
#         GIT_SHALLOW    1  # works only with branches or tags, not with arbitrary commit hashes
# )
#
# FetchContent_MakeAvailable(SomeLib)

###############################################################################

# external dependencies with find_package

# find_package(Threads REQUIRED)

###############################################################################

# NOTE: update executable name in .github/workflows/cmake.yml:25 when changing name here
file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES "src/*.cpp" "src/**/*.cpp")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set (EXCLUDE_DIR "/topologie/")

    foreach (TMP_PATH ${${PROJECT_NAME}_SOURCES})
        string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
        if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
            list (REMOVE_ITEM ${PROJECT_NAME}_SOURCES ${TMP_PATH})
        endif ()
    endforeach(TMP_PATH)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    file(GLOB_RECURSE ${PROJECT_NAME}_EXAMPLES "examples/*.cpp" "examples/**/*.cpp")
    add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_EXAMPLES})
else()
    file(GLOB_RECURSE ASSET_FILES "assets/build/*")
    foreach(FILE ${ASSET_FILES})
        get_filename_component(FILE_WE ${FILE} NAME)
        add_custom_command(
            OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.h
            COMMAND xxd -i ${FILE_WE} ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.h
            DEPENDS ${FILE}
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/assets/build
        )
        list(APPEND GENERATED_HEADERS ${CMAKE_CURRENT_BINARY_DIR}/${FILE_WE}.h)
    endforeach()
    add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} ${GENERATED_HEADERS} main.cpp)
endif()

if(APPLE)
        target_link_libraries(${PROJECT_NAME} "-framework WebKit")
elseif (UNIX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    pkg_check_modules(WEBKIT2GTK4 REQUIRED webkit2gtk-4.0)
    include_directories(${GTK3_INCLUDE_DIRS} ${WEBKIT2GTK4_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${GTK3_LIBRARIES} ${WEBKIT2GTK4_LIBRARIES})
elseif (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE -mwindows -Ilibs/webview2/build/native/include -ladvapi32 -lole32 -lshell32 -lshlwapi -luser32 -lversion)
endif()

###############################################################################

# target definitions

if(GITHUB_ACTIONS)
  message("NOTE: GITHUB_ACTIONS defined")
  target_compile_definitions(${PROJECT_NAME} PRIVATE GITHUB_ACTIONS)
endif()

###############################################################################

if(WARNINGS_AS_ERRORS)
    set_property(TARGET ${PROJECT_NAME} PROPERTY COMPILE_WARNING_AS_ERROR ON)
endif()

# custom compiler flags
message("Compiler: ${CMAKE_CXX_COMPILER_ID} version ${CMAKE_CXX_COMPILER_VERSION}")

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive- /wd4244 /wd4267 /wd4996 /external:anglebrackets /external:W0)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -pedantic)
endif()

target_compile_options(${PROJECT_NAME} PRIVATE -g)
###############################################################################

# sanitizers
set_custom_stdlib_and_sanitizers(${PROJECT_NAME} true)

###############################################################################

target_link_directories(${PROJECT_NAME} PRIVATE ${webview_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} SYSTEM PRIVATE ${webview_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)

# use SYSTEM so cppcheck/clang-tidy does not report warnings from these directories

# target_link_directories(${PROJECT_NAME} PRIVATE ${<SomeLib>_BINARY_DIR}/lib)
# target_link_libraries(${PROJECT_NAME} <SomeLib>)

###############################################################################

# copy binaries to "bin" folder; these are uploaded as artifacts on each release
# update name in .github/workflows/cmake.yml:29 when changing "bin" name here
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
# install(DIRECTORY some_dir1 some_dir2 DESTINATION bin)
# install(FILES some_file1.txt some_file2.md DESTINATION bin)