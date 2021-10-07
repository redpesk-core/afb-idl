###########################################################################
# Copyright (C) 2018-2020 IoT.bzh Company
#
# Author: José Bollo <jose.bollo@iot.bzh>
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
###########################################################################

cmake_minimum_required(VERSION 3.10)

project(afb-idl
	DESCRIPTION "Interface Description Language and tools for Micro-service Framework"
	VERSION 0.1
	LANGUAGES C
)
set(HOMEPAGE_URL "https://github.com/redpesk-core/afb-idl")

###########################################################################
option(WITH_FLATBUFFERS "include generation of flatbuffers" OFF)
option(WITHOUT_FLATBUFFERS "exclude generation of flatbuffers" OFF)
###########################################################################

include(FindPkgConfig)
pkg_check_modules(json-c REQUIRED json-c)
add_compile_options(${json-c_CFLAGS})
link_libraries(${json-c_LDFLAGS})

###########################################################################

set(utilsrc exprefs.c  getref.c  json2c.c)
add_executable(afb-genskel  main-genskel.c  ${utilsrc})
add_executable(afb-exprefs  main-exprefs.c  ${utilsrc})
add_executable(afb-json2c   main-json2c.c   ${utilsrc})

###########################################################################

include(GNUInstallDirs)
install(TARGETS afb-genskel RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
install(TARGETS afb-exprefs RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})
install(TARGETS afb-json2c  RUNTIME DESTINATION ${CMAKE_INSTALL_FULL_BINDIR})

###########################################################################
#### add afb-fbgen ####
if(NOT WITH_FLATBUFFERS AND NOT WITHOUT_FLATBUFFERS)
	find_program(FLATCC flatcc)
	if(FLATCC)
		message(STATUS "Check for flatcc: ${FLATCC}")
	else(FLATCC)
		message(STATUS "Check for flatcc: not found")
		set(WITHOUT_FLATBUFFERS ON)
	endif(FLATCC)
endif(NOT WITH_FLATBUFFERS AND NOT WITHOUT_FLATBUFFERS)
if(NOT WITHOUT_FLATBUFFERS)
	add_subdirectory(flatbuffers)
endif(NOT WITHOUT_FLATBUFFERS)
