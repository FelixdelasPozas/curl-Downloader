cmake_minimum_required (VERSION 3.5.0)

project(Downloader)

# Version Number
set (VERSION_MAJOR 1)
set (VERSION_MINOR 5)
set (VERSION_PATCH 1)

# Find the QtWidgets library
find_package(Qt6 COMPONENTS Core Gui Widgets Multimedia Network)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

string(TIMESTAMP CURRENT_YEAR "%Y")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt6Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt6Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt6Mutimedia_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt6WinExtras_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt6Network_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -m64 -Wall -mwindows -municode -Wno-deprecated")


if (CMAKE_BUILD_TYPE MATCHES Debug)
  set(CORE_EXTERNAL_LIBS ${CORE_EXTERNAL_LIBS} ${QT_QTTEST_LIBRARY})
endif (CMAKE_BUILD_TYPE MATCHES Debug)

if(DEFINED MINGW)
  configure_file("${PROJECT_SOURCE_DIR}/main.rc.in" "${PROJECT_BINARY_DIR}/main.rc")
  configure_file("${PROJECT_SOURCE_DIR}/installer/script.iss.in" "${PROJECT_BINARY_DIR}/script.iss")
  set(SOURCE_FILES ${SOURCE_FILES} ${CMAKE_CURRENT_BINARY_DIR}/main.rc)
  set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff -o <OBJECT> -i <SOURCE>")
  enable_language(RC)
endif(DEFINED MINGW)

qt6_add_resources(RESOURCES
    resources/resources.qrc
)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}          # Generated .h files
  ${CMAKE_CURRENT_BINARY_DIR}  # For wrap/ui files
  )

set (SOURCE_FILES
  ${SOURCE_FILES}
  ${RESOURCES}
  main.cpp
  AboutDialog.cpp
  MainWindow.cpp
  AddItemDialog.cpp
  ConfigurationDialog.cpp
  ItemWidget.cpp
  Utils.cpp
  ConsoleOutputDialog.cpp
  external/QTaskBarButton.cpp
)
  
set (EXTERNAL_LIBRARIES
  Qt6::Core
  Qt6::Gui
  Qt6::Widgets
  Qt6::Multimedia
  Qt6::Network
)
  
add_executable(CurlDownloader WIN32 ${SOURCE_FILES})
target_link_libraries (CurlDownloader ${EXTERNAL_LIBRARIES})
