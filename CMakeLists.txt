cmake_minimum_required(VERSION 3.5)

project(editor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

add_executable(editor
    main.cpp
    mainwindow.cpp
    mainwindow.h
    accessarray.h
    roundedpolygon.h
    tabpage.h
    tabctrl.h
  )

target_link_libraries(editor PRIVATE Qt5::Widgets)
