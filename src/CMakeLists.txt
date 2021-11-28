set(SOURCE_FILES Vec2D.h Vec2D.cpp main.cpp)

add_executable(kami ${SOURCE_FILES})

# Requires "apt-get install libsfml-dev"
target_link_libraries(kami 
    sfml-graphics 
    sfml-window
    sfml-system
    sfml-network
    sfml-audio)
    
target_link_libraries(kami)
