cmake_minimum_required(VERSION 3.27)
project(xbc_ai_indxer)

set(LIBRSX_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/sdk")
set(LIBRSX_LIB_DIR "${CMAKE_SOURCE_DIR}/sdk")
include_directories(${LIBRSX_INCLUDE_DIR})
link_directories(${LIBRSX_LIB_DIR})

set(CJSON_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/liba/cjson/cjson")
set(CJSON_LIB_DIR "${CMAKE_SOURCE_DIR}/liba/cjson")
include_directories(${CJSON_INCLUDE_DIR})
link_directories(${CJSON_LIB_DIR})

# LEVELDB
set(LEVELDB_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/liba/leveldb/leveldb")
set(LEVELDB_LIB_DIR "${CMAKE_SOURCE_DIR}/liba/leveldb")
include_directories(${LEVELDB_INCLUDE_DIR})
link_directories(${LEVELDB_LIB_DIR})

add_executable(indxer
        src/main.c
        src/lib/bt_tasks.c
        src/lib/bt_tasks.h
        src/lib/hashmap.c
        src/lib/hashmap.h
        src/lib/uthash.h
)

set(SOURCE_DIR "${CMAKE_SOURCE_DIR}/res")
set(DEST_DIR "${CMAKE_BINARY_DIR}/cnf")
add_custom_command(
        OUTPUT ${DEST_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${DEST_DIR}
)

add_custom_command(
        OUTPUT ${DEST_DIR}/routes.json ${DEST_DIR}/config.xbc
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SOURCE_DIR}/config.xbc ${DEST_DIR}/config.xbc
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SOURCE_DIR}/bt.conf ${DEST_DIR}/bt.conf
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SOURCE_DIR}/routes.json ${DEST_DIR}/routes.json
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SOURCE_DIR}/vs1984-btd ${CMAKE_BINARY_DIR}/vs1984-btd
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SOURCE_DIR}/vs1984-rag ${CMAKE_BINARY_DIR}/vs1984-rag
        DEPENDS ${SOURCE_DIR}/config.xbc ${SOURCE_DIR}/routes.json
        COMMENT "prepare files to build directory"
)

add_custom_target(
        PREPARE_COMPILE ALL
        DEPENDS ${DEST_DIR}/routes.json ${DEST_DIR}/config.xbc
)

target_link_directories(indxer PRIVATE
        ${CMAKE_SOURCE_DIR}/libs
)

set_target_properties(indxer PROPERTIES
        BUILD_RPATH "$ORIGIN/../libs"
        INSTALL_RPATH "$ORIGIN/../libs;$ORIGIN"
)

target_link_libraries(indxer PRIVATE xbc
        microhttpd
        pulse
        asound
        jack
        pthread dl
        m z png stdc++)

add_dependencies(indxer PREPARE_COMPILE)