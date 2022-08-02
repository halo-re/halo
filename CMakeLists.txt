project(halo)
cmake_minimum_required(VERSION 3.16)

find_package(PythonInterp)

set(gen_dir "${PROJECT_BINARY_DIR}/generated")
add_custom_target(create_gen_dir ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory ${gen_dir})

add_custom_command(
    OUTPUT "${gen_dir}/decl.h" "${gen_dir}/halo.xbe.def" "${gen_dir}/thunks.c"
    COMMAND
        "${PYTHON_EXECUTABLE}"
        "${CMAKE_CURRENT_LIST_DIR}/tools/knowledge.py"
		--gen-header "${gen_dir}/decl.h"
		--gen-def "${gen_dir}/halo.xbe.def"
		--gen-thunks "${gen_dir}/thunks.c"
    DEPENDS "${CMAKE_CURRENT_LIST_DIR}/tools/knowledge.py" create_gen_dir
)

if (MSVC)
	set(lib_tool "lib")
else()
	set(lib_tool "lld-link")
endif()

add_custom_command(
	OUTPUT "${gen_dir}/halo.xbe.lib"
	COMMAND
		${lib_tool} /def:${gen_dir}/halo.xbe.def /machine:x86 /out:${gen_dir}/halo.xbe.lib
	DEPENDS "${gen_dir}/halo.xbe.def" create_gen_dir
	VERBATIM
)

add_custom_command(
	OUTPUT "${gen_dir}/xboxkrnl.exe.lib"
	COMMAND
		${lib_tool} /def:${CMAKE_SOURCE_DIR}/src/xboxkrnl.exe.def /machine:x86 /out:${gen_dir}/xboxkrnl.exe.lib
	DEPENDS create_gen_dir
	VERBATIM
)

add_custom_command(
	OUTPUT "${gen_dir}/build_info.c" "${gen_dir}/.force"
	COMMAND
		"${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_LIST_DIR}/tools/gen-build-info.py" > ${gen_dir}/build_info.c
	DEPENDS create_gen_dir
	VERBATIM
)

add_custom_command(
	OUTPUT "${gen_dir}/typechecks.c"
	COMMAND
		"${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_LIST_DIR}/tools/gen-struct-checks.py" "${CMAKE_SOURCE_DIR}/src/types.h" > ${gen_dir}/typechecks.c
	DEPENDS ${CMAKE_SOURCE_DIR}/src/types.h create_gen_dir
	VERBATIM
)

set(sources
	src/entrypoint.c
	src/halo/cseries/cseries.c
	src/halo/game/game.c
	src/halo/game/game_engine.c
	src/halo/game/game_time.c
	src/halo/game/players.c
	src/halo/input/input_xbox.c
	src/halo/interface/hud.c
	src/halo/interface/player_ui.c
	src/halo/interface/ui_widget.c
	src/halo/interface/ui_widget_game_data_input_functions.c
	src/halo/main/main.c
	src/halo/memory/data.c
	src/halo/render/render.c
	src/halo/saved_games/game_state.c
	src/halo/shell_xbox.c
	${gen_dir}/thunks.c
	${gen_dir}/build_info.c
	${gen_dir}/typechecks.c
	)

include_directories(${gen_dir} ${CMAKE_SOURCE_DIR}/src)

if (MSVC)
	if(CMAKE_SIZEOF_VOID_P EQUAL 8)
		message(FATAL_ERROR "You are attempting to build for 64 bit. Delete the build directory, then specify -AWin32 when configuring with CMake to build for 32 bit.")
	endif()
	set(CMAKE_C_FLAGS "/FI\"${CMAKE_SOURCE_DIR}/src/common.h\" /GS- /DMSVC")
	set(CMAKE_EXE_LINKER_FLAGS "/entry:_start /nodefaultlib:msvcrtd.lib /nodefaultlib:msvcrt.lib /incremental:no")
else()
	set(CMAKE_C_FLAGS "-Wall -Werror -target i386-pc-win32 -march=pentium3 -nostdlib -ffreestanding -fno-builtin -fno-exceptions -I${CMAKE_SOURCE_DIR}/src -include ${CMAKE_SOURCE_DIR}/src/common.h")
	set(CMAKE_EXE_LINKER_FLAGS "-fuse-ld=lld-link -Xlinker -entry:_start")
endif()

add_executable(halo ${sources})

set(import_libs ${gen_dir}/halo.xbe.lib ${gen_dir}/xboxkrnl.exe.lib)
target_link_libraries(halo ${import_libs})
add_custom_target(import_libs_target DEPENDS ${import_libs})
add_dependencies(halo import_libs_target)

add_custom_target(
	patched_xbe ALL
	DEPENDS "$<TARGET_FILE:halo>"
	BYPRODUCTS ${CMAKE_CURRENT_LIST_DIR}/halo-patched/default.xbe
	COMMAND
		"${PYTHON_EXECUTABLE}" "${CMAKE_CURRENT_LIST_DIR}/tools/patch.py"
			"${CMAKE_CURRENT_LIST_DIR}/halo-patched/cachebeta.xbe"
			"$<TARGET_FILE:halo>"
			"${CMAKE_CURRENT_LIST_DIR}/halo-patched/default.xbe"
	VERBATIM
	)
