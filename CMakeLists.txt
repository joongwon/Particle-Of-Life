project("Particle Of Life")
cmake_minimum_required(VERSION 3.21)

set(src
        src/apicall.cpp
        src/apicall.h
        src/App.cpp
        src/App.h
        src/Button.cpp
        src/Button.h
        src/main.cpp
        src/ParticleOfLife.cpp
        src/ParticleOfLife.h
        src/ParticleOfLifeConfig.cpp
        src/ParticleOfLifeConfig.h)

add_executable(main ${src})
target_link_libraries(main sfml-graphics sfml-window sfml-system pthread)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/config.txt
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/icon.png
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/font.ttf
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
