cmake_minimum_required(VERSION 3.23)
project(TuneZW)

set(CMAKE_CXX_STANDARD 17)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CMake")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Frontend/public/out/ws)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(CMake)
add_subdirectory(Source)
add_subdirectory(Source/TuneZ/WASM)
include_directories(${CMAKE_SOURCE_DIR}/Source/vendor)
include_directories(${CMAKE_SOURCE_DIR}/Source/TuneZ)
add_sources(main.cpp)
add_external(main shutdown run)

add_executable(${PROJECT_NAME} ${SRCS})

foreach(SRC_ IN LISTS SRCS)
    string(REPLACE "Source/" "" SRC_NAME "${SRC_}")
    string(REPLACE "/" "-" SRC_NAME "${SRC_NAME}")
    set_source_files_properties(${SRC_} PROPERTIES COMPILE_OPTIONS "-MJ${CMAKE_SOURCE_DIR}/FlagDir/${SRC_NAME}.o.json")
endforeach()

set(CMAKE_EXECUTABLE_SUFFIX ".js")
list(JOIN EXTERNAL "," EMC_EXTERNAL)
# PREPARE EMSCRIPTEN
linker_flags(
        -03
        -s WASM=1
        -s USE_SDL=0
        -s USE_WEBGL2=1
        -s USE_GLFW=3
        -s MODULARIZE=1
        -s EXPORT_NAME="TuneZ"
        -s ALLOW_MEMORY_GROWTH=1
        -s MIN_WEBGL_VERSION=2
        -s MAX_WEBGL_VERSION=2
        -s FULL_ES3=1
        -s ASYNCIFY
        -s FORCE_FILESYSTEM=1
        -flto
        --bind
        -g2
        -s EXPORTED_FUNCTIONS=${EMC_EXTERNAL}
        -s EXPORTED_RUNTIME_METHODS=FS,ccall,cwrap
)

list(JOIN EMC_FLAGS " " LINKER_FLAGS)
set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "${LINKER_FLAGS}")
target_link_libraries(${PROJECT_NAME} glfw3 GL)

ADD_CUSTOM_COMMAND(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND "${CMAKE_SOURCE_DIR}/compile-commands-gen.sh")
#file(COPY ${CMAKE_BINARY_DIR}/compile_commands.json DESTINATION ${CMAKE_SOURCE_DIR})