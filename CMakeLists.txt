cmake_minimum_required(VERSION 3.1.0)

project(QtDmSfuClientLib)

# Instruct CMake to run moc automatically when needed
set( CMAKE_AUTOMOC ON )
set( CMAKE_CXX_STANDARD 14 )

find_package( Qt5Core REQUIRED )
find_package( dm-sfu-signaling REQUIRED )

add_library( QtDmSfuClientLib
  ${CMAKE_CURRENT_LIST_DIR}/qt-sfu_signaling.cpp
  ${CMAKE_CURRENT_LIST_DIR}/qt-sfu_signaling.h
  ${CMAKE_CURRENT_LIST_DIR}/qtsfuclientlib_global.h
  )
target_link_libraries(QtDmSfuClientLib
  dm-sfu-signaling::dm-sfu-signaling
  Qt5::Core
  )
target_compile_definitions( QtDmSfuClientLib PRIVATE QTSFUCLIENTLIB_LIBRARY)

