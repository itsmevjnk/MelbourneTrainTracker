set(ASSETS_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/assets")
set(ASSETS_BUILD_DIR "${CMAKE_BINARY_DIR}/assets")

file(GLOB_RECURSE COMPRESS_SRC "${ASSETS_SRC_DIR}/*.htm" "${ASSETS_SRC_DIR}/*.svg" "${ASSETS_SRC_DIR}/*.css" "${ASSETS_SRC_DIR}/*.dat")

# Generate list of expected output files
set(COMPRESSED_ASSETS "")
foreach(src_file ${COMPRESS_SRC})
    file(RELATIVE_PATH rel_file "${ASSETS_SRC_DIR}" "${src_file}")
    list(APPEND COMPRESSED_ASSETS "${ASSETS_BUILD_DIR}/${rel_file}.gz")
endforeach()

idf_component_register(
    SRC_DIRS "src" "src/config_cli" "src/lsid" "src/subsystems" "src/utils" "src/subsystems/endpoints" "src/subsystems/endpoints/config"
    INCLUDE_DIRS "include"
    EMBED_FILES ${COMPRESSED_ASSETS}
)

# Custom commands must be after idf_component_register in ESP-IDF
add_custom_command(
    OUTPUT "${ASSETS_BUILD_DIR}/processing.stamp" ${COMPRESSED_ASSETS}
    COMMAND ${Python3_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/gzip_compress.py" "-o" "${ASSETS_BUILD_DIR}" "${ASSETS_SRC_DIR}"
    COMMAND ${CMAKE_COMMAND} -E touch "${ASSETS_BUILD_DIR}/processing.stamp"
    DEPENDS ${COMPRESS_SRC} "${CMAKE_CURRENT_SOURCE_DIR}/gzip_compress.py"
    COMMENT "Compressing assets"
    VERBATIM
)

add_custom_target(compress_assets ALL DEPENDS "${ASSETS_BUILD_DIR}/processing.stamp")
add_dependencies(${COMPONENT_LIB} compress_assets)
