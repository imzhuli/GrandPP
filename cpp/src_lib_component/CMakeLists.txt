cmake_minimum_required(VERSION 3.14)
set(libname ppcomponent)
project (lib_${libname})

include_directories(
	../src_lib_common/
	${LOCAL_3RD_INCLUDE_DIR}
	${X_INCLUDE_DIRS}
)

# local libs
aux_source_directory("./pp_component"   SRC)
add_library(${libname} ${SRC})

install(
	DIRECTORY ${PROJECT_SOURCE_DIR}/
	DESTINATION include
	FILES_MATCHING
		PATTERN "*.h"
		PATTERN "*.hpp"
	)

install(
	TARGETS
	${libname}
	RUNTIME DESTINATION bin
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
)

