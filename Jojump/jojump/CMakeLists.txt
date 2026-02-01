project(jojump)

find_package(SFML REQUIRED network audio graphics window system)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/images" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

add_executable(jojump jojo.cpp)

target_link_libraries(doodle_jump PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
