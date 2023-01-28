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
            sinastd/Parser.c
            sinastd/Error.c
            sinastd/Param.c
            sinastd/TimePeriod.c
            models/User.c
            models/TestCase.c
            models/Self.c
            models/Food.c
            views/console.c
            views/test.c
            controllers/UserController.c
            controllers/SelfController.c
            controllers/FoodController.c
            libbcrypt/bcrypt.c
            libbcrypt/crypt_blowfish/crypt_blowfish.c
            libbcrypt/crypt_blowfish/crypt_gensalt.c
            libbcrypt/crypt_blowfish/wrapper.c
            )

add_executable(FoodReservation ${SOURCES})
#add gcrypt library
# target_link_libraries(FoodReservation gcrypt)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lsqlite3 -lgcrypt -lm")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lsqlite3 -lm")
include(CPack)
