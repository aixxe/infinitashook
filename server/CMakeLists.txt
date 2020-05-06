cmake_minimum_required(VERSION 3.13)
project(infinitashook)

# default to windows 7 if CMAKE_SYSTEM_VERSION isn't set, mostly a cross-compiling thing
# e.g. https://aur.archlinux.org/cgit/aur.git/tree/toolchain-mingw.cmake?h=mingw-w64-cmake
if (NOT CMAKE_SYSTEM_VERSION)
    set (CMAKE_SYSTEM_VERSION 7.0 CACHE INTERNAL "")
endif()

set(SOURCE_FILES
    src/core/bm2dx/bm2dx.cc
    src/core/bm2dx/bm2dx_util.cc
    src/core/event_server.cc
    src/event/score_event.cc
    src/hook/result_hook.cc
    src/message/base_message.cc
    src/message/input_message.cc
    src/message/score_message.cc
    src/context.cc
    src/game_environment.cc
    src/infinitashook.cc
    src/input_poll.cc
)

set(BUILD_SHARED_LIBS OFF)
set(BUILD_TESTS OFF)
set(WITH_DOCS OFF)
set(WITH_PERF_TOOL OFF)
set(ZMQ_BUILD_TESTS OFF)

add_subdirectory(external/MinHook EXCLUDE_FROM_ALL)
add_subdirectory(external/libzmq EXCLUDE_FROM_ALL)

add_library(infinitashook SHARED ${SOURCE_FILES})

set_target_properties(infinitashook PROPERTIES PREFIX "")

if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(infinitashook PRIVATE -s -static -Wl,--gc-sections -Wl,--exclude-all-symbols)
endif()

target_link_libraries(infinitashook minhook libzmq-static)
target_compile_features(infinitashook PRIVATE cxx_std_20)
target_include_directories(infinitashook PRIVATE external/cppzmq external/json/include)
target_compile_definitions(infinitashook PRIVATE ZMQ_STATIC)