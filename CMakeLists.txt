cmake_minimum_required (VERSION 3.5.0)

project(Downloader)

# Version Number
set (VERSION_MAJOR 0)
set (VERSION_MINOR 1)
set (VERSION_PATCH 0)

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Core Gui Widgets Multimedia Network WinExtras)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Mutimedia_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5WinExtras_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++20 -m64 -Wall -mconsole -municode -Wno-deprecated")

if (CMAKE_BUILD_TYPE MATCHES Debug)
  set(CORE_EXTERNAL_LIBS ${CORE_EXTERNAL_LIBS} ${QT_QTTEST_LIBRARY})
endif (CMAKE_BUILD_TYPE MATCHES Debug)

if(DEFINED MINGW)
  configure_file("${PROJECT_SOURCE_DIR}/main.rc.in" "${PROJECT_BINARY_DIR}/main.rc")
  set(SOURCE_FILES ${SOURCE_FILES} ${CMAKE_CURRENT_BINARY_DIR}/main.rc)
  set(CMAKE_RC_COMPILE_OBJECT "<CMAKE_RC_COMPILER> -O coff -o <OBJECT> -i <SOURCE>")
  enable_language(RC)
endif(DEFINED MINGW)

qt5_add_resources(RESOURCES
    resources/resources.qrc
)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${PROJECT_BINARY_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_BINARY_DIR}          # Generated .h files
  ${CMAKE_CURRENT_BINARY_DIR}  # For wrap/ui files
  ${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Gui_INCLUDE_DIRS}
  ${Qt5Multimedia_INCLUDE_DIRS}
	${Qt5Core_INCLUDE_DIRS}
	${Qt5WinExtras_INCLUDE_DIRS}	
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
)
  
set (EXTERNAL_LIBRARIES
	Qt5::Core
	Qt5::Gui
	Qt5::Widgets
	Qt5::Multimedia
  Qt5::Network
	Qt5::WinExtras
)
  
add_executable(Downloader WIN32 ${SOURCE_FILES})
target_link_libraries (Downloader ${EXTERNAL_LIBRARIES})
qt5_use_modules(Downloader Core Gui Widgets Multimedia WinExtras)