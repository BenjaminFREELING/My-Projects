cmake_minimum_required(VERSION 3.7)

project(tetris LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)

function(ADD_PRG)

	set(options OPTIONAL)
	set(oneValueArgs TARGET )
	set(multiValueArgs SOURCES)
	cmake_parse_arguments(ADD_PRG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

	add_executable(${ADD_PRG_TARGET} "")
	target_sources(${ADD_PRG_TARGET} PUBLIC ${ADD_PRG_SOURCES})
	if (${CMAKE_SYSTEM_NAME} MATCHES Linux)
		target_include_directories(${ADD_PRG_TARGET} PUBLIC /usr/include/SDL2)
		target_compile_options(${ADD_PRG_TARGET} PUBLIC "-lSDL_mixer" "-lSDL_ttf")
		target_link_libraries(${ADD_PRG_TARGET} PUBLIC SDL2 SDL2main SDL2_mixer SDL2_ttf)
	else()
		target_link_libraries(${ADD_PRG_TARGET} PUBLIC SDL2::SDL2 SDL2::SDL2main)
	endif()
	target_compile_definitions(${ADD_PRG_TARGET} PUBLIC _USE_MATH_DEFINES)

endfunction()


ADD_PRG(TARGET Tetris SOURCES shapes.h piece.h piece.cpp board.h board.cpp game.h game.cpp main.cpp window.inl window.h window.cpp)	

