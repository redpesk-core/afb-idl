cmake_minimum_required(VERSION 3.10)

project(
	afb-fbgen
	DESCRIPTION "afb-fbgen generate afb api baser on flatbuffers schema"
)

#### afb-fbgen ####
set(AFB_FBGEN_SH ${CMAKE_CURRENT_SOURCE_DIR}/afb-fbgen)
set(FB_TEMPLATES
	${CMAKE_CURRENT_SOURCE_DIR}/fb-template-api.c
	${CMAKE_CURRENT_SOURCE_DIR}/fb-template-cli.c
	${CMAKE_CURRENT_SOURCE_DIR}/fb-template-cvt.c
	${CMAKE_CURRENT_SOURCE_DIR}/fb-template-cvt.h
	${CMAKE_CURRENT_SOURCE_DIR}/fb-template-CMakeLists.txt

)

#### reflection json printer ####
set(REFLECTION_HEADER reflection_json_printer.h)
set(REFLECTION_SCHEMA ${CMAKE_CURRENT_SOURCE_DIR}/reflection.fbs)

# Check for flatcc generator
find_program(FLATCC flatcc)

add_custom_command(
	OUTPUT ${REFLECTION_HEADER}
	DEPENDS ${REFLECTION_SCHEMA}
	COMMAND ${FLATCC} --json-printer --outfile=${REFLECTION_HEADER} ${REFLECTION_SCHEMA}
)

add_custom_target(
    reflection_generated_header
    DEPENDS ${REFLECTION_HEADER}
)

#### bfbs2json ####
add_executable(bfbs2json bfbs2json.c)
target_link_libraries(bfbs2json flatccrt)
add_dependencies(bfbs2json reflection_generated_header)
target_include_directories(bfbs2json PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

#### instalation ####
include(GNUInstallDirs)
install(TARGETS bfbs2json RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_LIBEXECDIR})
install(FILES ${FB_TEMPLATES} DESTINATION ${CMAKE_INSTALL_FULL_DATAROOTDIR}/afb-idl/afb-flatbuffers)
install(PROGRAMS ${AFB_FBGEN_SH} DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
