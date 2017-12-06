set(HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/column_info.hpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/data_reader.hpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/deprecation_warning.hpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/image_channel_info.hpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/image_info.hpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/image_reader.hpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/spreadsheet_reader.hpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/strutils.hpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/exceptions.hpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/parsers.hpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/formatters.hpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/container_io_config.hpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/windows.hpp
                 )

set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/column_info.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/data_reader.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/image_info.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/image_reader.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/spreadsheet_reader.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/exceptions.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/container_io_config.cpp
                 ${CMAKE_CURRENT_SOURCE_DIR}/strutils.cpp)
                 
set(PNIIO_HEADER_FILES )
set(PNIIO_SOURCE_FILES )
                 
install(FILES ${HEADER_FILES}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pni/io)


add_subdirectory("cbf")
add_subdirectory("fio")
add_subdirectory("nx")
add_subdirectory("parsers")
add_subdirectory("formatters")
add_subdirectory("tiff")

set(PNIIO_HEADER_FILES ${PNIIO_HEADER_FILES} ${HEADER_FILES})
set(PNIIO_SOURCE_FILES ${PNIIO_SOURCE_FILES} ${SOURCE_FILES})

add_library(pniio SHARED ${PNIIO_SOURCE_FILES} ${PNIIO_HEADER_FILES})

target_include_directories(pniio
PRIVATE ${PROJECT_SOURCE_DIR}/src
INTERFACE
$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>
$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
#add_dependencies(pniio config.hpp)
set_target_properties(pniio PROPERTIES VERSION "${LIBRARY_VERSION}"
                                       SOVERSION "${SO_VERSION}")

target_link_libraries(pniio
                      PUBLIC pnicore_shared h5cpp_shared
                      ${HDF5_LIBRARIES}
                      Boost::filesystem
                      Boost::regex
                      Boost::date_time
                      )
target_compile_definitions(pniio PUBLIC BOOST_ALL_DYN_LINK)

#install code
install(TARGETS pniio
        EXPORT pniio_targets
        LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
        RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
        COMPONENT libraries
        )

install(EXPORT pniio_targets
DESTINATION ${CMAKE_INSTALL_PACKAGEDIR}
COMPONENT development)