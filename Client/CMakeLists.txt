cmake_minimum_required(VERSION 3.24)
project(OurChat)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(MSVC)
    add_compile_options(/std:c++17)
    add_compile_options(/Zc:__cplusplus)
    add_compile_options(/permissive-)
    add_compile_options(/utf-8)
endif()

set(FORMS
        Adding/addfriend.ui
        Adding/systemmessage.ui
        Adding/verificationitem.ui
        Chatting/chatwindow.ui
        IconSetting/iconselect.ui
        Rewriting/frienditem.ui
        client.ui
        friendinfowidget.ui
        logging.ui
        selfinfowidget.ui
        )

set(HEADERS
        Adding/addfriend.h
        Adding/systemmessage.h
        Adding/verificationitem.h
        Chatting/chatlistwidget.h
        Chatting/chatwindow.h
        IconSetting/iconselect.h
        Rewriting/friendiconlabel.h
        Rewriting/frienditem.h
        Rewriting/sendtextedit.h
        Tools/stringtool.h
        Tools/tcpclient.h
        client.h
        friendinfowidget.h
        logging.h
        selfinfowidget.h
        )

set(SOURCES
        Adding/addfriend.cpp
        Adding/systemmessage.cpp
        Adding/verificationitem.cpp
        Chatting/chatlistwidget.cpp
        Chatting/chatwindow.cpp
        IconSetting/iconselect.cpp
        Rewriting/friendiconlabel.cpp
        friendinfowidget.cpp
        Rewriting/frienditem.cpp
        Rewriting/sendtextedit.cpp
        Tools/stringtool.cpp
        Tools/tcpclient.cpp
        client.cpp
        logging.cpp
        main.cpp
        selfinfowidget.cpp
        )

set(RESOURCES
        src.qrc
        )

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/Adding
        ${CMAKE_CURRENT_SOURCE_DIR}/Chatting
        ${CMAKE_CURRENT_SOURCE_DIR}/IconSelect
        ${CMAKE_CURRENT_SOURCE_DIR}/Tools
        ${CMAKE_CURRENT_SOURCE_DIR}/Rewriting
        ${CMAKE_CURRENT_SOURCE_DIR}
)

set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.9.9\\6.5.0\\mingw_64")

find_package(Qt6 COMPONENTS
  Core Gui
  Widgets Network
  REQUIRED)

qt6_add_resources(RESOURCE_FILES ${RESOURCES})

add_executable(OurChat src.qrc ${SOURCES} ${HEADERS} ${UI_HEADERS} ${RESOURCE_FILES})

#find_library(LIBEAY32_LIBRARY NAMES libeay32.dll PATHS ${CMAKE_CURRENT_SOURCE_DIR}/libs)
#find_library(SSLEAY32_LIBRARY NAMES ssleay32.dll PATHS ${CMAKE_CURRENT_SOURCE_DIR}/libs)

target_link_libraries(OurChat PRIVATE
  Qt::Core
  Qt::Gui
  Qt::Widgets Qt6::Network
)

if(WIN32)
    set(CMAKE_WIN32_EXECUTABLE ON)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    foreach (QT_LIB Core Gui Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
