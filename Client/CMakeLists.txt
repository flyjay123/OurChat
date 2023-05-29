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

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} SRC_LIST)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Adding SRC_LIST_ADDING)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Chatting SRC_LIST_CHATTING)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/IconSetting SRC_LIST_ICONSETTING)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Tools SRC_LIST_TOOLS)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Rewriting SRC_LIST_REWRITING)

file(GLOB_RECURSE HEADER_FILES "*.h" "*.hpp")
source_group(TREE ${CMAKE_SOURCE_DIR} FILES ${SRC_LIST} ${SRC_LIST_ADDING} ${SRC_LIST_CHATTING} ${SRC_LIST_ICONSETTING} ${SRC_LIST_TOOLS} ${SRC_LIST_REWRITING} ${HEADER_FILES})

add_executable(OurChat src.qrc ${SRC_LIST} ${SRC_LIST_ADDING} ${SRC_LIST_CHATTING} ${SRC_LIST_ICONSETTING} ${SRC_LIST_TOOLS} ${SRC_LIST_REWRITING} ${HEADER_FILES})

target_include_directories(OurChat PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/Adding
        ${CMAKE_CURRENT_SOURCE_DIR}/Chatting
        ${CMAKE_CURRENT_SOURCE_DIR}/IconSetting
        ${CMAKE_CURRENT_SOURCE_DIR}/Tools
        ${CMAKE_CURRENT_SOURCE_DIR}/Rewriting
        ${CMAKE_CURRENT_SOURCE_DIR}
        )
if(MSVC)
    set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.9.9\\6.5.0\\msvc2019_64")
else()
    set(CMAKE_PREFIX_PATH "C:\\Qt\\Qt5.9.9\\6.5.0\\mingw_64")
endif()

find_package(Qt6 COMPONENTS
        Core Gui
        Widgets Network
        OpenGLWidgets
        REQUIRED)

qt6_add_resources(RESOURCE_FILES ${RESOURCES})

target_link_libraries(OurChat PRIVATE
  Qt::Core
  Qt::Gui
  Qt::Widgets Qt6::Network
  Qt6::OpenGLWidgets
)

set(LIBS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/libs")
if(NOT EXISTS ${LIBS_DIR})
    message(WARNING "libs directory does not exist: ${LIBS_DIR}")
endif()

file(GLOB DLL_FILES "${LIBS_DIR}/*.dll")
foreach(DLL_FILE ${DLL_FILES})
    add_custom_command(TARGET OurChat POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${DLL_FILE}
            $<TARGET_FILE_DIR:OurChat>)
endforeach()




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
