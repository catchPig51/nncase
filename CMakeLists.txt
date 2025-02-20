﻿cmake_minimum_required(VERSION 3.13)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake/Modules)

if(NOT DEFINED NNCASE_VERSION)
    set(NNCASE_VERSION "1.0.0")
endif()

if(DEFINED ENV{NNCASE_VERSION_SUFFIX})
    set(NNCASE_VERSION_SUFFIX $ENV{NNCASE_VERSION_SUFFIX})
endif()

if(NOT DEFINED NNCASE_VERSION_SUFFIX)
    find_package (Git)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} describe --always --dirty
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_DESC
        OUTPUT_STRIP_TRAILING_WHITESPACE)
    set(NNCASE_VERSION_SUFFIX "-${GIT_DESC}")
endif()

if (NOT PACKAGE_VERSION)
    set(PACKAGE_VERSION
        "${NNCASE_VERSION}${NNCASE_VERSION_SUFFIX}")
endif()

project(nncase
  VERSION ${NNCASE_VERSION}
  LANGUAGES C CXX ASM)

option(ENABLE_OPENMP "OpenMP support" ON)
option(ENABLE_HALIDE "halide kernels support" ON)
option(BUILD_PYTHON_BINDING "Build python binding" ON)
option(BUILD_BENCHMARK "Build benchmark programs" ON)
option(BUILD_TESTING "Build test programs" OFF)
option(ENABLE_OP_PROFILE "Profile ops cast time" OFF)
if (ENABLE_OP_PROFILE)
    add_definitions(-DENABLE_OP_PROFILE)
endif()

if (BUILDING_RUNTIME)
    option(ENABLE_VULKAN_RUNTIME "Enable Vulkan runtime" ON)
    option(ENABLE_K210_RUNTIME "Enable k210 runtime" OFF)
    option(DEFAULT_BUILTIN_RUNTIMES "Use default builtin runtimes" ON)
    option(DEFAULT_SHARED_RUNTIME_TENSOR_PLATFORM_IMPL "Use default shared memory platform impl" ON)
endif()

include(cmake/configure-conan.cmake)
include(cmake/conan.cmake)

