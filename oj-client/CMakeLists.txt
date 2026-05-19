cmake_minimum_required(VERSION 3.19)
project(oj LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Network Sql)

qt_standard_project_setup()

qt_add_executable(oj
    MACOSX_BUNDLE
    main.cpp
    src/ui/mainwindow.cpp
    src/ui/mainwindow.h
    src/ui/mainwindow.ui
    src/ui/pages/loginpage.cpp
    src/ui/pages/loginpage.h
    src/ui/pages/homepage.cpp
    src/ui/pages/homepage.h
    src/ui/pages/classpage.cpp
    src/ui/pages/classpage.h
    src/ui/pages/contestpage.cpp
    src/ui/pages/contestpage.h
    src/ui/pages/problempage.cpp
    src/ui/pages/problempage.h
    src/ui/pages/favoritepage.cpp
    src/ui/pages/favoritepage.h
    src/network/cookiestore.cpp
    src/network/cookiestore.h
    src/network/openjudgeclient.cpp
    src/network/openjudgeclient.h
    src/parser/loginparser.cpp
    src/parser/loginparser.h
    src/parser/classparser.cpp
    src/parser/classparser.h
    src/parser/groupparser.cpp
    src/parser/groupparser.h
    src/parser/contestparser.cpp
    src/parser/contestparser.h
    src/parser/problemparser.cpp
    src/parser/problemparser.h
    src/parser/resultparser.cpp
    src/parser/resultparser.h
    src/parser/submitparser.cpp
    src/parser/submitparser.h
    src/parser/parsercommon.cpp
    src/parser/parsercommon.h
    src/repository/submit/homerepository.cpp
    src/repository/submit/homerepository.h
    src/repository/cache/homecacherepository.cpp
    src/repository/cache/homecacherepository.h
    src/repository/cache/classcacherepository.cpp
    src/repository/cache/classcacherepository.h
    src/repository/submit/classrepository.cpp
    src/repository/submit/classrepository.h
    src/repository/cache/contestcacherepository.cpp
    src/repository/cache/contestcacherepository.h
    src/repository/submit/contestrepository.cpp
    src/repository/submit/contestrepository.h
    src/repository/submit/problemrepository.cpp
    src/repository/submit/problemrepository.h
    src/repository/submit/resultrepository.cpp
    src/repository/submit/resultrepository.h
    src/repository/submit/submitrepository.cpp
    src/repository/submit/submitrepository.h
    src/repository/cache/problemcacherepository.cpp
    src/repository/cache/problemcacherepository.h
    src/repository/favorite/favoriteproblemrepository.cpp
    src/repository/favorite/favoriteproblemrepository.h
    src/service/browse/homeservice.cpp
    src/service/browse/homeservice.h
    src/service/browse/classservice.cpp
    src/service/browse/classservice.h
    src/service/browse/contestservice.cpp
    src/service/browse/contestservice.h
    src/service/browse/problemservice.cpp
    src/service/browse/problemservice.h
    src/service/cache/cacheservice.cpp
    src/service/cache/cacheservice.h
    src/service/reminder/reminderservice.cpp
    src/service/reminder/reminderservice.h
    src/service/submit/resultservice.cpp
    src/service/submit/resultservice.h
    src/service/submit/submitservice.cpp
    src/service/submit/submitservice.h
    src/service/favorite/favoriteproblemservice.cpp
    src/service/favorite/favoriteproblemservice.h
    src/service/login/loginservice.cpp
    src/service/login/loginservice.h
)

target_include_directories(oj
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(oj
    PRIVATE
        Qt::Core
        Qt::Widgets
        Qt::Network
        Qt::Sql
)

if (WIN32)
    get_target_property(QT_QMAKE_EXECUTABLE Qt6::qmake IMPORTED_LOCATION)
    get_filename_component(QT_BIN_DIR "${QT_QMAKE_EXECUTABLE}" DIRECTORY)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${QT_BIN_DIR}")

    if (WINDEPLOYQT_EXECUTABLE)
        add_custom_command(TARGET oj POST_BUILD
            COMMAND "${WINDEPLOYQT_EXECUTABLE}" --no-compiler-runtime "$<TARGET_FILE:oj>"
            COMMENT "Deploying Qt runtime dependencies"
        )
    endif()
endif()

install(TARGETS oj
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET oj
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
