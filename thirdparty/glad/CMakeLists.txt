cmake_minimum_required(VERSION 3.20)

# yes

add_library(glad STATIC
    src/glad.c
    include/glad/glad.h
    include/KHR/khrplatform.h
)
# do we write this manually or get it online
target_include_directories(glad PUBLIC include)