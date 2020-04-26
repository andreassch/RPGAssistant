cmake_minimum_required(VERSION 3.11)
project(tests)

set(CMAKE_CXX_STANDARD 17)

if (UNIX)
    add_compile_options(-Wall -Wextra)
endif()

find_package(Qt5 COMPONENTS Core REQUIRED)

set(source_files
    TestCalendar.cpp
    ../Backend/TDECalendar.h
    ../Backend/TDECalendar.cpp
    ../Backend/Holiday.h
    ../Backend/Holiday.cpp
)

add_executable(testCalendar
        ${source_files}
)

target_link_libraries(testCalendar
    PRIVATE
        Qt5::Core
)

