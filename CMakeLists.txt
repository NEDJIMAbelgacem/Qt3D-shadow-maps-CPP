cmake_minimum_required(VERSION 3.5)

project(ShadowMapsCPP LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets 3DCore 3DRender 3DInput 3DQuick Qml Quick 3DQuickExtras REQUIRED)

if(ANDROID)
  add_library(ShadowMapsCPP SHARED
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
  )
else()
  add_executable(ShadowMapsCPP
    main.cpp
    mainwindow.cpp
    shadowmaplight.cpp
    groundplane.cpp
    toyplane.cpp
    trefoil.cpp
    shadowmapframegraph.cpp
    previewquad.cpp
    postprocessquad.cpp
    mainwindow.h
    shadowmaplight.h
    groundplane.h
    toyplane.h
    trefoil.h
    shadowmapframegraph.h
    previewquad.h
    postprocessquad.h
    mainwindow.ui
    Resources/Resources.qrc
  )
endif()

target_link_libraries(ShadowMapsCPP PRIVATE Qt5::Widgets Qt5::3DCore Qt5::3DRender Qt5::3DInput Qt5::3DQuick Qt5::Qml Qt5::Quick Qt5::3DQuickExtras)
