cmake_minimum_required (VERSION 2.8)
project (T_final)
set(PROG1 client)


set(SOURCES1 src/client.c src/myf.c)




include_directories ("${PROJECT_SOURCE_DIR}/../../../Aula13/Ex1/srcmyutils")
link_directories ("${PROJECT_SOURCE_DIR}/../../../Aula13/Ex1/lib")

add_executable(${PROG1} ${SOURCES1})


find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})



target_link_libraries (${PROG1} myutils ${CURSES_LIBRARIES})


