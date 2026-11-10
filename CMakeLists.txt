set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_minimum_required(VERSION 3.19)
project(Calculator LANGUAGES CXX)


find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets Qml Test)

qt_standard_project_setup()

add_library(CoreLib STATIC
    src/core/clcengine.h
    src/core/clcengine.cpp
    src/core/programmerengine.h
    src/core/programmerengine.cpp
    src/core/StringUtils.h
)

target_include_directories(CoreLib PUBLIC src/core)
target_link_libraries(CoreLib PUBLIC Qt6::Core)


qt_add_executable(Calculator
    WIN32 MACOSX_BUNDLE
    src/main.cpp

    resources/app.qrc

    src/app/mainwindow.h
    src/app/mainwindow.cpp
    src/app/mainwindow.ui
    src/app/Themes.h

    src/modes/standardcalculator.h
    src/modes/standardcalculator.cpp
    src/modes/standardcalculator.ui
    src/modes/scientificcalculator.h
    src/modes/scientificcalculator.cpp
    src/modes/scientificcalculator.ui
    src/modes/programmercalculator.h
    src/modes/programmercalculator.cpp
    src/modes/programmercalculator.ui
    src/modes/datecalculator.h
    src/modes/datecalculator.cpp
    src/modes/datecalculator.ui

    src/graph/graphingcalculator.h
    src/graph/graphingcalculator.cpp
    src/graph/graphingcalculator.ui
    src/graph/graphscene.h
    src/graph/graphscene.cpp

    src/app/UiHelper.h
    src/app/Icons.h
    src/app/TintColor.h
)

target_include_directories(Calculator PRIVATE
    src
    src/app
    src/core
    src/modes
    src/graph
)


target_link_libraries(Calculator
    PRIVATE
        CoreLib
        Qt6::Core
        Qt6::Widgets
        Qt6::Qml
)

if(WIN32)
    target_sources(Calculator PRIVATE resources/app.rc)
endif()

include(CTest)

qt_add_executable(TestClcEngine
    tests/tst_clcengine.cpp
)
target_include_directories(TestClcEngine PRIVATE src)
target_link_libraries(TestClcEngine PRIVATE CoreLib Qt6::Core Qt6::Test)
add_test(NAME test_clc_engine COMMAND TestClcEngine)

qt_add_executable(TestProgrammerEngine
    tests/tst_programmerengine.cpp
)
target_include_directories(TestProgrammerEngine PRIVATE src)
target_link_libraries(TestProgrammerEngine PRIVATE CoreLib Qt6::Core Qt6::Test)
add_test(NAME test_programmer_engine COMMAND TestProgrammerEngine)
install(TARGETS Calculator
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET Calculator
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})






