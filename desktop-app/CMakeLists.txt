#Specify the minimum version of CMake
cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

# Specify project title
project(VideoConf)

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

# Take care of Qt dependencies
find_package(Qt5 COMPONENTS Core Gui Widgets Test Concurrent REQUIRED)

# The Qt5Widgets_INCLUDES also includes the include directories for
# dependencies QtCore and QtGui
include_directories(${Qt5Widgets_INCLUDES})

# We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
add_definitions(${Qt5Widgets_DEFINITIONS})

# Executables fail to build with Qt 5 in the default configuration
# without -fPIE. We add that here.
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")

find_package( Threads )

include_directories(
    "src"
    "src/utilities"

    "src/services"
    "src/services/data_types"
    "src/services/protocols"
    "src/services/client_socket"
    "src/services/stream"
    "src/services/video"

    "src/gui"
    "src/gui/framelesswindow"
    ${SDL2_INCLUDE_DIRS}
    "src/yaml-cpp/include"
)

# add required source, header, ui and resource files
add_executable(${PROJECT_NAME} 
    "src/main.cpp"

    "src/utilities/utility.cpp"
    "src/utilities/file_storage.cpp"
    "src/utilities/timer.cpp"

    "src/gui/mainwindow.cpp"
    "src/gui/mainwindow.ui"


    "resources.qrc"
    "src/gui/framelesswindow.qrc"
    "src/gui/darkstyle.qrc"
    "src/gui/DarkStyle.cpp"
    "src/gui/framelesswindow/framelesswindow.cpp"
    "src/gui/framelesswindow/windowdragger.cpp"
    "src/gui/framelesswindow/framelesswindow.ui"

    "src/gui/new_conference_wizard.cpp"
    "src/gui/new_conference_wizard.ui"

    "src/services/data_types/conference.cpp"
    "src/services/data_types/participant.cpp"

    "src/services/stream/video_stream_service.cpp"

    "src/services/protocols/video_frame_protocol.cpp"
    "src/services/client_socket/client_socket.cpp"

    "src/services/video/video_capture.cpp"
    "src/services/video/video_frame.cpp"
)

ADD_LIBRARY(
    ${PROJECTNAME} STATIC
    ${HEADER_FILES}
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
target_link_libraries(${PROJECT_NAME} ${Qt5Widgets_LIBRARIES} ${OpenCV_LIBS} ${CPP_FS_LIB} ${SDL2_LIBRARIES} yaml-cpp ${CMAKE_THREAD_LIBS_INIT})

# Copy files
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/images $<TARGET_FILE_DIR:${PROJECT_NAME}>/images)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/sounds $<TARGET_FILE_DIR:${PROJECT_NAME}>/sounds)
