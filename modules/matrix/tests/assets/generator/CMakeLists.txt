project(tests_assets_generator LANGUAGES CXX)

add_executable(${PROJECT_NAME}
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        assets::config
        common

        dumb_math::matrix
        dumb_math::random
)