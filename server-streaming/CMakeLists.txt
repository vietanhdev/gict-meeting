#Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Specify project title
project(StreamServer)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# To automatically run MOC when building (Meta Object Compiler)
set(CMAKE_AUTOMOC ON)

# To automatically run UIC when building (User Interface Compiler)
set(CMAKE_AUTOUIC ON)

# To automatically run RCC when building (Resource Compiler)
set(CMAKE_AUTORCC ON)

# Specify OpenCV folder, and take care of dependencies and includes
#set(OpenCV_DIR "path_to_opencv")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

find_package(SDL2 REQUIRED sdl2)


find_package( Threads )

include_directories(
    "src"
    "src/utilities"

    "src/services"
    "src/services/data_types"
    "src/services/protocols"
    "src/services/server_socket"
    "src/services/stream"
    "src/services/video"

    ${SDL2_INCLUDE_DIRS}
    "src/yaml-cpp/include"
)

# add required source, header, ui and resource files
add_executable(${PROJECT_NAME} 
    "src/main.cpp"

    "src/utilities/utility.cpp"
    "src/utilities/file_storage.cpp"
    "src/utilities/timer.cpp"

    "src/services/data_types/conference.cpp"
    "src/services/data_types/participant.cpp"

    "src/services/stream/video_stream_service.cpp"
    "src/services/protocols/video_frame_protocol.cpp"

    "src/services/stream/audio_stream_service.cpp"
    "src/services/protocols/audio_frame_protocol.cpp"

    "src/services/server_socket/server_socket.cpp"

    "src/services/video/video_capture.cpp"
    "src/services/video/video_frame.cpp"
)


# Use C++ 17
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

if (CMAKE_COMPILER_IS_GNUCC AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9)
    set (CPP_FS_LIB "stdc++fs")
endif()

# yaml-cpp library
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "disable yaml tests")
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "disable yaml tools")
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "disable yaml contrib")
add_subdirectory(src/yaml_cpp)

# link required libs
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS} ${CPP_FS_LIB} ${SDL2_LIBRARIES} yaml-cpp ${CMAKE_THREAD_LIBS_INIT})