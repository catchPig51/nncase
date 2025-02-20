find_package(mpark_variant REQUIRED)
find_package(gsl-lite REQUIRED)
if (ENABLE_OPENMP)
    find_package(OpenMP COMPONENTS CXX REQUIRED)
endif ()

if ((NOT BUILDING_RUNTIME) OR ENABLE_VULKAN_RUNTIME)
    find_package(Vulkan REQUIRED)
endif ()

if (NOT BUILDING_RUNTIME)
    find_package(flatbuffers REQUIRED)
    find_package(libzip REQUIRED)
    if(NOT CONAN_EXPORTED)
        set(FLATBUFFERS_FLATC_EXECUTABLE ${flatbuffers_LIB_DIRS}/../bin/flatc)
        set(LIBZIP_ZIPTOOL_EXECUTABLE ${libzip_zip_LIB_DIRS}/../bin/ziptool)
    endif()
    find_package(fmt REQUIRED)
    find_package(lyra REQUIRED)
    find_package(magic_enum REQUIRED)
    find_package(nlohmann_json REQUIRED)
    find_package(OpenCV REQUIRED)
    find_package(Protobuf REQUIRED)
    find_package(xtensor REQUIRED)
    find_package(spdlog REQUIRED)
    find_package(libzippp REQUIRED)
    find_package(inja REQUIRED)
    find_package(shaderc REQUIRED)
endif ()

if (BUILD_TESTING)
    find_package(GTest REQUIRED)
endif ()

if (ENABLE_HALIDE)
    find_package(hkg REQUIRED)
endif ()