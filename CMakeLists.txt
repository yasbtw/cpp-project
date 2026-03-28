cmake_minimum_required(VERSION 3.20)

project(CPPProject)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON) # std::println is on C++23 and above

add_subdirectory(thirdparty/glfw)
add_subdirectory(thirdparty/glad)

add_executable(main
    main.cpp
)

target_include_directories(main PRIVATE 
    thirdparty/glfw/include
    thirdparty/glad/include
) # these are pretty self explanatory
target_link_libraries(main PRIVATE glfw glad)