set (LIB_NAME QtDmSfuClientLib)

# Instruct CMake to run moc automatically when needed
set( CMAKE_AUTOMOC ON )
set( CMAKE_CXX_STANDARD 14 )

find_package( Qt5Core REQUIRED )
find_package( dm-sfu-signaling REQUIRED )

add_library( ${LIB_NAME} STATIC
  ${CMAKE_CURRENT_LIST_DIR}/qt_sfu_signaling.cpp
  ${CMAKE_CURRENT_LIST_DIR}/qt_sfu_signaling.h
  )
target_link_libraries(${LIB_NAME}
  dm-sfu-signaling::dm-sfu-signaling
  Qt5::Core
  )
target_include_directories(${LIB_NAME} INTERFACE ${CMAKE_CURRENT_LIST_DIR})
target_compile_definitions( ${LIB_NAME} PUBLIC QT_NO_KEYWORDS)

