project(minstd_validation_tests)

add_executable(${PROJECT_NAME}
    scalar_skipahead_test.cpp
    vector_skipahead_test.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        GTest::gtest
        GTest::gtest_main
        dumb_math
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

include(GoogleTest)
gtest_discover_tests(${PROJECT_NAME})