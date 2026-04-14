project(tests_assets_generator LANGUAGES CXX)

add_library(${PROJECT_NAME} STATIC 
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        assets::config
        common
        
        matrix
        random
)