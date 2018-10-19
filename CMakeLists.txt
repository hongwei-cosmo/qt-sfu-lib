cmake_minimum_required(VERSION 3.1.0)

project(QtDmSfuClientLib)

# Instruct CMake to run moc automatically when needed
set( CMAKE_AUTOMOC ON )
set( CMAKE_CXX_STANDARD 14 )

if (WIN32)
  # Custom source list for Windows Desktop
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MT")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MTd")
endif()

find_package( Qt5Core REQUIRED )
find_package( dm-sfu-signaling REQUIRED )

add_library( ${PROJECT_NAME} STATIC
  ${CMAKE_CURRENT_LIST_DIR}/qt_sfu_signaling.cpp
  ${CMAKE_CURRENT_LIST_DIR}/qt_sfu_signaling.h
  ${CMAKE_CURRENT_LIST_DIR}/qtsfuclientlib_global.h
  )
target_link_libraries(${PROJECT_NAME}
  dm-sfu-signaling::dm-sfu-signaling
  Qt5::Core
  )
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions( ${PROJECT_NAME} PRIVATE QTSFUCLIENTLIB_LIBRARY)
target_compile_definitions( ${PROJECT_NAME} PUBLIC QT_NO_KEYWORDS)

