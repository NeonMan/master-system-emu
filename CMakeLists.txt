# Main
cmake_minimum_required (VERSION 2.8)

# --- Project name ---
project (SMS-Emu)

# --- Explicitly enable C
enable_language (C)
enable_language (CXX)
enable_testing()

# --- Tell build type
MESSAGE( STATUS "CMAKE_BUILD_TYPE: " ${CMAKE_BUILD_TYPE} )

# --- Enable C99 on GCC
if (CMAKE_COMPILER_IS_GNUCC)
	#Add code coverage on GNU debug builds
	if(CMAKE_BUILD_TYPE STREQUAL "Debug")
		MESSAGE( STATUS "Activating coverage tooling")
		MESSAGE( STATUS "Run `make cov` after build to create coverage report")
		#Set flags
		SET(CMAKE_CXX_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
		SET(CMAKE_C_FLAGS "-g -O0 -fprofile-arcs -ftest-coverage")
		#Add a targets for coverage reporting
		add_custom_target(cov_clear
			COMMAND lcov -q --zerocounters  --directory "${CMAKE_BINARY_DIR}"
			COMMENT "Clearing coverage logs"
		)
		add_custom_target(cov_report
			COMMAND lcov -q --directory . --capture --output-file "coverage.info"
			COMMAND genhtml -q --output-directory "coverage" 
				--demangle-cpp --num-spaces 2 --sort 
				--title "${PROJECT_NAME} Test Coverage" 
				--function-coverage --branch-coverage --legend 
				"coverage.info"
			COMMENT "Making coverage report"
		)
		add_custom_target(cov
			COMMAND make cov_clear
			COMMAND make test
			COMMAND make cov_report
		)
	endif()
	
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
	SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wno-unused-variable -Wno-unused-function -Wno-strict-aliasing")
	#SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Werror")
endif (CMAKE_COMPILER_IS_GNUCC)

# --- Setup C++
if (CMAKE_COMPILER_IS_GNUCXX)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-unused-variable -Werror -std=c++11")
endif (CMAKE_COMPILER_IS_GNUCXX)

# --- MSVC configs
if (MSVC)
    #Tell MSVC to shut the fuck up about _CRT_SECURE_NO_WARNINGS.
    add_definitions( -D_CRT_SECURE_NO_WARNINGS )
    #Make it a little more verbose and set warnings-as-errors.
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W3 /WX")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W3 /WX")
endif (MSVC)

# --- Enable OpenMP if available
if(NOT MINGW)
	find_package(OpenMP)
	if (OPENMP_FOUND)
		set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
		set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	endif(OPENMP_FOUND)
endif(NOT MINGW)

# --- Find python3 (code generation) ---
find_package(PythonInterp 3 REQUIRED)

# --- Dependencies ---
include_directories(${PROJECT_SOURCE_DIR})
add_subdirectory (glue)
add_subdirectory (psg)
add_subdirectory (ram)
add_subdirectory (rom)
add_subdirectory (z80)
add_subdirectory (z80dasm)
add_subdirectory (peripheral)
add_subdirectory (io)
add_subdirectory (vdp)
add_subdirectory (sdsc)
add_subdirectory (test)
add_subdirectory (sms-ui)

# --- The project executables ---

SET(EXT_LIBS SDL2main SDL2)
if (MINGW)
	ADD_DEFINITIONS(-DWIN32)
	SET(EXT_LIBS mingw32 ${EXT_LIBS})
endif()
# --- psg-play
add_executable (psg-play psg-play.c)
target_link_libraries (psg-play psg ${EXT_LIBS})

# --- test-vdp (SDL2 is presumed to be available)
add_executable (test-vdp test-vdp.c)
target_link_libraries (test-vdp vdp ${EXT_LIBS})

# --- sms-emu
add_executable (emu sms-emu.c)
target_link_libraries (emu glue smsrom smsio smsram psg z80 peripherial vdp sdsc ${EXT_LIBS})
