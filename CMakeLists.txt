# 2018-2020, Spontz

cmake_minimum_required(VERSION 3.8)
set(CMAKE_TOOLCHAIN_FILE "../vcpkg/scripts/buildsystems/vcpkg.cmake")
project(physfs_test)
set(CMAKE_CXX_STANDARD 20)

if (MSVC)
	add_compile_options(/MP)
	add_compile_options(-bigobj)
	add_compile_options(
		"$<$<CONFIG:Debug>:/MTd>"
		"$<$<CONFIG:RelWithDebInfo>:/MT>"
		"$<$<CONFIG:Release>:/MT>"
		"$<$<CONFIG:MinSizeRel>:/MT>"
	)
endif()

# Hide console and allow main() to be the entry point
#set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")

# HACK: Hide some warnings
add_definitions(-D_CRT_SECURE_NO_WARNINGS)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.c ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/src/*.h ${CMAKE_SOURCE_DIR}/src/*.hpp ${CMAKE_SOURCE_DIR}/src/*.ipp ${CMAKE_SOURCE_DIR}/src/*.inl)
file(GLOB_RECURSE RESOURCE_FILES ${CMAKE_SOURCE_DIR}/res/*.rc)

# Dependencies
find_package(PhysFS CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${RESOURCE_FILES})
if(MSVC)
	set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE PhysFS::PhysFS)

message($CMAKE_BUILD_TYPE)

include_directories("${CMAKE_SOURCE_DIR}/src" "${CMAKE_SOURCE_DIR}/include")

# Set project working dir
set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})

# Declare Visual Studio project virtual folders
if(MSVC_IDE)
	# Macro to preserve source files hierarchy in the IDE
	macro(GroupSources curdir)
		file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)

		foreach(child ${children})
			if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
				GroupSources(${curdir}/${child})
			else()
				string(REPLACE "/" "\\" groupname ${curdir})
				string(REPLACE "src" "Sources" groupname ${groupname})
				source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
			endif()
		endforeach()
	endmacro()

	# Run macro
	GroupSources(src)
endif()
