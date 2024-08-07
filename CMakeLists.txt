cmake_minimum_required(VERSION 3.16)
project(Supertris LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)

include(FetchContent)
FetchContent_Declare(SFML
	GIT_REPOSITORY https://github.com/SFML/SFML.git
	GIT_TAG 2.6.x)
FetchContent_MakeAvailable(SFML)

# Add all source files
# TODO: check all listed
# TODO: glob?
add_executable(Supertris
	src/main.cpp
	src/renderer.h
	src/renderer.cpp
	src/board.h
	src/board.cpp
	src/piece.h
	src/piece.cpp
	src/constants.h
	src/utils.h
	src/bag.h
	src/inputhandler.h
	src/assethandler.h
	src/effects.h
	src/effects.cpp
	src/score.h
	src/config.h
)
target_link_libraries(Supertris PRIVATE sfml-graphics)
target_compile_features(Supertris PRIVATE cxx_std_17)

if(WIN32)
	add_custom_command(
		TARGET Supertris
	COMMENT "Copy OpenAL DLL"
	PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:Supertris>
	VERBATIM)
endif()

install(TARGETS Supertris)

# List all assets
set(ASSETS
	LiberationSans-Regular.ttf
)
# Copy over the assets folder
foreach(ASSET ${ASSETS})
	# Copy the asset into the build assets folder
	file(COPY "src/assets/${ASSET}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin/Debug/assets/")
	file(COPY "src/assets/${ASSET}" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/bin/Release/assets/")
endforeach()
