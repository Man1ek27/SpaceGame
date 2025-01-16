cmake_minimum_required(VERSION 3.10)
# Nazwa projektu
project(Schemat)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../bin)

set(SFML_DIR "C:/Users/Mariu/Documents/Library/SFML-2.6.2") 

include_directories(${SFML_DIR}/include)

link_directories(${SFML_DIR}/lib)
link_directories(C:/Users/Mariu/OneDrive/Dokumenty/Library/flac-1.4.3-win/Win64)

set(SOURCES src/main.cpp)

# Tworzenie pliku wykonywalnego
add_executable(Prog  ${SOURCES}) #WIN32



add_definitions(-DSFML_STATIC)

target_link_libraries(Prog
    sfml-graphics-s
    sfml-audio-s
    sfml-network-s
    sfml-window-s
    sfml-system-s
    sfml-main
    FLAC
    vorbisfile
    vorbisenc
    vorbis
    ogg
    openal32
    ws2_32
    opengl32
    freetype
    winmm
    gdi32
    
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
