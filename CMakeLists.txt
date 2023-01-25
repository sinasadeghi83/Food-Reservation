cmake_minimum_required(VERSION 3.0.0)
# project(FoodReservation VERSION 0.1.0)
project(FoodReservation C)
# project(FoodReservation C)

include(CTest)
enable_testing()

set(SOURCES main.c
            database/DbManager.c
            database/Migration.c
            sinastd/Date.c
            sinastd/Parser.c)

add_executable(FoodReservation ${SOURCES})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lsqlite3")
include(CPack)
