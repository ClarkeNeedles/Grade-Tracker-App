cmake_minimum_required(VERSION 3.16)

project(Grade-Tracker-App VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS
    Widgets
    Network
    Charts
)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS
    Widgets
    Network
    Charts
)

set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_SOURCE_DIR}/ui
)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        include/mainwindow.h
        ui/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Grade-Tracker-App
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        ui/mainwindow.ui
        include/addfrienddialog.h include/allcoursespage.h include/course.h include/coursecreatedialog.h include/coursedata.h include/coursepage.h include/database.h include/errordialog.h include/friend_data.h include/friendpage.h include/helpdialog.h include/homepage.h include/inputgradedialog.h include/loadingdialog.h include/loghoursdialog.h include/loginpage.h include/mainwindow.h include/rank_type.h include/timerdialog.h include/userdata.h
        resources/assets.qrc resources/resource.qrc
        src/addfrienddialog.cpp src/allcoursespage.cpp src/course.cpp src/coursecreatedialog.cpp src/coursedata.cpp src/coursepage.cpp src/database.cpp src/errordialog.cpp src/friend_data.cpp src/friendpage.cpp src/helpdialog.cpp src/homepage.cpp src/inputgradedialog.cpp src/loadingdialog.cpp src/loghoursdialog.cpp src/loginpage.cpp src/main.cpp src/mainwindow.cpp src/timerdialog.cpp src/userdata.cpp
        ui/addfrienddialog.ui ui/allcoursespage.ui ui/coursecreatedialog.ui ui/coursepage.ui ui/errordialog.ui ui/friendpage.ui ui/helpdialog.ui ui/homepage.ui ui/inputgradedialog.ui ui/loadingdialog.ui ui/loghoursdialog.ui ui/loginpage.ui ui/mainwindow.ui ui/timerdialog.ui
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Grade-Tracker-App APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(Grade-Tracker-App SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Grade-Tracker-App
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Grade-Tracker-App PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Charts
)

set_target_properties(Grade-Tracker-App PROPERTIES
    AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/ui"
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Grade-Tracker-App)
endif()
set_target_properties(Grade-Tracker-App PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Grade-Tracker-App
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Grade-Tracker-App)
endif()
