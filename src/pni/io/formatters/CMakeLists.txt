set(HEADER_FILES ${CMAKE_CURRENT_SOURCE_DIR}/scalar_format.hpp 
                 ${CMAKE_CURRENT_SOURCE_DIR}/vector_format.hpp)
                
set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/scalar_format.cpp 
			     ${CMAKE_CURRENT_SOURCE_DIR}/vector_format.cpp)

install(FILES ${HEADER_FILES}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/pni/io/formatters)

set(PNIIO_HEADER_FILES ${PNIIO_HEADER_FILES} ${HEADER_FILES} PARENT_SCOPE)
set(PNIIO_SOURCE_FILES ${PNIIO_SOURCE_FILES} ${SOURCE_FILES} PARENT_SCOPE)