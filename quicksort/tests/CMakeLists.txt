find_package(GTest CONFIG REQUIRED)

add_executable(${PROJECT_NAME}_test correctness_test.cpp)
target_link_libraries(${PROJECT_NAME}_test ${PROJECT_NAME}_lib GTest::gtest_main)