if(NOT CONAN_EXPORTED)
    conan_check()
    conan_add_remote(NAME sunnycase URL https://conan.sunnycase.moe INDEX 0)
endif()

if(CONAN_EXPORTED) # in conan local cache
    message(STATUS "Standard Conan Installation")
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup() # NOTE need manmul set cppstd in conanfile.py
else() # in user space
    message(STATUS "Auto Cmake Conan Installation")
    include(${CMAKE_SOURCE_DIR}/cmake/conan.cmake)
    conan_cmake_run(CONANFILE conanfile.py
                    BASIC_SETUP
                    OPTIONS ${CONAN_OPTS}
                    SETTINGS ${CONAN_SETTINGS}
                    BUILD missing)
endif()

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
include(cmake/dependencies.cmake)

if (BUILDING_RUNTIME)
    set(NNCASE_MAIN_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
    set(NNCASE_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
    set(THIRD_PARTY ${CMAKE_CURRENT_LIST_DIR}/third_party)
    set_property(GLOBAL PROPERTY POSITION_INDEPENDENT_CODE ON)

    if (MSVC)
        add_definitions(/D_CRT_SECURE_NO_WARNINGS /DNOMINMAX)
        add_compile_options(/wd4267 /wd4251 /wd4244 /FC /utf-8 /W3 /WX)
    else()
        add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-multichar -Wno-missing-field-initializers -Wno-unused-function -Wno-type-limits)
        if (APPLE)
            add_compile_options(-Wno-four-char-constants -Wno-sometimes-uninitialized)
        elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            add_compile_options(-Wno-uninitialized -Wno-unused-private-field -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function)
        else ()
            add_compile_options(-Wno-maybe-uninitialized -Wno-unused-private-field  -Wno-unused-variable -Wno-unused-parameter -Wno-unused-function)
        endif()
    endif()

    include_directories(${NNCASE_MAIN_INCLUDE_DIR})
    include_directories(${NNCASE_INCLUDE_DIR})

    add_subdirectory(include/nncase)
    add_subdirectory(src/kernels)
    add_subdirectory(src/runtime)
    add_subdirectory(src/functional)
    if(BUILD_BENCHMARK)
        add_subdirectory(benchmark)
    endif()

    # Python binding
    if(BUILD_PYTHON_BINDING)
        add_subdirectory(python/nncaseruntime/native)
    endif()

    install(DIRECTORY ${NNCASE_INCLUDE_DIR}/nncase
        DESTINATION include
        COMPONENT nncase-headers
        FILES_MATCHING
        PATTERN "*version.h"
        PATTERN "CMakeFiles" EXCLUDE
        )

    install(DIRECTORY include/nncase/kernels
        DESTINATION include/nncase
        COMPONENT nncase-headers
        FILES_MATCHING
        PATTERN "*.def"
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "*.td"
        PATTERN "*.inc"
        PATTERN "LICENSE.TXT"
        )

    install(DIRECTORY include/nncase/runtime
        DESTINATION include/nncase
        COMPONENT nncase-headers
        FILES_MATCHING
        PATTERN "*.def"
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "*.td"
        PATTERN "*.inc"
        PATTERN "LICENSE.TXT"
        )
else()
    set(CMAKE_SKIP_RPATH OFF)

    set(NNCASE_MAIN_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR}/include)
    set(NNCASE_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/include)
    set(THIRD_PARTY ${CMAKE_CURRENT_LIST_DIR}/third_party)
    set_property(GLOBAL PROPERTY POSITION_INDEPENDENT_CODE ON)
    if (APPLE)
        set(CMAKE_MACOSX_RPATH TRUE)
        set(CMAKE_INSTALL_RPATH "@loader_path")
        set(CMAKE_INSTALL_NAME_DIR "@rpath")
    else ()
        set(CMAKE_INSTALL_RPATH "$ORIGIN")
    endif()

    if (MSVC)
        add_definitions(/D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS /D_CRT_SECURE_NO_WARNINGS /DNOMINMAX)
        add_compile_options(/wd4267 /wd4251 /wd4244 /FC /utf-8 /W3 /WX)
        set(PYBIND11_CPP_STANDARD "/std:c++latest")
    else()
        add_compile_options(-fvisibility=hidden)
        add_compile_options(-Wall -Wextra -pedantic -Werror -Wno-multichar -Wno-missing-field-initializers -Wno-unused-function -Wno-type-limits -Wno-unused-local-typedefs -Wno-sign-compare)
        if (APPLE)
            add_compile_options(-Wno-four-char-constants -Wno-sometimes-uninitialized -Wno-deprecated)
        elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
            add_compile_options(-Wno-uninitialized)
        else ()
            add_compile_options(-Wno-maybe-uninitialized -Wno-deprecated-copy)
            if (CMAKE_CXX_COMPILER_VERSION GREATER_EQUAL "12.0")
                add_compile_options(-Wno-array-bounds -Wno-deprecated-declarations -Wno-restrict)
            endif()
            add_link_options(-Wl,--exclude-libs,ALL)
        endif()
    endif()

    include_directories(${NNCASE_MAIN_INCLUDE_DIR})
    include_directories(${NNCASE_INCLUDE_DIR})

    add_subdirectory(include/nncase)
    add_subdirectory(src/nncase)
    add_subdirectory(src/data)
    add_subdirectory(src/ir)
    add_subdirectory(src/importer)
    add_subdirectory(src/schedule)
    add_subdirectory(src/evaluator)
    add_subdirectory(src/functional)
    add_subdirectory(src/transforms)
    add_subdirectory(src/codegen)
    add_subdirectory(src/kernels)
    add_subdirectory(src/runtime)
    add_subdirectory(src/targets)
    add_subdirectory(src/plugin)
    add_subdirectory(src/cli)

    if(BUILD_TESTING)
        add_subdirectory(tests/kernels)
    endif()

    # Python binding
    if(BUILD_PYTHON_BINDING)
        add_subdirectory(python/nncase/native)
    endif()

    # Thrid party
    add_subdirectory(third_party/onnx)

    install(DIRECTORY include/nncase
        DESTINATION include
        COMPONENT nncase-headers
        FILES_MATCHING
        PATTERN "*.def"
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "*.td"
        PATTERN "*.inc"
        PATTERN "LICENSE.TXT"
        )

    install(DIRECTORY ${NNCASE_INCLUDE_DIR}/nncase
        DESTINATION include
        COMPONENT nncase-headers
        FILES_MATCHING
        PATTERN "*.def"
        PATTERN "*.h"
        PATTERN "*.hpp"
        PATTERN "*.td"
        PATTERN "*.inc"
        PATTERN "CMakeFiles" EXCLUDE
        PATTERN "config.h" EXCLUDE
        )

    install(DIRECTORY python/nncase
        DESTINATION python
        COMPONENT nncase-python
        FILES_MATCHING
        PATTERN "*.py"
        )

    # Targets
    add_subdirectory(targets/cpu)
    add_subdirectory(targets/k210)
    add_subdirectory(targets/vulkan)
endif()

# Modules
add_subdirectory(modules/k210)
add_subdirectory(modules/vulkan)
