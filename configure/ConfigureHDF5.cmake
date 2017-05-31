

if(PNIIO_CONAN_HDF5)

    find_program(CONAN conan)
    if(CONAN MATCHES "CONAN-NONFOUND")
        message(FATAL_ERROR "Could not find conan executable to install dependencies")
    endif()

    if(NOT EXISTS ${PROJECT_BINARY_DIR}/conan.cmake)
        message(STATUS "Downloading conan.cmake file from github")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
             ${PROJECT_BINARY_DIR}/conan.cmake)
    endif()

    include(${PROJECT_BINARY_DIR}/conan.cmake)

    conan_cmake_run(REQUIRES "hdf5/1.10.1@wintersb/stable"
                    BASIC_SETUP
                    BUILD missing)

endif()


if(hdf5_DIR)
    #look for a cmake package specified by the user or provided via the global
    #package registry

    message(STATUS "Try to find HDF5 cmake package ...")

    find_package(hdf5 CONFIG REQUIRED)

    set(hdf5_LIBRARIES hdf5::hdf5-shared)
    get_target_property(hdf5_INCLUDE_DIRS hdf5::hdf5-shared INTERFACE_INCLUDE_DIRECTORIES)
    get_target_property(hdf5_LIBRARY_DIRS hdf5::hdf5-shared LOCATION)
    get_filename_component(hdf5_LIBRARY_DIRS ${hdf5_LIBRARY_DIRS} DIRECTORY)

    link_directories(${hdf5_LIBRARY_DIRS})
    add_definitions(-DH5_BUILT_AS_DYNAMIC_LIB)
else()
    message(STATUS "Try to configure HDF5 manually via tools ...")
    find_package(HDF5 REQUIRED C)

    message(STATUS "HDF5 header files: ${HDF5_INCLUDE_DIRS}")
    message(STATUS "HDF5 libraries: ${HDF5_LIBRARIES}")

endif()

# add HDF5 include directories - we need them everywhere in the code
include_directories(${HDF5_INCLUDE_DIRS} ${hdf5_INCLUDE_DIRS})
