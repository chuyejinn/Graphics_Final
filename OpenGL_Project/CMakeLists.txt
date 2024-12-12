cmake_minimum_required(VERSION 3.10)

# 프로젝트 이름
project(OpenGL_Project)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 소스 파일 추가
set(SOURCES
    main.cpp
    src/objects.cpp
    src/glad.c
    src/shader.cpp
)

# include 디렉토리 추가
include_directories(${CMAKE_SOURCE_DIR}/src /opt/homebrew/include)

# 라이브러리 디렉토리 추가
link_directories(/opt/homebrew/lib)

# 실행 파일 생성
add_executable(OpenGL_Project ${SOURCES})

# 라이브러리 링크
target_link_libraries(OpenGL_Project glfw "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")