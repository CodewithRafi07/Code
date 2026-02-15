cmake_minimum_required(VERSION 3.16)
project(IROS-GUI VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 COMPONENTS Core Widgets REQUIRED)

set(SOURCES
    main.cpp
    mainwindow.cpp
    gui/registrationdialog.cpp
    gui/logindialog.cpp
    gui/productsearchdialog.cpp
    backend/product.cpp
    backend/customer.cpp
    backend/admin.cpp
    # Add other backend files as needed
)

set(HEADERS
    mainwindow.h
    gui/registrationdialog.h
    gui/logindialog.h
    gui/productsearchdialog.h
    backend/product.h
    backend/customer.h
    backend/admin.h
    # Add other backend headers as needed
)

qt_add_executable(IROS-GUI
    ${SOURCES}
    ${HEADERS}
    resources.qrc
)

target_link_libraries(IROS-GUI PRIVATE Qt6::Core Qt6::Widgets)