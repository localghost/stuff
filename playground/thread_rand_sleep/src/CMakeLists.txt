set(SRCS
    randomizer.cc
    )

add_definitions("-ggdb")

add_library(${PROJECT_NAME} SHARED ${SRCS})
target_link_libraries(${PROJECT_NAME} "-ldl -lboost_system")

set(SRCS_TEST
    test.cc
   )
add_executable("test${PROJECT_NAME}" ${SRCS_TEST})
target_link_libraries("test${PROJECT_NAME}" "-ldl -lboost_system -lboost_filesystem -lboost_thread -pthread" ${PROJECT_NAME})

set(SRCS_LOAD_TEST
    testload.cc
   )
add_executable("testload${PROJECT_NAME}" ${SRCS_LOAD_TEST})
target_link_libraries("testload${PROJECT_NAME}" "-ldl -pthread")
